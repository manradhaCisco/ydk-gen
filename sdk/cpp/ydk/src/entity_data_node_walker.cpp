#include "entity_data_node_walker.hpp"

#include <iostream>
#include <assert.h>

#include "core.hpp"
#include "types.hpp"

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
	EntityPath path = entity.get_entity_path(entity.parent);
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
		return std::move(entity.get_entity_path(nullptr));
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

static void
validate_missing_keys(const EntityPath& entity_path,
                      const ydk::core::SchemaNode& schema_node,
                      EntityDiagnostic& diagnostic)
{
    auto keys = schema_node.keys();
    //create a map of the keys
    std::map<std::string, std::string> name_value_map{};
    for(auto value_path : entity_path.value_paths) {
        name_value_map.insert(value_path);
    }
        
    //we need a key
    for(auto key : keys) {
        //check if the value list has the value's
        if(name_value_map.find(key.arg) == name_value_map.end()) {
            ydk::core::DiagnosticNode<std::string, ydk::core::ValidationError> attr{};
            attr.source = key.arg;
            attr.errors.push_back(ydk::core::ValidationError::MISSELEM);
            diagnostic.attrs.push_back(std::move(attr));
        }
    }
    
}

static void
validate_attributes(const EntityPath& entity_path, const ydk::core::SchemaNode& schema_node,
                    EntityDiagnostic& diagnostic)
{
    //now validate the values in the value_path that have some values in them
    for(auto value_path : entity_path.value_paths) {
        if(value_path.second.empty())
            continue;
        auto leaf_schema_node_list = schema_node.find(value_path.first);
        if(leaf_schema_node_list.empty()) {
            //the leaf is invalid or not present in the schema
            ydk::core::DiagnosticNode<std::string, ydk::core::ValidationError> attr{};
            attr.source = value_path.first;
            attr.errors.push_back(ydk::core::ValidationError::SCHEMA_NOT_FOUND);
            diagnostic.attrs.push_back(std::move(attr));
        } else {
            ydk::core::SchemaNode* leaf_schema_node = leaf_schema_node_list[0];
            //now test to see if the value is correct
            ydk::core::SchemaValueType* type = leaf_schema_node->type();
            
            if(type == nullptr) {
                //TODO log this
                throw YDKIllegalStateException{"Cannot derive type for "};
            } else {
                auto attr = type->validate(value_path.second);
                if(attr.has_errors()){
                    attr.source = value_path.first;
                    diagnostic.attrs.push_back(std::move(attr));
                }
                
            }
            
        }
    }
    
}

static bool
keyword_is_leaf(std::string & keyword)
{
    if(keyword == "leaf" || keyword == "leaf-list" || keyword == "anyxml")
        return true;
        
    return false;
}
    
static EntityDiagnostic
validate(const ydk::core::ServiceProvider& sp, ydk::Entity& entity, ydk::Entity* parent,
             ydk::ValidationService::Option option)
{
   
    EntityPath entity_path = entity.get_entity_path(parent);
    
    //validation checking
    //first check if the schema node that represents this path
    //actually exists and then throw an error
    
    auto root_schema_node = sp.get_root_schema();
    auto schema_node_list = root_schema_node->find(entity_path.path);
    
    EntityDiagnostic diagnostic{};
    diagnostic.source = &entity;
    
    if(schema_node_list.empty()) {
        diagnostic.errors.push_back(ydk::core::ValidationError::SCHEMA_NOT_FOUND);
        //no point processing children
        return diagnostic;
        
    }
        
    //schema node cannot be leaf, leaf-list or anyxml
    ydk::core::SchemaNode* schema_node = schema_node_list[0];
    auto stmt = schema_node->statement();
    if(keyword_is_leaf(stmt.keyword)) {
        diagnostic.errors.push_back(ydk::core::ValidationError::INVALID_USE_OF_SCHEMA);
            
    } else {
        // there is no error with the schema for this node
        // first check if is a list
            
        if(option == ValidationService::Option::EDIT_CONFIG || option == ValidationService::Option::DATASTORE) {
            
            if(stmt.keyword == "list") {
                auto keys = schema_node->keys();
                if(!keys.empty()) {
                    validate_missing_keys(entity_path, *schema_node, diagnostic);
                }
            }
            ydk::validate_attributes(entity_path, *schema_node, diagnostic);
            
        }
    }
    
    for(auto child_entity : entity.get_children()){
        EntityDiagnostic child_diagnostic = validate(sp, *child_entity, &entity, option);
        diagnostic.children.push_back(child_diagnostic);
    }
    
    return diagnostic;
    
}
    
EntityDiagnostic
ValidationService::validate(const core::ServiceProvider& sp, Entity& entity,
                            ValidationService::Option option)
{
        
    return ydk::validate(sp, entity, entity.parent, option);
        
}

}
