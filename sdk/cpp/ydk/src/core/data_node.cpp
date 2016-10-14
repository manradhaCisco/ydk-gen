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


#include "core_private.hpp"
#include <boost/log/trivial.hpp>


////////////////////////////////////////////////////////////////////////
/// DataNode
////////////////////////////////////////////////////////////////////////

ydk::core::DataNode::~DataNode()
{

}

ydk::core::DataNode*
ydk::core::DataNode::create(const std::string& path)
{
    return create(path, "");
}


////////////////////////////////////////////////////////////////////////////
// class ydk::DataNodeImpl
//////////////////////////////////////////////////////////////////////////
ydk::core::DataNodeImpl::DataNodeImpl(DataNode* parent, struct lyd_node* node): m_parent{parent}, m_node{node}
{
	//add the children
    if(m_node && m_node->child && !(m_node->schema->nodetype == LYS_LEAF ||
                          m_node->schema->nodetype == LYS_LEAFLIST ||
                          m_node->schema->nodetype == LYS_ANYXML)){
        struct lyd_node *iter = nullptr;
        LY_TREE_FOR(m_node->child, iter) {
            DataNodeImpl* dn = new DataNodeImpl{this, iter};
            child_map.insert(std::make_pair(iter, dn));
        }
    }

}

ydk::core::DataNodeImpl::~DataNodeImpl()
{
    //first destroy the children
    for (auto p : child_map) {
        delete p.second;
    }

    if(m_node){
        if(m_parent) {
            lyd_free(m_node);
        } else {
            lyd_free_withsiblings(m_node);
        }

        m_node = nullptr;
    }
}

const ydk::core::SchemaNode*
ydk::core::DataNodeImpl::schema() const
{
    return reinterpret_cast<const SchemaNode*>(m_node->schema->priv);
}

std::string
ydk::core::DataNodeImpl::path() const
{
    char* path = lyd_path(m_node);
    if (!path) {
        return std::string{};
    }
    std::string str{path};
    std::free(path);
    return str;
}

ydk::core::DataNode*
ydk::core::DataNodeImpl::create(const std::string& path, const std::string& value)
{
    if(path.empty()){
        BOOST_LOG_TRIVIAL(debug) << "Path is empty.";
        throw YDKInvalidArgumentException{"Path is empty."};
    }

    std::vector<std::string> segments = segmentalize(path);

    DataNodeImpl* dn = this;

    size_t start_index = 0;
    auto iter = segments.begin();

    while (iter != segments.end()) {
        auto r = dn->find(*iter);
        if(r.empty()){
            break;
        } else if(r.size() != 1){
            BOOST_LOG_TRIVIAL(debug) << "Path " << path << " is ambiguous";
            throw YDKPathException{YDKPathException::Error::PATH_AMBIGUOUS};
        } else {
            dn = dynamic_cast<DataNodeImpl*>(r[0]);
            if (dn == nullptr) {
                BOOST_LOG_TRIVIAL(debug) << "Invalid data node";
                throw YDKCoreException{"Invalid data node"};
	    }
	    ++iter;
            start_index++;
        }
    }

    if (segments.empty()) {
        BOOST_LOG_TRIVIAL(debug) << "path " << path << " points to existing node";
	throw YDKInvalidArgumentException{"path points to existing node."};
    }

    std::vector<struct lyd_node*> nodes_created;
    struct lyd_node* first_node_created = nullptr;
    struct lyd_node* cn = dn->m_node;

    for(size_t i=start_index; i< segments.size(); i++){
        if (i != segments.size() - 1) {
            cn = lyd_new_path(cn, nullptr, segments[i].c_str(), nullptr, 0);
	} else {
            cn = lyd_new_path(cn, nullptr, segments[i].c_str(), value.c_str(), 0);
	}

	if (cn == nullptr) {
            if(first_node_created) {
		lyd_unlink(first_node_created);
		lyd_free(first_node_created);
            }
            throw YDKInvalidArgumentException{"invalid path"};
        } else if (!first_node_created) {
            first_node_created = cn;
        }
    }

    if (first_node_created) {
        auto p = new DataNodeImpl{dn, first_node_created};
        dn->child_map.insert(std::make_pair(first_node_created, p));

        DataNodeImpl* rdn = p;

        while(!rdn->children().empty() && rdn->m_node != cn){
            rdn = dynamic_cast<DataNodeImpl*>(rdn->children()[0]);
        }

        return rdn;
    } else {
        return dn;
    }
}

void
ydk::core::DataNodeImpl::set(const std::string& value)
{
    //set depends on the kind of the node
    struct lys_node* s_node = m_node->schema;

    if (s_node->nodetype == LYS_LEAF || s_node->nodetype == LYS_LEAFLIST) {
        struct lyd_node_leaf_list* leaf= reinterpret_cast<struct lyd_node_leaf_list *>(m_node);
        if(lyd_change_leaf(leaf, value.c_str())) {
            BOOST_LOG_TRIVIAL(debug) << "Invalid value " << value;
            throw YDKInvalidArgumentException{"Invalid value"};
        }
    } else if (s_node->nodetype == LYS_ANYXML) {
        struct lyd_node_anyxml* anyxml = reinterpret_cast<struct lyd_node_anyxml *>(m_node);
        anyxml->xml_struct = 0;
        anyxml->value.str = value.c_str();
    }else {
        BOOST_LOG_TRIVIAL(debug) << "Trying to set value " << value << " for a non leaf non anyxml node.";
        throw YDKInvalidArgumentException{"Cannot set value for this Data Node"};
    }
}

