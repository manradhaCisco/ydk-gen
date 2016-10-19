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


///////////////////////////////////////////////////////////////////////////////
/// SchemaNode
///////////////////////////////////////////////////////////////////////////////
ydk::core::SchemaNode::~SchemaNode()
{

}


/////////////////////////////////////////////////////////////////////
// ydk::SchemaNodeImpl
////////////////////////////////////////////////////////////////////
ydk::core::SchemaNodeImpl::SchemaNodeImpl(const SchemaNode* parent, struct lys_node* node):m_parent{parent}, m_node{node}, m_children{}, m_type{nullptr}
{
    node->priv = this;
    if(node->nodetype != LYS_LEAF && node->nodetype != LYS_LEAFLIST) {

        const struct lys_node *last = nullptr;

        while( auto q = lys_getnext(last, node, nullptr, 0)) {
            m_children.emplace_back(new SchemaNodeImpl{this,const_cast<struct lys_node*>(q)});
            last = q;
        }
    } else {
        struct lys_node_leaf* node_leaf = reinterpret_cast<struct lys_node_leaf*>(m_node);
        m_type = ydk::core::create_schema_value_type(node_leaf);

    }

}

ydk::core::SchemaNodeImpl::~SchemaNodeImpl()
{
    //delete all the children
    for(auto p : children()) {
	delete p;
    }

    if(m_type){
        delete m_type;
    }
    m_type = nullptr;
}

std::string
ydk::core::SchemaNodeImpl::path() const
{
    std::string ret{};

    std::vector<std::string> segments;

    struct lys_node* cur_node = m_node;
    struct lys_module* module = nullptr;

    while(cur_node != nullptr){
	module = cur_node->module;
	if (!cur_node->parent || cur_node->parent->module != module) {
		//qualify with module name

		std::string segname {module->name};
		segname+=':';
		segname+=cur_node->name;
		segments.push_back(segname);
	} else {
		segments.push_back(cur_node->name);
	}
	cur_node = cur_node->parent;
    }

    std::reverse(segments.begin(), segments.end());
    for ( auto seg : segments ) {
	ret+="/";
	ret+=seg;

    }

    return ret;
}

std::vector<ydk::core::SchemaNode*>
ydk::core::SchemaNodeImpl::find(const std::string& path) const
{
    if(path.empty()) {
        BOOST_LOG_TRIVIAL(debug) << "Path is empty";
	throw YDKInvalidArgumentException{"path is empty"};
    }

    //has to be a relative path
    if(path.at(0) == '/') {
        BOOST_LOG_TRIVIAL(debug) << "path must be a relative path";
	throw YDKInvalidArgumentException{"path must be a relative path"};
    }

    std::vector<SchemaNode*> ret;
    struct ly_ctx* ctx = m_node->module->ctx;

    const struct lys_node* found_node = ly_ctx_get_node(ctx, m_node, path.c_str());

    if (found_node){
	auto p = reinterpret_cast<SchemaNode*>(found_node->priv);
	if(p) {
            ret.push_back(p);
	}
    }

    return ret;
}

const ydk::core::SchemaNode*
ydk::core::SchemaNodeImpl::parent() const noexcept
{
    return m_parent;
}

std::vector<ydk::core::SchemaNode*>
ydk::core::SchemaNodeImpl::children() const
{

    return m_children;
}

const ydk::core::SchemaNode*
ydk::core::SchemaNodeImpl::root() const noexcept
{
    if(m_parent == nullptr){
	return this;
    } else {
	return m_parent->root();
    }
}

ydk::core::Statement
ydk::core::SchemaNodeImpl::statement() const
{
    Statement s{};
    s.arg = m_node->name;

    switch(m_node->nodetype) {
    case LYS_CONTAINER:
        s.keyword = "container";
        break;
    case LYS_CHOICE:
	s.keyword = "choice";
	break;
    case LYS_LEAF:
	s.keyword = "leaf";
	break;
    case LYS_LEAFLIST:
	s.keyword = "leaf-list";
	break;
    case LYS_LIST:
        s.keyword = "list";
        break;
    case LYS_CASE:
        s.keyword = "case";
        break;
    case LYS_NOTIF:
        s.keyword = "notification";
        break;
    case LYS_RPC:
        s.keyword = "rpc";
        break;
    case LYS_INPUT:
        s.keyword = "input";
        break;
    case LYS_OUTPUT:
        s.keyword = "output";
        break;
    case LYS_GROUPING:
        s.keyword = "grouping";
        break;
    case LYS_USES:
        s.keyword = "uses";
        break;
    case LYS_AUGMENT:
        s.keyword = "augment";
        break;
    case LYS_ANYXML:
        s.keyword = "anyxml";
        break;
    case LYS_UNKNOWN:
        break;
    }
    return s;
}

///
/// @brief return YANG statement corresponding the the keys
///
/// Returns the vector of Statement keys
/// @return vector of Statement that represent keys
///
std::vector<ydk::core::Statement>
ydk::core::SchemaNodeImpl::keys() const
{
    std::vector<Statement> stmts{};

    Statement stmt = statement();
    if(stmt.keyword == "list") {
        //sanity check
        if(m_node->nodetype != LYS_LIST) {
            BOOST_LOG_TRIVIAL(debug) << "Mismatch in schema";
            throw YDKIllegalStateException{"Mismatch in schema"};
        }
        struct lys_node_list *slist = (struct lys_node_list *)m_node;
        for(uint8_t i=0; i < slist->keys_size; ++i) {
            SchemaNode* sn = reinterpret_cast<SchemaNode*>(slist->keys[i]->priv);
            if(sn != nullptr){
                stmts.push_back(sn->statement());
            }
        }
    }

    return stmts;
}



ydk::core::SchemaValueType*
ydk::core::SchemaNodeImpl::type() const
{

    return m_type;
}


