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
#include "netconf_model_provider.hpp"
#include "entity_data_node_walker.hpp"
#include "errors.hpp"
#include "ydk_yang.hpp"
#include <memory>
#include <boost/log/trivial.hpp>

using namespace std;
using namespace ydk;

namespace ydk
{
static core::SchemaNode* get_schema_for_operation(core::RootSchemaNode& root_schema, string operation);
static std::vector<ydk::core::Capability> get_core_capabilities(const std::vector<std::string> & server_capabilities);

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

const char* NetconfServiceProvider::CANDIDATE = "urn:ietf:params:netconf:capability:candidate:1.0";
const char* NetconfServiceProvider::MODULE_NAME = "ietf-netconf";

NetconfServiceProvider::NetconfServiceProvider(core::Repository* repo, string address, string username, string password, int port)
    : m_repo{repo}, client(make_unique<NetconfClient>(username, password, address, port, 0)),
	  model_provider(make_unique<NetconfModelProvider>(*client))
{
    if(m_repo == nullptr) {
        BOOST_LOG_TRIVIAL(debug) << "Repo passed in is nullptr";
        throw YDKInvalidArgumentException{"repo is null"};
    }

    client->connect();
    server_capabilities = client->get_capabilities();

	for(std::string c : server_capabilities )
	{
		if(c.find("ietf-netconf-monitoring") != std::string::npos)
		{
			ietf_nc_monitoring_available = true;
			m_repo->add_model_provider(model_provider.get());
		}
	}

	std::vector<core::Capability> yang_caps {};
	for(std::string c : server_capabilities )
	{
		if(c.find("calvados") != std::string::npos || c.find("tailf") != std::string::npos)
		{
			continue;
		}

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

	//add ydk capability
	core::Capability ydk_cap{ydk::core::YDK_MODULE_NAME, ydk::core::YDK_MODULE_REVISION, {}, {}};
	auto result = std::find(yang_caps.begin(), yang_caps.end(), ydk_cap);
	if(result == yang_caps.end()){
		yang_caps.push_back(ydk_cap);
	}
	//add ietf-netconf capability
	core::Capability ietf_netconf_cap{ydk::IETF_NETCONF_MODULE_NAME, ydk::IETF_NETCONF_MODULE_REVISION, {}, {}};
	result = std::find(yang_caps.begin(), yang_caps.end(), ietf_netconf_cap);
	if(result == yang_caps.end()){
		yang_caps.push_back(ietf_netconf_cap);
	}

    root_schema = std::unique_ptr<ydk::core::RootSchemaNode>(m_repo->create_root_schema
									(
									//get_core_capabilities(server_capabilities)
									yang_caps
									)
								);

    if(root_schema.get() == nullptr) {
        BOOST_LOG_TRIVIAL(debug) << "Root schema cannot be obtained";
        throw YDKIllegalStateException{"Root schema cannot be obtained"};
    }
}

NetconfServiceProvider::~NetconfServiceProvider()
{
	client->close();
	if(ietf_nc_monitoring_available){
		m_repo->remove_model_provider(model_provider.get());
	}
}

core::RootSchemaNode* NetconfServiceProvider::get_root_schema() const
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

    std::string reply = client->execute_payload(netconf_payload);
    BOOST_LOG_TRIVIAL(debug) <<"=============Reply payload=============";
    BOOST_LOG_TRIVIAL(debug) << reply;
    BOOST_LOG_TRIVIAL(debug) <<"=========================="<<endl;
    return handle_read_reply(reply, root_schema.get());
}

core::DataNode* NetconfServiceProvider::handle_edit(core::Rpc* ydk_rpc, core::Annotation annotation) const
{
    //for now we only support crud rpc's
    bool candidate_supported = is_candidate_supported(server_capabilities);

    auto netconf_rpc = create_rpc_instance(*root_schema, "ietf-netconf:edit-config");
    auto input = create_rpc_input(*netconf_rpc);
    create_input_target(*input, candidate_supported);
    create_input_error_option(*input);
    string config_payload = get_annotated_config_payload(root_schema.get(), *ydk_rpc, annotation);

    string netconf_payload = get_netconf_payload(input, config_payload, "config");

    std::string reply = client->execute_payload(netconf_payload);
    BOOST_LOG_TRIVIAL(debug) <<"=============Reply payload=============";
    BOOST_LOG_TRIVIAL(debug) << reply;
    BOOST_LOG_TRIVIAL(debug) <<"=========================="<<endl;
    return handle_edit_reply(reply, *client, candidate_supported);
}

core::DataNode* NetconfServiceProvider::invoke(core::Rpc* rpc) const
{
	core::SchemaNode* create_schema;
	core::SchemaNode* read_schema;
	core::SchemaNode* update_schema;
	core::SchemaNode* delete_schema;
	create_schema = get_schema_for_operation(*root_schema, "ydk:create");
	read_schema = get_schema_for_operation(*root_schema, "ydk:read");
	update_schema  = get_schema_for_operation(*root_schema, "ydk:update");
	delete_schema = get_schema_for_operation(*root_schema, "ydk:delete");

    //sanity check of rpc
    if(rpc == nullptr)
    {
        BOOST_LOG_TRIVIAL(error) << "rpc is nullptr";
        throw YDKInvalidArgumentException{"rpc is null!"};
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
        BOOST_LOG_TRIVIAL(error) << "rpc is not supported";
        throw YDKOperationNotSupportedException{"rpc is not supported!"};
    }

    return datanode;
}

static unique_ptr<core::Rpc> create_rpc_instance(core::RootSchemaNode & root_schema, string rpc_name)
{
	auto rpc = unique_ptr<core::Rpc>(root_schema.rpc(rpc_name));
	if(rpc == nullptr){
            BOOST_LOG_TRIVIAL(error) << "Cannot create payload for RPC: " << rpc_name;
            throw YDKIllegalStateException{"Cannot create payload for RPC: "+ rpc_name};
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
            BOOST_LOG_TRIVIAL(debug) << "Failed setting target datastore";
            throw YDKIllegalStateException{"Failed setting target datastore"};
        }
    } else {
        if(!input.create("target/running", "")){
            BOOST_LOG_TRIVIAL(debug) << "Failed setting running datastore";
            throw YDKIllegalStateException{"Failed setting running datastore"};
        }
    }
}

