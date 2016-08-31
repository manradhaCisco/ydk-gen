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

#include "crud_service.hpp"
#include "entity.hpp"
#include "core.hpp"
#include "entity_data_node_walker.hpp"
#include <iostream>

using namespace std;

namespace ydk {
static string get_data_payload(Entity & entity, core::RootSchemaNode & root_schema);
static core::DataNode* execute_rpc(core::ServiceProvider & provider, Entity & entity,
		const string & operation, const string & data_tag, bool set_config_flag=false);
static unique_ptr<Entity> get_top_entity_from_filter(Entity & filter);
static bool operation_succeeded(core::DataNode * node);

CrudService::CrudService()
{
}

bool CrudService::create(core::ServiceProvider & provider, Entity & entity)
{
	return operation_succeeded(
			execute_rpc(provider, entity, "ydk:create", "entity")
			);
}

bool CrudService::update(core::ServiceProvider & provider, Entity & entity)
{
	return operation_succeeded(
			execute_rpc(provider, entity, "ydk:update", "entity")
			);
}

bool CrudService::delete_(core::ServiceProvider & provider, Entity & entity)
{
	return operation_succeeded(
			execute_rpc(provider, entity, "ydk:delete", "entity")
			);
}

unique_ptr<Entity> CrudService::read(core::ServiceProvider & provider, Entity & filter)
{
	unique_ptr<Entity> top_entity = get_top_entity_from_filter(filter);
	core::DataNode* read_data_node = execute_rpc(provider, filter, "ydk:read", "filter");
	if (read_data_node == nullptr)
		return nullptr;

	get_entity_from_data_node(read_data_node->children()[0], top_entity.get());
    return top_entity;
}

unique_ptr<Entity> CrudService::read(core::ServiceProvider & provider, Entity & filter, bool config_only)
{
	unique_ptr<Entity> top_entity = get_top_entity_from_filter(filter);
	core::DataNode* read_data_node = execute_rpc(provider, filter, "ydk:read", "filter", config_only);
	if (read_data_node == nullptr)
		return nullptr;

	get_entity_from_data_node(read_data_node->children()[0], top_entity.get());
    return top_entity;
}

static bool operation_succeeded(core::DataNode * node)
{
	return node == nullptr;
}

static unique_ptr<Entity> get_top_entity_from_filter(Entity & filter)
{
	if(filter.parent == nullptr)
		return filter.clone_ptr();

	return get_top_entity_from_filter(*(filter.parent));
}

static core::DataNode* execute_rpc(core::ServiceProvider & provider, Entity & entity,
		const string & operation, const string & data_tag, bool set_config_flag)
{
	core::RootSchemaNode* root_schema = provider.get_root_schema();
	std::unique_ptr<ydk::core::Rpc> ydk_rpc { root_schema->rpc(operation) };
	string data = get_data_payload(entity, *root_schema);

	if(set_config_flag)
	{
		ydk_rpc->input()->create("only-config");
	}
	ydk_rpc->input()->create(data_tag, data);
	return (*ydk_rpc)(provider);
}

static string get_data_payload(Entity & entity, core::RootSchemaNode & root_schema)
{
	const ydk::core::DataNode* data_node = get_data_node_from_entity(entity, root_schema);
	if (data_node==nullptr)
		return "";
	core::CodecService codec{};
	return codec.encode(data_node, ydk::core::CodecService::Format::XML, true);
}

}