std::string
ydk::core::DataNodeImpl::get() const
{
    struct lys_node* s_node = m_node->schema;
    std::string ret {};
    if (s_node->nodetype == LYS_LEAF || s_node->nodetype == LYS_LEAFLIST) {
        struct lyd_node_leaf_list* leaf= reinterpret_cast<struct lyd_node_leaf_list *>(m_node);
        return leaf->value_str;
    } else if (s_node->nodetype == LYS_ANYXML ){
        struct lyd_node_anyxml* anyxml = reinterpret_cast<struct lyd_node_anyxml *>(m_node);
        if(!anyxml->xml_struct){
            return anyxml->value.str;
        }
    }
    return ret;
}

std::vector<ydk::core::DataNode*>
ydk::core::DataNodeImpl::find(const std::string& path) const
{
    std::vector<DataNode*> results;

    if(m_node == nullptr) {
        return results;
    }
    std::string spath{path};

    auto s = schema()->statement();
    if(s.keyword == "rpc"){
        spath="input/" + spath;
    }
    const struct lys_node* found_snode =
        ly_ctx_get_node(m_node->schema->module->ctx, m_node->schema, spath.c_str());

    if(found_snode) {
        struct ly_set* result_set = lyd_get_node(m_node, path.c_str());
        if( result_set ){
            if (result_set->number > 0){
                for(size_t i=0; i < result_set->number; i++){
                    struct lyd_node* node_result = result_set->set.d[i];
                    results.push_back(get_dn_for_desc_node(node_result));
                }
            }
            ly_set_free(result_set);
        }

    }

    return results;
}

ydk::core::DataNode*
ydk::core::DataNodeImpl::parent() const
{
    return m_parent;
}

std::vector<ydk::core::DataNode*>
ydk::core::DataNodeImpl::children() const
{
    std::vector<DataNode*> ret{};
    //the ordering should be determined by the lyd_node
    struct lyd_node *iter;
    if(m_node && m_node->child && !(m_node->schema->nodetype == LYS_LEAF ||
                          m_node->schema->nodetype == LYS_LEAFLIST ||
                          m_node->schema->nodetype == LYS_ANYXML)){
        LY_TREE_FOR(m_node->child, iter){
            auto p = child_map.find(iter);
            if (p != child_map.end()) {
                ret.push_back(p->second);
            }

        }
    }

    return ret;
}

const ydk::core::DataNode*
ydk::core::DataNodeImpl::root() const
{
    if(m_parent){
        return m_parent->root();
    }
    return this;
}

std::string
ydk::core::DataNodeImpl::xml() const
{
	std::string ret;
	char* xml = nullptr;
	if(!lyd_print_mem(&xml, m_node,LYD_XML, LYP_FORMAT)) {
		ret = xml;
		std::free(xml);
	}
	return ret;
}

ydk::core::DataNodeImpl*
ydk::core::DataNodeImpl::get_dn_for_desc_node(struct lyd_node* desc_node) const
{
	DataNodeImpl* dn = nullptr;

	//create DataNode wrappers
	std::vector<struct lyd_node*> nodes{};
	struct lyd_node* node = desc_node;

	while (node != nullptr && node != m_node) {
		nodes.push_back(node);
		node= node->parent;
	}

	//reverse
	std::reverse(nodes.begin(), nodes.end());

	const DataNodeImpl* parent = this;

        if(nodes[0] == m_node){
            nodes.erase(nodes.begin());
        }

	for( auto p : nodes) {
            auto res = parent->child_map.find(p);

	   if(res != parent->child_map.end()) {
		   //DataNode is already present
		   dn = res->second;

	   } else {
               if(!m_node->parent) {
                   //special case the root is the first node
                   parent = child_map.begin()->second;
                   res = parent->child_map.find(p);

                   if(res != parent->child_map.end()){
                       dn = res->second;
                   } else {
                       BOOST_LOG_TRIVIAL(error) << "Cannot find child DataNode";
                       throw YDKCoreException{"Cannot find child!"};
                   }
               } else {
                   BOOST_LOG_TRIVIAL(error) << "Parent is nullptr";
                   throw YDKCoreException{"Parent is nullptr"};
               }
           }
	   parent = dn;
	}

	return dn;
}


void
ydk::core::DataNodeImpl::add_annotation(const ydk::core::Annotation& an)
{

    if(!m_node) {
        BOOST_LOG_TRIVIAL(error) << "Cannot annotate uninitialized node";
        throw YDKIllegalStateException{"Cannot annotate node"};
    }

    std::string name { an.m_ns + ":" + an.m_name };

    struct lyd_attr* attr = lyd_insert_attr(m_node, nullptr, name.c_str(), an.m_val.c_str());

    if(attr == nullptr) {
        BOOST_LOG_TRIVIAL(debug) << "Cannot find module " << name.c_str();
        throw YDKInvalidArgumentException("Cannot find module with given namespace.");
    }
}


bool
ydk::core::DataNodeImpl::remove_annotation(const ydk::core::Annotation& an)
{
    if(!m_node) {
        return false;
    }

    struct lyd_attr* attr = m_node->attr;
    while(attr){
        struct lys_module *module = attr->module;
        if(module){
            Annotation an1{module->ns, attr->name, attr->value};
            if (an == an1){
                lyd_free_attr(m_node->schema->module->ctx, m_node, attr, 0);
                return true;
            }
        }
    }

    return false;
}

std::vector<ydk::core::Annotation>
ydk::core::DataNodeImpl::annotations()
{
    std::vector<ydk::core::Annotation> ann {};

    if(m_node) {
        struct lyd_attr* attr = m_node->attr;
        while(attr) {
            struct lys_module *module = attr->module;
            if(module) {
                ann.emplace_back(module->ns, attr->name, attr->value);

            }
            attr = attr->next;
        }
    }


    return ann;
}

