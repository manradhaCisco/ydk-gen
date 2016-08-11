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


#include "ydk_private.hpp"


////////////////////////////////////////////////////////////////////
/// Function segmentalize()
////////////////////////////////////////////////////////////////////
std::vector<std::string>
ydk::segmentalize(const std::string& path)
{
    const std::string token {"/"};
    std::vector<std::string> output;
    size_t pos = std::string::npos; // size_t to avoid improbable overflow
    std::string data{path};
    do
    {
        pos = data.find(token);
        output.push_back(data.substr(0, pos));
        if (std::string::npos != pos)
            data = data.substr(pos + token.size());
    } while (std::string::npos != pos);
    return output;
}


/////////////////////////////////////////////////////////////////////
// ydk::SchemaNodeImpl
////////////////////////////////////////////////////////////////////
ydk::SchemaNodeImpl::SchemaNodeImpl(const SchemaNode* parent, struct lys_node* node):m_parent{parent}, m_node{node}
{
	node->priv = this;
    const struct lys_node *last = nullptr;
    while( auto q = lys_getnext(last, node, nullptr, 0)) {
        m_children.emplace_back(new ydk::SchemaNodeImpl{this,const_cast<struct lys_node*>(q)});
        last = q;
    }

}

ydk::SchemaNodeImpl::~SchemaNodeImpl()
{
	//delete all the children
	for(auto p : children()) {
		delete p;
	}
}

std::string
ydk::SchemaNodeImpl::path() const
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