static void create_input_error_option(core::DataNode & input)
{
	if(!input.create("error-option", "rollback-on-error")){
            BOOST_LOG_TRIVIAL(debug) << "Failed to set rollback-on-error";
            throw YDKIllegalStateException{"Failed to set rollback-on-error option"};
	}
}

static void create_input_source(core::DataNode & input, bool config)
{
	if(config && !input.create("source/running"))
	{
            BOOST_LOG_TRIVIAL(debug) << "Failed setting source";
            throw YDKIllegalStateException{"Failed setting source"};
	}
}

static string get_annotated_config_payload(core::RootSchemaNode* root_schema,
		core::Rpc & rpc, core::Annotation & annotation)
{
    core::CodecService codec_service{};
    auto entity = rpc.input()->find("entity");
    if(entity.empty()){
        BOOST_LOG_TRIVIAL(debug) << "Failed to get entity node";
        throw YDKInvalidArgumentException{"Failed to get entity node"};
    }

    core::DataNode* entity_node = entity[0];
    std::string entity_value = entity_node->get();

    //deserialize the entity_value
    core::DataNode* datanode = codec_service.decode(root_schema, entity_value, core::CodecService::Format::XML);

    if(!datanode){
        BOOST_LOG_TRIVIAL(debug) << "Failed to decode entity node";
        throw YDKInvalidArgumentException{"Failed to decode entity node"};
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
        BOOST_LOG_TRIVIAL(debug) << "Failed to get entity node.";
        throw YDKInvalidArgumentException{"Failed to get entity node"};
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
        BOOST_LOG_TRIVIAL(debug) << "Failed to create data tree";
        throw YDKIllegalStateException{"Failed to create data tree"};
    }

    std::string payload{"<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"};
    payload+=codec_service.encode(input, core::CodecService::Format::XML, false);
    payload+="</rpc>";
    BOOST_LOG_TRIVIAL(debug) <<"=============Generating payload=============";
    BOOST_LOG_TRIVIAL(debug) <<payload;
    BOOST_LOG_TRIVIAL(debug) <<"=========================="<<endl;
    return payload;
}

