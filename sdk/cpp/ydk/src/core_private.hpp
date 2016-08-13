// YANG Development Kit
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

#ifndef YDK_PRIVATE_HPP
#define YDK_PRIVATE_HPP

#include "core.hpp"
#include "libyang/libyang.h"
#include "libyang/tree_schema.h"
#include "libyang/tree_data.h"
#include "libyang/xml.h"


#include <algorithm>
#include <map>
#include <cstdlib>
#include <functional>
#include <iostream>

namespace ydk {
    namespace core {
        
        std::vector<std::string> segmentalize(const std::string& path);
        
        class SchemaNodeImpl : public SchemaNode
        {
        public:
            SchemaNodeImpl(const SchemaNode* parent, struct lys_node* node);
            
            virtual ~SchemaNodeImpl();
            
            std::string path() const;
            
            std::vector<SchemaNode*> find(const std::string& path) const;
            
            
            const SchemaNode* parent() const noexcept;
            
            std::vector<SchemaNode*> children() const;
            
            
            const SchemaNode* root() const noexcept;
            
            Statement statement() const;
            
            const SchemaNode* m_parent;
            struct lys_node* m_node;
            std::vector<SchemaNode*> m_children;
            
            
        };
        
        class RootSchemaNodeImpl : public RootSchemaNode
        {
        public:
            RootSchemaNodeImpl(struct ly_ctx* ctx) ;
            virtual ~RootSchemaNodeImpl();
            
            std::vector<SchemaNode*> find(const std::string& path) const;
            
            std::vector<SchemaNode*> children() const;
            DataNode* create(const std::string& path) const;
            DataNode* create(const std::string& path, const std::string& value) const;
            
            
            
            DataNode* from_xml(const std::string& xml) const;
            
            
            Rpc* rpc(const std::string& path) const;
            
            
            struct ly_ctx* m_ctx;
            std::vector<SchemaNode*> m_children;
            
        };
        
        
        
        class RootDataImpl : public DataNode {
        public:
            RootDataImpl(const RootSchemaNodeImpl* schema);
            
            virtual ~RootDataImpl();
            
            const SchemaNode* schema() const;
            
            std::string path() const;
            
            DataNode* create(const std::string& path, const std::string& value);
            
            void set(const std::string& value);
            
            std::string get() const;
            
            std::vector<DataNode*> find(const std::string& path) const;
            
            
            DataNode* parent()	const;
            
            std::vector<DataNode*> children() const;
            
            const DataNode* root() const;
            
            std::string xml() const;
            
            const RootSchemaNodeImpl* m_schema;
            std::map<std::string, DataNode*> m_childmap;
            
            
        };
        
        
        class DataNodeImpl : public DataNode{
            
        public:
            DataNodeImpl(DataNode* parent, struct lyd_node* node);
            
            //no copy constructor
            DataNodeImpl(const DataNodeImpl& dn) = delete;
            
            DataNodeImpl(DataNodeImpl&& dn) = delete;
            
            virtual ~DataNodeImpl();
            
            const SchemaNode* schema() const;
            
            std::string path() const;
            
            // Create a new data node based on a simple XPath
            // The new node is normally inserted at the end, either as the last child of a parent.
            //
            // If path points to a list key and the list does not exist, the key value from the predicate is used
            // and value is ignored
            //
            // returns the first created or updated node
            DataNode* create(const std::string& path, const std::string& value);
            
            void set(const std::string& value);
            
            std::string get() const;
            
            std::vector<DataNode*> find(const std::string& path) const;
            
            
            DataNode* parent() const;
            
            std::vector<DataNode*> children() const;
            
            
            const DataNode* root() const;
            
            std::string xml() const;
            
            DataNodeImpl* get_dn_for_desc_node(struct lyd_node* desc_node) const;
            
            DataNode* m_parent;
            struct lyd_node* m_node;
            std::map<struct lyd_node*, DataNodeImpl*> child_map;
            
        };
        
        class RpcImpl : public Rpc {
        public:
            
            RpcImpl(SchemaNodeImpl* sn, struct lyd_node * node);
            
            virtual ~RpcImpl();
            
            virtual DataNode* operator()(const ServiceProvider& provider) = 0;
            
            
            virtual DataNode* input() const;
            
            
            SchemaNodeImpl* m_sn;
            struct lyd_node* m_node;
        };
    }
    
}




#endif /* YDK_PRIVATE_HPP */