std::vector<ydk::SchemaNode*>
ydk::SchemaNodeImpl::find(const std::string& path) const
{
	if(path.empty()) {
		throw std::invalid_argument{"path is empty"};
	}

	//has to be a relative path
	if(path.at(0) == '/') {
		throw std::invalid_argument{"path must be a relative path"};
	}

	std::vector<ydk::SchemaNode*> ret;
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

const ydk::SchemaNode*
ydk::SchemaNodeImpl::parent() const noexcept
{
	return m_parent;
}

std::vector<ydk::SchemaNode*>
ydk::SchemaNodeImpl::children() const
{
	
	return m_children;
}

const ydk::SchemaNode*
ydk::SchemaNodeImpl::root() const noexcept
{
	if(m_parent == nullptr){
		return this;
	} else {
		return m_parent->root();
	}
}

ydk::Statement
ydk::SchemaNodeImpl::statement() const
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
		s.keyword = "anyxml";
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



/////////////////////////////////////////////////////////////////////////////////////
// class RootSchemaNodeImpl
/////////////////////////////////////////////////////////////////////////////////////
ydk::RootSchemaNodeImpl::RootSchemaNodeImpl(struct ly_ctx* ctx) : m_ctx{ctx}
{
	//populate the tree
	uint32_t idx = 0;

	while( auto p = ly_ctx_get_module_iter(ctx, &idx)) {
		const struct lys_node *last = nullptr;
		while( auto q = lys_getnext(last, nullptr, p, 0)) {
			m_children.push_back(new SchemaNodeImpl{this, const_cast<struct lys_node*>(q)});
			last = q;
		}
	}

}

ydk::RootSchemaNodeImpl::~RootSchemaNodeImpl()
{
	if(m_ctx){
		ly_ctx_destroy(m_ctx, nullptr);
		m_ctx = nullptr;
	}
}

std::vector<ydk::SchemaNode*>
ydk::RootSchemaNodeImpl::find(const std::string& path) const
{
	if(path.empty()) {
		throw std::invalid_argument{"path is empty"};
	}

	//has to be a relative path
	if(path.at(0) == '/') {
		throw std::invalid_argument{"path must be a relative path"};
	}

	std::vector<ydk::SchemaNode*> ret;


	const struct lys_node* found_node = ly_ctx_get_node(m_ctx, nullptr, path.c_str());

	if (found_node){
		auto p = reinterpret_cast<SchemaNode*>(found_node->priv);
		if(p) {
			ret.push_back(p);
		}
	}

	return ret;
}

std::vector<ydk::SchemaNode*>
ydk::RootSchemaNodeImpl::children() const
{
	return m_children;
}

ydk::DataNode*
ydk::RootSchemaNodeImpl::create(const std::string& path) const
{
	return create(path, "");
}

ydk::DataNode*
ydk::RootSchemaNodeImpl::create(const std::string& path, const std::string& value) const
{
	ydk::RootDataImpl* rd = new ydk::RootDataImpl{this};

	if (rd){
		return rd->create(path, value);
	}
	return nullptr;
}

ydk::DataNode*
ydk::RootSchemaNodeImpl::from_xml(const std::string& xml) const
{
	struct lyd_node *root = lyd_parse_mem(m_ctx, xml.c_str(), LYD_XML, 0);
	ydk::RootDataImpl* rd = new ydk::RootDataImpl{this};
	ydk::DataNodeImpl* nodeImpl = new ydk::DataNodeImpl{rd,root};

	return nodeImpl;

}



ydk::Rpc*
ydk::RootSchemaNodeImpl::rpc(const std::string& path) const
{
	//TODO
	return nullptr;
}

///////////////////////////////////////////////////////////////////////////////
// class ydk::RootDataImpl
//////////////////////////////////////////////////////////////////////////
ydk::RootDataImpl::RootDataImpl(const ydk::RootSchemaNodeImpl* schema) : m_schema{schema}
{

}

ydk::RootDataImpl::~RootDataImpl()
{
    for(auto c : children())
        delete c;
}

const ydk::SchemaNode*
ydk::RootDataImpl::schema() const
{
	return m_schema;
}

std::string
ydk::RootDataImpl::path() const
{
	return "/";
}

ydk::DataNode*
ydk::RootDataImpl::create(const std::string& path, const std::string& value)
{
    if(path.empty()){
        throw std::invalid_argument{"Path is empty"};
        
    }
    //path should not start with /
    if(path.at(0) == '/'){
        throw std::invalid_argument{"Path starts with /"};
    }
    
    std::vector<std::string> segments = ydk::segmentalize(path);
    auto iter =  m_childmap.find(segments[0]);
    
    DataNode* startdn = nullptr;
    bool created = false;
    if( m_childmap.end() == iter ){
        //we don't have a child for this segment
        std::string p{"/"};
        p+=path;
        
        struct lyd_node* dnode = lyd_new_path(nullptr, m_schema->m_ctx, p.c_str(), segments.size() == 1 ? value.c_str():nullptr, 0);
        if (!dnode) {
            throw std::invalid_argument{"Path is invalid"};
            
        }
        startdn = new DataNodeImpl{this, dnode};
        created = true;
    } else{
        startdn = iter->second;
    }
    
    DataNode* rdn = nullptr;
    
    if(segments.size() == 1) {
        //this is the only node to create
        rdn = startdn;
    } else {
        std::string remaining_path;
        bool addslash = false;
        for(size_t i = 1; i< segments.size(); i++){
            if (addslash)
                remaining_path+="/";
            else
                addslash = true;
            remaining_path+=segments[i];
        }
        //create the remaining node tree
        try{
             rdn = startdn->create(remaining_path, value);
            
            }catch(...){
                if(created)
                    delete startdn;
                throw std::invalid_argument{"Path is invalid"};
        }
            
        
    }

    if(created)
        m_childmap.insert(std::make_pair(segments[0], startdn));
    
    return rdn;
}

void
ydk::RootDataImpl::set(const std::string& value)
{
	throw std::invalid_argument{"Invalid value being assigned to root."};
}

std::string
ydk::RootDataImpl::get() const
{
	return "";
}

std::vector<ydk::DataNode*>
ydk::RootDataImpl::find(const std::string& path) const
{
	return std::vector<ydk::DataNode*>{};
}

ydk::DataNode*
ydk::RootDataImpl::parent() const
{
	return nullptr;
}

std::vector<ydk::DataNode*>
ydk::RootDataImpl::children() const
{
    std::vector<ydk::DataNode*> ret{};
    
    for(auto p : m_childmap ){
        ret.push_back(p.second);
    }
    return ret;
}

const ydk::DataNode*
ydk::RootDataImpl::root() const
{
	return this;
}

//std::string
//ydk::RootDataImpl::xml() const
//{
//    std::string ret{};
//    
//    if(m_childmap.size() != 0){
//        if(m_childmap.size() == 1) {
//            ret = m_childmap.begin()->second->xml();
//        }
//    }
//    
//    return ret;
//}

/////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////
// class ydk::DataNodeImpl
//////////////////////////////////////////////////////////////////////////
ydk::DataNodeImpl::DataNodeImpl(ydk::DataNode* parent, struct lyd_node* node): m_parent{parent}, m_node{node}
{
	//add the children
    if(m_node->child && !(m_node->schema->nodetype == LYS_LEAF ||
                          m_node->schema->nodetype == LYS_LEAFLIST ||
                          m_node->schema->nodetype == LYS_ANYXML)){
        struct lyd_node *iter = nullptr;
        LY_TREE_FOR(m_node->child, iter) {
            ydk::DataNodeImpl* dn = new ydk::DataNodeImpl{this, iter};
            child_map.insert(std::make_pair(iter, dn));
        }
    }

}

ydk::DataNodeImpl::~DataNodeImpl()
{
    //first destroy the children
	for (auto p : child_map) {
	   delete p.second;
	}

	if(m_node){
		lyd_free(m_node);
		m_node = nullptr;
	}
}

const ydk::SchemaNode*
ydk::DataNodeImpl::schema() const
{
	return reinterpret_cast<const SchemaNode*>(m_node->schema->priv);
}

std::string
ydk::DataNodeImpl::path() const
{
	char* path = lyd_path(m_node);
	if (!path) {
		return std::string{};
	}
	std::string str{path};
	std::free(path);
	return str;
}

ydk::DataNode*
ydk::DataNodeImpl::create(const std::string& path, const std::string& value)
{
	if(path.empty()){
		throw std::invalid_argument{"Path is empty."};
	}

    std::vector<std::string> segments = ydk::segmentalize(path);

	ydk::DataNodeImpl* dn = this;

    size_t start_index = 0;
	auto iter = segments.begin();
	while (iter != segments.end()) {
		auto r = dn->find(*iter);
	    if(r.empty()){
	    	break;
	    } else if(r.size() != 1){
	    	throw std::invalid_argument{"ambiguous path."};
	    } else {
	    	dn = dynamic_cast<ydk::DataNodeImpl*>(r[0]);
	    	if (dn == nullptr) {
	    		throw YDKException{};
	    	}
	        ++iter;
            start_index++;
	    }
	}

	if (segments.empty()) {
		throw std::invalid_argument{"path points to existing node."};
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
				throw std::invalid_argument{"invalid path"};
			}

		} else if (!first_node_created) {
			first_node_created = cn;
		}
	}

    auto p = new ydk::DataNodeImpl{dn, first_node_created};
    dn->child_map.insert(std::make_pair(first_node_created, p));
    
    ydk::DataNodeImpl* rdn = p;
    
	while(!rdn->children().empty() && rdn->m_node != cn){
        rdn = dynamic_cast<ydk::DataNodeImpl*>(rdn->children()[0]);
	}

	return rdn;

}

