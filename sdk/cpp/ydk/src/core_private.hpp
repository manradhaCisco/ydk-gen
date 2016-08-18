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
#include <cstring>
#include <cassert>

namespace ydk {
    namespace core {
        
        void xml_print_node(std::string& out, int level, const struct lyd_node *node, int toplevel);
        
        struct mlist {
            struct mlist *next;
            struct lys_module *module;
        } *mlist = NULL, *mlist_new;
        
        int
        modlist_add(struct mlist **mlist, const struct lys_module *mod);
        
        void
        xml_print_ns(std::string& out,  const struct lyd_node *node);
        
        void
        xml_print_attrs(std::string& out, const struct lyd_node *node);
        
        void
        xml_print_leaf(std::string& out, int level, const struct lyd_node *node, int toplevel);
        
        
        void
        xml_print_container(std::string& out, int level, const struct lyd_node *node, int toplevel);
        
        
        void
        xml_print_list(std::string& out, int level, const struct lyd_node *node, int is_list, int toplevel);
        
        
        void
        xml_print_anyxml(std::string& out, int level, const struct lyd_node *node, int toplevel);
        
        
        void
        xml_print_node(std::string& out, int level, const struct lyd_node *node, int toplevel);
        
        int
        xml_print_data(std::string& out, const struct lyd_node *root, int options);
        
        int
        ly_print(std::string& out, const char *format, ...);
        
        const char *
        transform_json2xml(const struct lys_module *module, const char *expr, const char ***prefixes, const char ***namespaces, uint32_t *ns_count);
        
        const char *
        transform_json2xml(const struct lys_module *module, const char *expr, int schema, const char ***prefixes,
                           const char ***namespaces, uint32_t *ns_count);
        
        int
        nscmp(const struct lyd_node *node1, const struct lyd_node *node2);
        
        int
        lyxml_dump_text(std::string& out, const char *text);

        const char *
        transform_module_name2import_prefix(const struct lys_module *module, const char *module_name);
        
        const char *
        strpbrk_backwards(const char *s, const char *accept, unsigned int s_len);

        
        std::vector<std::string> segmentalize(const std::string& path);
        
        
        
        void *
        ly_realloc(void *ptr, size_t size);
       
        
        /**
         * @brief Compare strings
         * @param[in] s1 First string to compare
         * @param[in] s2 Second string to compare
         * @param[in] both_in_dictionary Flag for optimization, 1 if it is sure that \p s1 and \p s2 were stored in dictionary.
         * This parameter is supposed to be a number (digit) known in compile time, not a variable or expression!
         * @return 1 if both strings are the same, 0 if they differ.
         */
        int ly_strequal_(const char *s1, const char *s2);
        #define ly_strequal0(s1, s2) ly_strequal_(s1, s2)
        #define ly_strequal1(s1, s2) (s1 == s2)
        #define ly_strequal(s1, s2, d) ly_strequal##d(s1, s2)
        
        
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
        
        
        

        
        
        class DataNodeImpl : public DataNode{
            
        public:
            DataNodeImpl(DataNode* parent, struct lyd_node* node);
            
            //no copy constructor
            DataNodeImpl(const DataNodeImpl& dn) = delete;
            
            DataNodeImpl(DataNodeImpl&& dn) = delete;
            
            virtual ~DataNodeImpl();
            
            virtual const SchemaNode* schema() const;
            
            virtual std::string path() const;
            
            // Create a new data node based on a simple XPath
            // The new node is normally inserted at the end, either as the last child of a parent.
            //
            // If path points to a list key and the list does not exist, the key value from the predicate is used
            // and value is ignored
            //
            // returns the first created or updated node
            virtual DataNode* create(const std::string& path, const std::string& value);
            
            virtual void set(const std::string& value);
            
            virtual std::string get() const;
            
            virtual std::vector<DataNode*> find(const std::string& path) const;
            
            
            virtual DataNode* parent() const;
            
            virtual std::vector<DataNode*> children() const;
            
            
            virtual const DataNode* root() const;
            
            virtual std::string xml() const;
            
            virtual DataNodeImpl* get_dn_for_desc_node(struct lyd_node* desc_node) const;
            
            DataNode* m_parent;
            struct lyd_node* m_node;
            std::map<struct lyd_node*, DataNodeImpl*> child_map;
            
        };
        
        
        class RootDataImpl : public DataNodeImpl {
        public:
            RootDataImpl(const SchemaNode* schema, struct ly_ctx* ctx, const std::string path);
            
            virtual ~RootDataImpl();
            
            virtual const SchemaNode* schema() const;
            
            virtual std::string path() const;
            
            virtual DataNode* create(const std::string& path, const std::string& value);
            
            virtual void set(const std::string& value);
            
            virtual std::string get() const;
            
                      
            virtual std::vector<DataNode*> children() const;
            
            virtual const DataNode* root() const;
            
            
            
            const SchemaNode* m_schema;
            
            struct ly_ctx* m_ctx;
            
            std::string m_path;
            
        };
        
        
        class RpcImpl : public Rpc {
        public:
            
            RpcImpl(SchemaNodeImpl* sn, struct ly_ctx* ctx);
            
            virtual ~RpcImpl();
            
            virtual DataNode* operator()(const ServiceProvider& provider);
            
            
            virtual DataNode* input() const;
            
            virtual SchemaNode* schema() const;
            
            
            SchemaNodeImpl* m_sn;
            DataNode* m_input_dn;
            
            
        };
    }
    
    
    

    
    
}




#endif /* YDK_PRIVATE_HPP */