static core::DataNode* handle_edit_reply(string reply, NetconfClient & client, bool candidate_supported)
{
	if(reply.find("<ok/>") == std::string::npos)
	{
        BOOST_LOG_TRIVIAL(debug) << "No ok in reply " << reply;
		throw YDKServiceProviderException{reply};
	}

	if(candidate_supported)
	{
		//need to send the commit request
		reply = client.execute_payload(get_commit_rpc_payload());
		if(reply.find("<ok/>") == std::string::npos)
		{
			BOOST_LOG_TRIVIAL(debug) << "No ok in reply " << reply;
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
		BOOST_LOG_TRIVIAL(debug) << "Found empty data tag";
		return nullptr;
	}

	auto data_start = reply.find("<data>");
	if(data_start == std::string::npos)
	{
		BOOST_LOG_TRIVIAL(debug) << "Can't find data tag in reply " << reply;
		throw YDKServiceProviderException{reply};
	}
	data_start+= sizeof("<data>") - 1;
	auto data_end = reply.find("</data>", data_start);
	if(data_end == std::string::npos)
	{
		BOOST_LOG_TRIVIAL(debug) << "No end data tag found in reply " << reply;
		throw YDKException{"No end data tag found"};
	}

	string data = reply.substr(data_start, data_end-data_start);

	auto datanode = codec_service.decode(root_schema, data, core::CodecService::Format::XML);

	if(!datanode){
		BOOST_LOG_TRIVIAL(debug) << "Codec service failed to decode datanode";
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
	if(c.empty())
	{
		BOOST_LOG_TRIVIAL(error) << "CRUD create rpc schema not found!";
		throw YDKIllegalStateException{"CRUD create rpc schema not found!"};
	}
	return c[0];
}

//TODO fix and uncomment
/*
static std::string get_parameter(const std::string & capability, const std::string & parameter_name)
{
	auto module_start = capability.find(parameter_name + "=");
	if(module_start == std::string::npos)
		return "";
	auto module_end = capability.find("&", module_start);
	module_start+=sizeof(parameter_name+"=");
	auto size = module_end;
	if(size != std::string::npos ){
		size = module_end - module_start + 1;
	}

	std::string c_module = capability.substr( module_start - 1, size );
	return c_module;
}

static std::vector<std::string> get_parameter_list(const std::string & capability, const std::string & parameter_name)
{
	std::vector<std::string> c_features;
	auto features_start = capability.find(parameter_name+"=");
	if(features_start != std::string::npos){
		auto features_end = capability.find("&", features_start);
		features_start+=sizeof(parameter_name+"=");
		auto size=features_end;
		if(size!=std::string::npos){
			size = features_end - features_start + 1;
		}
		std::string features = capability.substr(features_start - 1 , size);
		std::istringstream iss{features};
		std::string feature;
		while(std::getline(iss, feature, ',')) {
			c_features.push_back(std::move(feature));
		}

	}
	return c_features;
}

static std::vector<ydk::core::Capability> get_core_capabilities(const std::vector<std::string> & server_capabilities)
{
	std::vector<ydk::core::Capability> capabilities;
	for(std::string c : server_capabilities)
	{
		if(c.find("calvados") != std::string::npos || c.find("tailf") != std::string::npos
				|| (c.find("Cisco-IOS-XR-ifmgr-oper") != std::string::npos
				|| c.find("Cisco-IOS-XR-bundlemgr-oper")!=std::string::npos
				|| c.find("Cisco-IOS-XR-mpls-te-oper")!=std::string::npos
				|| c.find("Cisco-IOS-XR-ip-tcp-oper")!=std::string::npos
				|| c.find("Cisco-IOS-XR-ip-udp-oper")!=std::string::npos))
		{
			continue;
		}

		auto p = std::find(c.begin(), c.end(),'?');

		if(p == c.end())
			continue;

		std::string c_module = get_parameter(c, "module");
		std::string c_revision = get_parameter(c, "revision");
		if(c_module.size()==0 || c_revision.size()==0)
			continue;

		std::vector<std::string> c_features = get_parameter_list(c, "features");
		std::vector<std::string> c_deviations = get_parameter_list(c, "deviations");

		ydk::core::Capability core_cap{c_module, c_revision, c_features, c_deviations};
		capabilities.emplace_back(core_cap);
	}
	//add ydk capability
	ydk::core::Capability ydk_cap{ydk::core::YDK_MODULE_NAME, ydk::core::YDK_MODULE_REVISION, {}, {}};
	auto result = std::find(capabilities.begin(), capabilities.end(), ydk_cap);
	if(result == capabilities.end()){
		capabilities.push_back(ydk_cap);
	}
	return capabilities;
}*/
}
