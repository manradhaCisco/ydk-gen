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

//#include <boost/python.hpp>
//#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <libnetconf.h>
#include <libnetconf_ssh.h>

#include "entity.hpp"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "entity.hpp"
#include "netconf_client.hpp"
#include "netconf_provider.hpp"
#include "core.hpp"
#include "entity_data_node_walker.hpp"
#include <cstring>
#include <iostream>
#include <string>
#include <algorithm>
#include <sstream>

using namespace std;
using namespace ydk;

namespace ydk
{
static string get_commit_rpc_payload();
static unique_ptr<core::Rpc> create_rpc_instance(core::RootSchemaNode & root_schema, string rpc_name);
static core::DataNode* create_rpc_input(core::Rpc & netconf_rpc);

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

    auto c = root_schema->find("ydk:create");
    if(c.empty()){
        throw core::YDKIllegalStateException{"CRUD create rpc schema not found!"};
    }
    create_schema = c[0];

    c = root_schema->find("ydk:read");
    if(c.empty()){
        throw core::YDKIllegalStateException{"CRUD read rpc schema not found!"};
    }
    read_schema = c[0];

    c = root_schema->find("ydk:update");
    if(c.empty()){
        throw core::YDKIllegalStateException{"CRUD update rpc schema not found!"};

    }
    update_schema = c[0];

    c = root_schema->find("ydk:delete");
    if(c.empty()){
        throw core::YDKIllegalStateException{"CRUD delete rpc schema not found!"};
    }
    delete_schema = c[0];

}

NetconfServiceProvider::~NetconfServiceProvider()
{
	client->close();
}

core::RootSchemaNode* NetconfServiceProvider::get_root_schema()
{
    return root_schema.get();
}

core::DataNode* NetconfServiceProvider::handle_read(core::Rpc* rpc) const
{
    //for now we only support crud rpc's
    core::DataNode* datanode = nullptr;
    core::CodecService codec_service{};
    bool config = true;

    if(!rpc->input()->find("config").empty()){
        config = true;
    }

    std::string rpc_name = "ietf-netconf:get";
    if(config)
    {
        rpc_name = "ietf-netconf:get-config";
    }

    auto netconf_rpc = create_rpc_instance(*root_schema, rpc_name);
    auto input = create_rpc_input(*netconf_rpc);

    if(config && !input->create("source/running", "")){
        throw core::YDKIllegalStateException{"Failed setting source"};
    }

    auto entity = rpc->input()->find("filter");
    if(entity.empty()){
        throw core::YDKInvalidArgumentException{"Failed to get entity node"};
    }

    datanode = entity[0];

    std::string filter_value = datanode->get();
    //deserialize the entity_val

    auto filter_node = input->create("filter", filter_value);
    if(!filter_node)
    {
        throw core::YDKIllegalStateException{"Failed to set filter"};
    }

    std::string payload{"<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"};
    payload+=codec_service.encode(input, core::CodecService::Format::XML, false);
    payload+="</rpc>";

    std::cout << payload << std::endl;

    std::string reply = client->execute_payload(payload);
    std::cout << reply << std::endl;

    auto empty_data = reply.find("<data/>");
	if(empty_data != std::string::npos)
	{
		return nullptr;
	}

    auto data_start = reply.find("<data>");
    if(data_start == std::string::npos)
    {
        //TODO at a later stage this should be changed to
        //another exception hierarchy
        throw core::YDKException{reply};
    }
    data_start+= sizeof("<data>") - 1;
    auto data_end = reply.find("</data>", data_start);
    if(data_end == std::string::npos)
    {
        throw core::YDKException{"No end data tag found"};
    }

    payload = reply.substr(data_start, data_end-data_start);

    datanode = codec_service.decode(root_schema.get(), payload, core::CodecService::Format::XML);

    if(!datanode){
        throw core::YDKException{"Problems deserializing output"};
    }

    return datanode;
}


core::DataNode* NetconfServiceProvider::handle_edit(core::Rpc* rpc, core::Annotation ann) const
{
    //for now we only support crud rpc's
    core::DataNode* datanode = nullptr;
    core::CodecService codec_service{};
    bool candidate_supported = false;

    auto netconf_rpc = create_rpc_instance(*root_schema, "ietf-netconf:edit-config");
    auto input = create_rpc_input(*netconf_rpc);

    if(std::find(client_capabilities.begin(), client_capabilities.end(), NetconfServiceProvider::CANDIDATE) != client_capabilities.end()){
        //candidate is supported
        candidate_supported = true;
    }

    if(candidate_supported){
        if(!input->create("target/candidate", "")){
            throw core::YDKIllegalStateException{"Failed setting target datastore"};
        }
    } else {
        if(!input->create("target/running", "")){
            throw core::YDKIllegalStateException{"Failed setting running datastore"};
        }
    }

    if(!input->create("error-option", "rollback-on-error")){
        throw core::YDKIllegalStateException{"Failed to set rollback-on-error option"};
    }

    auto entity = rpc->input()->find("entity");
    if(entity.empty()){
        throw core::YDKInvalidArgumentException{"Failed to get entity node"};
    }

    core::DataNode* entity_node = entity[0];

    std::string entity_value = entity_node->get();
    //deserialize the entity_value

    datanode = codec_service.decode(root_schema.get(), entity_value, core::CodecService::Format::XML);

    if(!datanode){
        throw core::YDKInvalidArgumentException{"Failed to deserialize entity node"};
    }

    std::string config_payload {};

    for(auto child : datanode->children())
    {
        child->add_annotation(ann);
        config_payload += codec_service.encode(child, core::CodecService::Format::XML, false);
    }

    auto config_node = input->create("config", config_payload);
    if(!config_node)
    {
        throw core::YDKIllegalStateException{"Failed to create config tree"};
    }

    std::string payload{"<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"};
    payload+=codec_service.encode(input, core::CodecService::Format::XML, false);
    payload+="</rpc>";

    std::cout << payload << std::endl;

    std::string reply = client->execute_payload(payload);
    std::cout << reply << std::endl;
    if(reply.find("<ok/>") == std::string::npos)
    {
        //TODO at a later stage this should be changed to
        //another exception hierarchy
        throw core::YDKException{reply};
    }

    if(candidate_supported)
    {
        //need to send the commit request
        reply = client->execute_payload(get_commit_rpc_payload());
        if(reply.find("<ok/>") == std::string::npos)
        {
            //TODO at a later stage this should be changed to
            //another exception hierarchy
            throw core::YDKException{reply};
        }
    };

    //no error no output for edit-config
    return nullptr;
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
    core::CodecService codec_service{};

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
}
