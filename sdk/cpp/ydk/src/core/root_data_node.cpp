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
// class ydk::RootDataImpl
//////////////////////////////////////////////////////////////////////////
ydk::core::RootDataImpl::RootDataImpl(const SchemaNode* schema, struct ly_ctx* ctx, const std::string path) : DataNodeImpl{nullptr, nullptr}, m_schema{schema}, m_ctx{ctx}, m_path{path}
{

}

ydk::core::RootDataImpl::~RootDataImpl()
{
    m_node = nullptr;
}

const ydk::core::SchemaNode*
ydk::core::RootDataImpl::schema() const
{
    return m_schema;
}

std::string
ydk::core::RootDataImpl::path() const
{
    return m_schema->path();
}

ydk::core::DataNode*
ydk::core::RootDataImpl::create(const std::string& path, const std::string& value)
{
    if(path.empty()){
        BOOST_LOG_TRIVIAL(debug) << "Path is empty";
        throw YDKInvalidArgumentException{"Path is empty"};

    }

    //path should not start with /
    if(path.at(0) == '/'){
        BOOST_LOG_TRIVIAL(debug) << "Path " << path << " starts with /";
        throw YDKInvalidArgumentException{"Path starts with /"};
    }
    std::vector<std::string> segments = segmentalize(path);

    std::string start_seg = m_path + segments[0];
    struct lyd_node* dnode = lyd_new_path(m_node, m_ctx, start_seg.c_str(),
                                          segments.size() == 1 ? value.c_str():nullptr,0);

    if( dnode == nullptr){
        BOOST_LOG_TRIVIAL(debug) << "Path " << path << " is invalid";
        throw YDKInvalidArgumentException{"Path is invalid."};
    }

    DataNodeImpl* dn = nullptr;
    if(m_node == nullptr){
        m_node = dnode;
        dn = new DataNodeImpl{this, m_node};
        child_map.insert(std::make_pair(m_node, dn));
    } else {
        //dnode is one of the siblings of m_node
        auto iter = child_map.find(dnode);
        if(iter != child_map.end()) {
            dn = iter->second;

        } else {
            dn = new DataNodeImpl{this, m_node};
            child_map.insert(std::make_pair(m_node, dn));
        }

    }

    DataNode* rdn = dn;
    // created data node is the last child
    while(!rdn->children().empty()) {
        rdn = rdn->children()[0];
    }

    //at this stage we have dn so for the remaining segments use dn as the parent
    if(segments.size() > 1) {
        std::string remaining_path;
        for(size_t i =1; i< segments.size(); i++) {
            if(i!=1){
                remaining_path+="/";
            }
            remaining_path+=segments[i];
        }

        rdn = rdn->create(remaining_path);
    }


    return rdn;

 }

void
ydk::core::RootDataImpl::set(const std::string& value)
{
    if(!value.empty()) {
        BOOST_LOG_TRIVIAL(debug) << "Invalid value being assigned to root";
        throw YDKInvalidArgumentException{"Invalid value being assigned to root."};
    }
}

std::string
ydk::core::RootDataImpl::get() const
{
    return "";
}


std::vector<ydk::core::DataNode*>
ydk::core::RootDataImpl::children() const
{
    std::vector<DataNode*> ret{};

    struct lyd_node* iter = m_node;

    if( iter ){
        do {
            auto p = child_map.find(iter);
            if (p != child_map.end()) {
                ret.push_back(p->second);
            }

            iter=iter->next;

        } while (iter && iter != m_node);
    }

    return ret;
}

const ydk::core::DataNode*
ydk::core::RootDataImpl::root() const
{
    return this;
}

std::vector<ydk::core::DataNode*>
ydk::core::RootDataImpl::find(const std::string& path) const
{
    std::vector<DataNode*> results;

    if(m_node == nullptr) {
        return results;
    }

    std::string schema_path{ this->path() };
    if(schema_path.size()!= 1){
        schema_path+="/";
    }

    auto s = schema()->statement();
    if(s.keyword == "rpc") {
        schema_path+="input/";
    }

    schema_path+=path;

    const struct lys_node* found_snode =
    ly_ctx_get_node(m_node->schema->module->ctx, nullptr, schema_path.c_str());

    if(found_snode) {
        struct ly_set* result_set = lyd_get_node2(m_node, found_snode);
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
