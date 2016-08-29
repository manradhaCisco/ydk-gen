#include "entity_data_node_walker.hpp"

#include <assert.h>
#include "core.hpp"
#include "entity.hpp"
#include <iostream>

using namespace std;

namespace ydk {

static void populate_data_node(Entity & entity, core::DataNode* data_node);
static EntityPath get_top_entity_path(Entity & entity);
static void walk_children(Entity & entity, core::DataNode* data_node);
static void populate_name_values(core::DataNode* parent_data_node, EntityPath & path);
static bool data_node_is_leaf(core::DataNode* data_node);
static string strip_keys(string path);

core::DataNode* get_data_node_from_entity(Entity & entity, const ydk::core::RootSchemaNode & root_schema)
{
	EntityPath root_path = get_top_entity_path(entity);
	auto root_data_node = root_schema.create(root_path.path);
	populate_name_values(root_data_node, root_path);
	walk_children(entity, root_data_node);
	return root_data_node;
}

void get_entity_from_data_node(core::DataNode * node, Entity* entity)
{
//	cerr<<endl;
//	cerr<<"Looking at "<<node->path()<<endl;
	if (entity == nullptr || node == nullptr)
		return;

	for(core::DataNode* child_data_node:node->children())
	{
		std::string path = strip_keys(child_data_node->path());
		if(data_node_is_leaf(child_data_node))
		{
//			cerr<<"Setting value: "<<child_data_node->get()<<" to: "<<path<<endl;
			entity->set_value(path, child_data_node->get());
		}
		else
		{
//			cerr<<"Setting child: "<<path<<endl;
			Entity * child_entity = entity->set_child(path);
			if(child_entity == nullptr)
			    cerr << "Couln't find child entity!"<<endl;
//			cerr<<endl;
			get_entity_from_data_node(child_data_node, child_entity);
		}
	}
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
	std::vector<Entity*> & children = entity.get_children();
	for(Entity* child:children)
	{
		if(child->has_data())
			populate_data_node(*child, data_node);
	}
}

static void populate_name_values(core::DataNode* data_node, EntityPath & path)
{
	for(const std::pair<std::string, std::string> & name_value : path.value_paths)
	{
		data_node->create(name_value.first, name_value.second);
	}
}

static EntityPath get_top_entity_path(Entity & entity)
{
	if (entity.parent == nullptr)
	{
		return std::move(entity.get_entity_path());
	}

	return get_top_entity_path(*entity.parent);
}

static bool data_node_is_leaf(core::DataNode* data_node)
{
	return (data_node->schema()->statement().keyword == "leaf");
}

static string strip_keys(string path)
{
	size_t begin = path.find_first_of("[");
	if (begin == std::string::npos)
		return path;

	size_t end = path.find_first_of("]");
	if (end == std::string::npos)
		return path;

	string stripped_path = path;
	stripped_path = path.replace(begin, end-begin+1, "");

	return strip_keys(stripped_path);
}
}