void
ydk::DataNodeImpl::set(const std::string& value)
{
	//set depends on the kind of the node
	struct lys_node* s_node = m_node->schema;

	if (s_node->nodetype == LYS_LEAF || s_node->nodetype == LYS_LEAFLIST) {
		struct lyd_node_leaf_list* leaf= reinterpret_cast<struct lyd_node_leaf_list *>(m_node);
		if(lyd_change_leaf(leaf, value.c_str())) {
			throw std::invalid_argument{"Invalid value"};
		}

	} else if (s_node->nodetype == LYS_ANYXML) {
		struct lyd_node_anyxml* anyxml = reinterpret_cast<struct lyd_node_anyxml *>(m_node);
		anyxml->xml_struct = 0;
		anyxml->value.str = value.c_str();

	}else {
		throw std::invalid_argument{"Cannot set value for this Data Node"};
	}
}

std::string
ydk::DataNodeImpl::get() const
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

std::vector<ydk::DataNode*>
ydk::DataNodeImpl::find(const std::string& path) const
{
	std::vector<DataNode*> results;
    
    const struct lys_node* found_snode =
        ly_ctx_get_node(m_node->schema->module->ctx, m_node->schema, path.c_str());
    
    if(found_snode) {
        struct ly_set* result_set = lyd_get_node2(m_node, found_snode);
        if (result_set->number > 0){
            for(size_t i=0; i < result_set->number; i++){
                struct lyd_node* node_result = result_set->set.d[i];
                results.push_back(get_dn_for_desc_node(node_result));
            }
        }
        ly_set_free(result_set);

    }
    
	

	return results;
}

ydk::DataNode*
ydk::DataNodeImpl::parent() const
{
	return m_parent;
}

