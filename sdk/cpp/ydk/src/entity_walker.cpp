#include <assert.h>
#include "core.hpp"
#include "entity.hpp"
#include "entity_walker.hpp"
#include <iostream>

namespace ydk {

static void populate_data_node(Entity & entity, core::DataNode* data_node);
static Entity & get_top_entity(Entity & entity);
static void walk_children(Entity & entity, core::DataNode* data_node);
static void populate_name_values(core::DataNode* parent_data_node, EntityPath & path);

core::DataNode* get_data_node(Entity & entity, ydk::core::RootSchemaNode & root_schema)
{
	EntityPath root_path = get_top_entity(entity).get_entity_path();
	auto root_data_node = root_schema.create(root_path.path);
	populate_name_values(root_data_node, root_path);
	walk_children(entity, root_data_node);
	return root_data_node;
}

static void populate_data_node(Entity & entity, core::DataNode* parent_data_node)
{
	EntityPath path = entity.get_entity_path();
	auto data_node = parent_data_node->create(path.path);
	populate_name_values(data_node, path);
	walk_children(entity, data_node);
}

static void walk_children(Entity & entity, core::DataNode* data_node)
{
	for(Entity* child:entity.get_children())
	{
		if(child->has_data())
			populate_data_node(*child, data_node);
	}
}

static void populate_name_values(core::DataNode* data_node, EntityPath & path)
{
	for(std::pair<std::string, std::string> & name_value : path.value_paths)
	{
		data_node->create(name_value.first, name_value.second);
	}
}

static Entity & get_top_entity(Entity & entity)
{
	if (entity.parent == nullptr)
	{
		return entity;
	}

	return get_top_entity(*entity.parent);
}
}
