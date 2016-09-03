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
    
    
ydk::EntityDiagnostic 
ydk::ValidationService::validate(const ydk::core::ServiceProvider& sp, ydk::Entity& entity, ydk::ValidationService::Option option)
{
   
    EntityPath root_path = entity.get_entity_path(entity.parent);
    
    //validation checking
    //first check if the schema node that represents this path
    //actually exists and then throw an error
    
    auto root_sn = sp.get_root_schema();
    auto sn_vec = root_sn->find(root_path.path);
    
    EntityDiagnostic diagnostic{};
    
    if(sn_vec.empty()) {
        diagnostic.errors.push_back(ydk::core::ValidationError::SCHEMA_NOT_FOUND);
        //no point processing children
        return diagnostic;
        
    } else {
        
        //schema node cannot be leaf, leaf-list or anyxml
        auto sn = sn_vec[0];
        auto stmt = sn->statement();
        if(stmt.keyword == "leaf" || stmt.keyword == "leaf-list" || stmt.keyword == "anyxml") {
            diagnostic.errors.push_back(ydk::core::ValidationError::INVALID_USE_OF_SCHEMA);
            
        } else {
            // there is no error with the schema for this node
            // first check if is a list
            
            if(option == ValidationService::Option::EDIT_CONFIG || option == ValidationService::Option::DATASTORE) {
            
                if(stmt.keyword == "list") {
                    auto keys = sn->keys();
                    if(!keys.empty()) {
                        //create a map of the keys
                        std::map<std::string, std::string> name_value_map{};
                        for(auto value_path : root_path.value_paths) {
                            name_value_map.insert(value_path);
                        }
                    
                        //we need a key
                        for(auto key : sn->keys()) {
                            //check if the value list has the value's
                            if(name_value_map.find(key.arg) == name_value_map.end()) {
                                ydk::core::DiagnosticNode<std::string, ydk::core::ValidationError> attr{};
                                attr.source = key.arg;
                                attr.errors.push_back(ydk::core::ValidationError::MISSELEM);
                                diagnostic.attrs.push_back(std::move(attr));
                            }
                        }
                    }
                }
                
                //now validate the values in the value_path that have some values in them
                for(auto value_path : root_path.value_paths) {
                    if(value_path.second.empty())
                        continue;
                    auto leaf_sn_vec = sn->find(value_path.first);
                    if(leaf_sn_vec.empty()) {
                        //the leaf is invalid or not present in the schema
                        ydk::core::DiagnosticNode<std::string, ydk::core::ValidationError> attr{};
                        attr.source = value_path.first;
                        attr.errors.push_back(ydk::core::ValidationError::SCHEMA_NOT_FOUND);
                        diagnostic.attrs.push_back(std::move(attr));
                    } else {
                        ydk::core::SchemaNode* sn = leaf_sn_vec[0];
                        //now test to see if the value is correct
                        ydk::core::SchemaValueType* type = sn->type();
                        
                        if(type == nullptr) {
                            //TODO log this
                            throw YDKIllegalStateException{"Cannot derive type for "};
                        } else {
                            ydk::core::DiagnosticNode<std::string, ydk::core::ValidationError> attr = type->validate(value_path.second);
                            if(attr.has_errors()){
                                attr.source = value_path.first;
                                diagnostic.attrs.push_back(std::move(attr));
                            }
                            
                        }
                        
                    }
                }
            }
        }
    }
    
    for(auto e : entity.get_children()){
        EntityDiagnostic child_diag = validate(sp, *e, option);
        diagnostic.children.push_back(child_diag);
    }
    
    
    return diagnostic;
    
}

}