std::vector<ydk::DataNode*>
ydk::DataNodeImpl::children() const
{
	std::vector<DataNode*> ret{};
	//the ordering should be determined by the lyd_node
	struct lyd_node *iter;
    if(m_node->child && !(m_node->schema->nodetype == LYS_LEAF ||
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

const ydk::DataNode*
ydk::DataNodeImpl::root() const
{
	if(m_parent){
		return m_parent->root();
	}
	return this;
}

std::string
ydk::DataNodeImpl::xml() const
{
	std::string ret;
	char* xml = nullptr;
	if(!lyd_print_mem(&xml, m_node,LYD_XML, LYP_FORMAT)) {
		ret = xml;
		std::free(xml);
	}
	return ret;
}

ydk::DataNodeImpl*
ydk::DataNodeImpl::get_dn_for_desc_node(struct lyd_node* desc_node) const
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
    
	for( auto p : nodes)
    {
		auto res = parent->child_map.find(p);
        
	   if(res != parent->child_map.end()) {
		   //DataNode is already present
		   dn = res->second;

	   } else {
           throw YDKException{};
	   }
	   parent = dn;
	}

	return dn;
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// class ydk::ValidationService
//////////////////////////////////////////////////////////////////////////
void
ydk::ValidationService::validate(const ydk::DataNode* dn, ydk::ValidationService::Option option)
{
    std::string option_str = "";
    switch(option) {
        case ValidationService::Option::DATASTORE:
            option_str="DATATSTORE";
            break;
        case ValidationService::Option::EDIT_CONFIG:
            option_str="EDIT-CONFIG";
            break;
        case ValidationService::Option::GET:
            option_str="GET";
            break;
        case ValidationService::Option::GET_CONFIG:
            option_str="GET-CONFIG";
            break;
            
    }
    std::cout << "Validation called on " << dn->path() << " with option " << option_str << std::endl;
}


///////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// class ydk::CodecService
//////////////////////////////////////////////////////////////////////////
std::string
ydk::CodecService::encode(const ydk::DataNode* dn, ydk::CodecService::Format format, bool pretty)
{
    std::string ret{};
    
    
    LYD_FORMAT scheme = LYD_XML;
  
    
    if(format == ydk::CodecService::Format::JSON) {
        scheme = LYD_JSON;
    }
    struct lyd_node* m_node = nullptr;
    
    
    //first determine what kind of node this is
    if(dn->parent()) {
        //note a root node
        const ydk::DataNodeImpl* impl = dynamic_cast<const ydk::DataNodeImpl *>(dn);
        if( !impl) {
            throw ydk::YDKCoreException{};
        }
        m_node = impl->m_node;
        
    } else {
        //TODO
        return ret;
    }
    char* buffer;
    if(!lyd_print_mem(&buffer, m_node,scheme, pretty ? LYP_FORMAT : 0)) {
        ret = buffer;
        std::free(buffer);
    }

    return ret;
}

ydk::DataNode*
ydk::CodecService::decode(const ydk::RootSchemaNode* root_schema, const std::string& buffer, ydk::CodecService::Format format)
{
    LYD_FORMAT scheme = LYD_XML;
    if (format == ydk::CodecService::Format::JSON) {
        scheme = LYD_JSON;
    }
    const ydk::RootSchemaNodeImpl* rs_impl = dynamic_cast<const ydk::RootSchemaNodeImpl*>(root_schema);
    if(!rs_impl){
        throw YDKCoreException{};
    }
    
    struct lyd_node *root = lyd_parse_mem(rs_impl->m_ctx, buffer.c_str(), scheme, 0);
    ydk::RootDataImpl* rd = new ydk::RootDataImpl{rs_impl};
    ydk::DataNodeImpl* nodeImpl = new ydk::DataNodeImpl{rd,root};
    return nodeImpl;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// class ydk::Repository
//////////////////////////////////////////////////////////////////////////

ydk::Repository::Repository(const std::string& search_dir) : m_search_dir{search_dir}
{
    ly_verb(LY_LLDBG);
}

ydk::RootSchemaNode*
ydk::Repository::create_root_schema(const std::vector<ydk::Capability> capabilities)
{
	struct ly_ctx* ctx = ly_ctx_new(m_search_dir.c_str());

	if(!ctx) {
		throw std::bad_alloc{};
	}
    
    for (auto c : capabilities) {
        auto p = ly_ctx_load_module(ctx, c.module.c_str(), c.revision.empty()?0:c.revision.c_str());
        if (!p) {
            ly_ctx_destroy(ctx, nullptr);
            throw YDKException{};
        }
        for (auto f : c.features)
            lys_features_enable(p, f.c_str());
        
    }

	ydk::RootSchemaNodeImpl* rs = new ydk::RootSchemaNodeImpl{ctx};

	return rs;
}

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// class Rpc
////////////////////////////////////////////////////////////////////////////////
ydk::DataNode*
ydk::Rpc::operator()(const ServiceProvider& provider)
{
	return provider.invoke(this);
}

ydk::DataNode*
ydk::Rpc::input() const
{
	//TODO
	return nullptr;
}

//std::string
//ydk::Rpc::xml() const
//{
//	//TODO
//	return "";
//}


