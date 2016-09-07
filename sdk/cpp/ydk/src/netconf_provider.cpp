/// YANG Development Kit
// Copyright 2016 Cisco Systems. All rights reserved
//
////////////////////////////////////////////////////////////////
// Licensed to the Apache Software Foundation (ASF) under one
// or more contributor license agreements.  See the NOTICE file
// distributed with this work for additional information
// regarding copyright ownership.  The ASF licenses this file
// to you under the Apache License, Version 2.0 (the
// "License"); you may not use this file except in compliance
// with the License.  You may obtain a copy of the License at
//
//   http://www.apache.org/licenses/LICENSE-2.0
//
//  Unless required by applicable law or agreed to in writing,
// software distributed under the License is distributed on an
// "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
// KIND, either express or implied.  See the License for the
// specific language governing permissions and limitations
// under the License.
//
//////////////////////////////////////////////////////////////////

#include <iostream>
#include <sstream>

#include "types.hpp"
#include "netconf_client.hpp"
#include "netconf_provider.hpp"
#include "core.hpp"
#include "entity_data_node_walker.hpp"
#include "errors.hpp"

using namespace std;
using namespace ydk;

namespace ydk
{
static core::SchemaNode* get_schema_for_operation(core::RootSchemaNode& root_schema, string operation);

static unique_ptr<core::Rpc> create_rpc_instance(core::RootSchemaNode & root_schema, string rpc_name);
static core::DataNode* create_rpc_input(core::Rpc & netconf_rpc);

static bool is_candidate_supported(vector<string> capbilities);
static void create_input_target(core::DataNode & input, bool candidate_supported);
static void create_input_source(core::DataNode & input, bool config);
static void create_input_error_option(core::DataNode & input);
static string get_annotated_config_payload(core::RootSchemaNode* root_schema, core::Rpc & rpc, core::Annotation & annotation);
static string get_commit_rpc_payload();
static core::DataNode* handle_edit_reply(string reply, NetconfClient & client, bool candidate_supported);

static string get_read_rpc_name(bool config);
static bool is_config(core::Rpc & rpc);
static string get_filter_payload(core::Rpc & ydk_rpc);
static string get_netconf_payload(core::DataNode* input, string data, string data_tag);
static core::DataNode* handle_read_reply(string reply, core::RootSchemaNode * root_schema);

const char* NetconfServiceProvider::WRITABLE_RUNNING = "urn:ietf:params:netconf:capability:writable-running:1.0";
const char* NetconfServiceProvider::CANDIDATE = "urn:ietf:params:netconf:capability:candidate:1.0";
const char* NetconfServiceProvider::ROLLBACK_ON_ERROR = "urn:ietf:params:netconf:capability:rollback-on-error:1.0";
const char* NetconfServiceProvider::STARTUP = "urn:ietf:params:netconf:capability:startup:1.0";
const char* NetconfServiceProvider::URL = "urn:ietf:params:netconf:capability:url:1.0";
const char* NetconfServiceProvider::XPATH = "urn:ietf:params:netconf:capability:xpath:1.0";
const char* NetconfServiceProvider::BASE_1_1 = "urn:ietf:params:netconf:base:1.1";
const char* NetconfServiceProvider::CONFIRMED_COMMIT_1_1 = "urn:ietf:params:netconf:capability:confirmed-commit:1.1";
const char* NetconfServiceProvider::VALIDATE_1_1 = "urn:ietf:params:netconf:capability:validate:1.1";
const char* NetconfServiceProvider::NS = "urn:ietf:params:xml:ns:netconf:base:1.0";
const char* NetconfServiceProvider::MODULE_NAME = "ietf-netconf";


NetconfServiceProvider::NetconfServiceProvider(const core::Repository* repo, string address, string username, string password, int port)
    : m_repo{repo}, client(make_unique<NetconfClient>(username, password, address, port, 0))
{
    if(m_repo == nullptr) {
        throw core::YDKInvalidArgumentException{"repo is null"};
    }

    client->connect();
    client_capabilities = client->get_capabilities();
    std::vector<core::Capability> yang_caps {};

    for(std::string c : client_capabilities ){

        auto p = std::find(c.begin(), c.end(),'?');

        if(p == c.end())
            continue;

        auto module_start = c.find("module=");


        if(module_start == std::string::npos)
            continue;

        auto revision_start = c.find("revision=");
        if(revision_start == std::string::npos)
            continue;

        std::vector<std::string> c_features{};
        std::vector<std::string> c_deviations{};

        auto module_end = c.find("&", module_start);

        module_start+=sizeof("module=");
        auto size = module_end;
        if(size != string::npos ){
            size = module_end - module_start + 1;
        }

        std::string c_module = c.substr( module_start - 1, size );


        auto revision_end = c.find("&", revision_start);
        revision_start+=sizeof("revision=");
        size = revision_end;
        if(size!= string::npos) {
            size= revision_end - revision_start + 1;
        }
        std::string c_revision = c.substr(revision_start - 1, size);

        auto features_start = c.find("features=");
        if(features_start != string::npos){
            auto features_end = c.find("&", features_start);
            features_start+=sizeof("features=");
            size=features_end;
            if(size!=string::npos){
                size = features_end - features_start + 1;
            }
            std::string features = c.substr(features_start - 1 , size);
            std::istringstream iss{features};
            std::string feature;
            while(std::getline(iss, feature, ',')) {
                c_features.push_back(std::move(feature));
            }

        }

        auto deviations_start = c.find("deviations=");
        if(deviations_start != string::npos){
            auto deviations_end = c.find("&", deviations_start);
            deviations_start+=sizeof("deviations=");
            size=deviations_end;
            if(size!=string::npos){
                size = deviations_end - deviations_start + 1;
            }
            std::string deviations = c.substr(deviations_start - 1, size);
            std::istringstream iss{deviations};
            std::string deviation;
            while(std::getline(iss, deviation, ',')) {
                c_deviations.push_back(std::move(deviation));
            }

        }
        if(c_module.find("tailf") != std::string::npos) {
            continue;
        }
        core::Capability core_cap{c_module, c_revision, c_features, c_deviations};
        yang_caps.emplace_back(core_cap);


    }

    root_schema = std::unique_ptr<ydk::core::RootSchemaNode>(m_repo->create_root_schema(yang_caps));

    if(root_schema.get() == nullptr) {
        throw core::YDKIllegalStateException{"Root schema cannot be obtained"};
    }

    create_schema = get_schema_for_operation(*root_schema, "ydk:create");
    read_schema = get_schema_for_operation(*root_schema, "ydk:read");
    update_schema  = get_schema_for_operation(*root_schema, "ydk:update");;
    delete_schema = get_schema_for_operation(*root_schema, "ydk:delete");;
}

NetconfServiceProvider::~NetconfServiceProvider()
{
	client->close();
}

core::RootSchemaNode* NetconfServiceProvider::get_root_schema()
{
    return root_schema.get();
}

core::DataNode* NetconfServiceProvider::handle_read(core::Rpc* ydk_rpc) const
{
    //for now we only support crud rpc's
    bool config = is_config(*ydk_rpc);
    auto netconf_rpc = create_rpc_instance(*root_schema, get_read_rpc_name(config));
    auto input = create_rpc_input(*netconf_rpc);
    create_input_source(*input, config);
    std::string filter_value = get_filter_payload(*ydk_rpc);

    string netconf_payload = get_netconf_payload(input, filter_value, "filter");
    std::cout << netconf_payload << std::endl;

    std::string reply = client->execute_payload(netconf_payload);
    std::cout << reply << std::endl;
    return handle_read_reply(reply, root_schema.get());
}

core::DataNode* NetconfServiceProvider::handle_edit(core::Rpc* ydk_rpc, core::Annotation annotation) const
{
    //for now we only support crud rpc's
    bool candidate_supported = is_candidate_supported(client_capabilities);

    auto netconf_rpc = create_rpc_instance(*root_schema, "ietf-netconf:edit-config");
    auto input = create_rpc_input(*netconf_rpc);
    create_input_target(*input, candidate_supported);
    create_input_error_option(*input);
    string config_payload = get_annotated_config_payload(root_schema.get(), *ydk_rpc, annotation);

    string netconf_payload = get_netconf_payload(input, config_payload, "config");
    std::cout << netconf_payload << std::endl;

    std::string reply = client->execute_payload(netconf_payload);
    std::cout << reply << std::endl;
    return handle_edit_reply(reply, *client, candidate_supported);
}

core::DataNode* NetconfServiceProvider::invoke(core::Rpc* rpc) const
{
    //sanity check of rpc
    if(rpc == nullptr)
    {
        throw core::YDKInvalidArgumentException{"rpc is null!"};
    }

     //for now we only support crud rpc's
    core::SchemaNode* rpc_schema = rpc->schema();
    core::DataNode* datanode = nullptr;

    if(rpc_schema == create_schema || rpc_schema == delete_schema || rpc_schema == update_schema)
    {
        //for each child node in datanode add the nc:operation attribute
        core::Annotation an{NetconfServiceProvider::MODULE_NAME, "operation", rpc_schema == delete_schema ? "delete" : "merge"};
        return handle_edit(rpc, an);
    }
    else if(rpc_schema == read_schema)
    {
        return handle_read(rpc);

    }
    else
    {
        throw core::YDKOperationNotSupportedException{"rpc is not supported!"};
    }

    return datanode;
}

static unique_ptr<core::Rpc> create_rpc_instance(core::RootSchemaNode & root_schema, string rpc_name)
{
	auto rpc = unique_ptr<core::Rpc>(root_schema.rpc(rpc_name));
	if(rpc == nullptr)
	{
		throw core::YDKIllegalStateException{"Cannot create payload for RPC: "+ rpc_name};
	}
	return rpc;
}

static core::DataNode* create_rpc_input(core::Rpc & netconf_rpc)
{
	return netconf_rpc.input();
}

static string get_commit_rpc_payload()
{
	return "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
		   "<commit/>"
			"</rpc>";
}

static bool is_candidate_supported(vector<string> capabilities)
{
	if(std::find(capabilities.begin(), capabilities.end(), NetconfServiceProvider::CANDIDATE) != capabilities.end()){
		//candidate is supported
		return true;
	}
	return false;
}

static void create_input_target(core::DataNode & input, bool candidate_supported)
{
    if(candidate_supported){
        if(!input.create("target/candidate", "")){
            throw core::YDKIllegalStateException{"Failed setting target datastore"};
        }
    } else {
        if(!input.create("target/running", "")){
            throw core::YDKIllegalStateException{"Failed setting running datastore"};
        }
    }
}

static void create_input_error_option(core::DataNode & input)
{
	if(!input.create("error-option", "rollback-on-error"))
	{
		throw core::YDKIllegalStateException{"Failed to set rollback-on-error option"};
	}
}

static void create_input_source(core::DataNode & input, bool config)
{
	if(config && !input.create("source/running"))
	{
		throw core::YDKIllegalStateException{"Failed setting source"};
	}
}

static string get_annotated_config_payload(core::RootSchemaNode* root_schema,
		core::Rpc & rpc, core::Annotation & annotation)
{
	core::CodecService codec_service{};
    auto entity = rpc.input()->find("entity");
    if(entity.empty()){
        throw core::YDKInvalidArgumentException{"Failed to get entity node"};
    }

    core::DataNode* entity_node = entity[0];
    std::string entity_value = entity_node->get();

    //deserialize the entity_value
    core::DataNode* datanode = codec_service.decode(root_schema, entity_value, core::CodecService::Format::XML);

    if(!datanode){
        throw core::YDKInvalidArgumentException{"Failed to deserialize entity node"};
    }

    std::string config_payload {};

    for(auto child : datanode->children())
    {
        child->add_annotation(annotation);
        config_payload += codec_service.encode(child, core::CodecService::Format::XML, false);
    }
    return config_payload;
}

static string get_filter_payload(core::Rpc & ydk_rpc)
{
    auto entity = ydk_rpc.input()->find("filter");
    if(entity.empty()){
        throw core::YDKInvalidArgumentException{"Failed to get entity node"};
    }

    auto datanode = entity[0];
    return datanode->get();
}

static string get_netconf_payload(core::DataNode* input, string data_value, string data_tag)
{
	core::CodecService codec_service{};
    auto config_node = input->create(data_tag, data_value);
    if(!config_node)
    {
        throw core::YDKIllegalStateException{"Failed to create data tree"};
    }

    std::string payload{"<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"};
    payload+=codec_service.encode(input, core::CodecService::Format::XML, false);
    payload+="</rpc>";
    return payload;
}

static core::DataNode* handle_edit_reply(string reply, NetconfClient & client, bool candidate_supported)
{
	if(reply.find("<ok/>") == std::string::npos)
	{
		throw YDKServiceProviderException{reply};
	}

	if(candidate_supported)
	{
		//need to send the commit request
		reply = client.execute_payload(get_commit_rpc_payload());
		if(reply.find("<ok/>") == std::string::npos)
		{
			throw YDKServiceProviderException{reply};
		}
	}

	//no error no output for edit-config
	return nullptr;
}

static core::DataNode* handle_read_reply(string reply, core::RootSchemaNode * root_schema)
{
	core::CodecService codec_service{};
	auto empty_data = reply.find("<data/>");
	if(empty_data != std::string::npos)
	{
		return nullptr;
	}

	auto data_start = reply.find("<data>");
	if(data_start == std::string::npos)
	{
		throw YDKServiceProviderException{reply};
	}
	data_start+= sizeof("<data>") - 1;
	auto data_end = reply.find("</data>", data_start);
	if(data_end == std::string::npos)
	{
		throw YDKException{"No end data tag found"};
	}

	string data = reply.substr(data_start, data_end-data_start);

	auto datanode = codec_service.decode(root_schema, data, core::CodecService::Format::XML);

	if(!datanode){
		throw YDKException{"Problems deserializing output"};
	}
	return datanode;
}

static string get_read_rpc_name(bool config)
{
    if(config)
    {
        return "ietf-netconf:get-config";
    }
    return "ietf-netconf:get";
}

static bool is_config(core::Rpc & rpc)
{
	if(!rpc.input()->find("only-config").empty())
	{
		return true;
	}
	return false;
}

static core::SchemaNode* get_schema_for_operation(core::RootSchemaNode & root_schema, string operation)
{
	auto c = root_schema.find(operation);
	if(c.empty()){
		throw core::YDKIllegalStateException{"CRUD create rpc schema not found!"};
	}
	return c[0];
}
}
