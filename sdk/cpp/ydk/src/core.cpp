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



////////////////////////////////////////////////////////////////////
/// Function segmentalize()
////////////////////////////////////////////////////////////////////
std::vector<std::string>
ydk::core::segmentalize(const std::string& path)
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

/////////////////////////////////////////////////////////////////////////
/// YDKCodecException
/////////////////////////////////////////////////////////////////////////
ydk::core::YDKCodecException::YDKCodecException(YDKCodecException::Error ec) : YDKException(ly_errmsg()), err{ec}
{
    
}






/////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////
// ydk::SchemaNodeImpl
////////////////////////////////////////////////////////////////////
ydk::core::SchemaNodeImpl::SchemaNodeImpl(const SchemaNode* parent, struct lys_node* node):m_parent{parent}, m_node{node}
{
	node->priv = this;
    const struct lys_node *last = nullptr;
    while( auto q = lys_getnext(last, node, nullptr, 0)) {
        m_children.emplace_back(new SchemaNodeImpl{this,const_cast<struct lys_node*>(q)});
        last = q;
    }

}

ydk::core::SchemaNodeImpl::~SchemaNodeImpl()
{
	//delete all the children
	for(auto p : children()) {
		delete p;
	}
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
		throw ydk::core::YDKInvalidArgumentException{"path is empty"};
	}

	//has to be a relative path
	if(path.at(0) == '/') {
		throw ydk::core::YDKInvalidArgumentException{"path must be a relative path"};
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
ydk::core::RootSchemaNodeImpl::RootSchemaNodeImpl(struct ly_ctx* ctx) : m_ctx{ctx}
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

ydk::core::RootSchemaNodeImpl::~RootSchemaNodeImpl()
{
	if(m_ctx){
		ly_ctx_destroy(m_ctx, nullptr);
		m_ctx = nullptr;
	}
}

std::vector<ydk::core::SchemaNode*>
ydk::core::RootSchemaNodeImpl::find(const std::string& path) const
{
	if(path.empty()) {
		throw ydk::core::YDKInvalidArgumentException{"path is empty"};
	}

	//has to be a relative path
	if(path.at(0) == '/') {
		throw ydk::core::YDKInvalidArgumentException{"path must be a relative path"};
	}

	std::vector<SchemaNode*> ret;


	const struct lys_node* found_node = ly_ctx_get_node(m_ctx, nullptr, path.c_str());

	if (found_node){
		auto p = reinterpret_cast<SchemaNode*>(found_node->priv);
		if(p) {
			ret.push_back(p);
		}
	}

	return ret;
}

std::vector<ydk::core::SchemaNode*>
ydk::core::RootSchemaNodeImpl::children() const
{
	return m_children;
}

ydk::core::DataNode*
ydk::core::RootSchemaNodeImpl::create(const std::string& path) const
{
	return create(path, "");
}

ydk::core::DataNode*
ydk::core::RootSchemaNodeImpl::create(const std::string& path, const std::string& value) const
{
	RootDataImpl* rd = new RootDataImpl{this, m_ctx, "/"};

	if (rd){
		return rd->create(path, value);
	}
	return nullptr;
}

ydk::core::DataNode*
ydk::core::RootSchemaNodeImpl::from_xml(const std::string& xml) const
{
	struct lyd_node *root = lyd_parse_mem(m_ctx, xml.c_str(), LYD_XML, 0);
	RootDataImpl* rd = new RootDataImpl{this, m_ctx, "/"};
	DataNodeImpl* nodeImpl = new DataNodeImpl{rd,root};

	return nodeImpl;

}



ydk::core::Rpc*
ydk::core::RootSchemaNodeImpl::rpc(const std::string& path) const
{
	
    struct lyd_node* node = lyd_new_path(nullptr, m_ctx, path.c_str(), "", 0);
    
    if(node == nullptr) {
        throw YDKInvalidArgumentException{"Path is invalid"};
    }
    
    SchemaNodeImpl* sn = new SchemaNodeImpl{this, node->schema};
    struct ly_ctx* ctx = node->schema->module->ctx;
    lyd_free(node);
    
    return new RpcImpl{sn, ctx};
}

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
        throw ydk::core::YDKInvalidArgumentException{"Path is empty"};
        
    }
    //path should not start with /
    if(path.at(0) == '/'){
        throw ydk::core::YDKInvalidArgumentException{"Path starts with /"};
    }
    std::vector<std::string> segments = segmentalize(path);
    
    
    

    std::string start_seg = m_path + segments[0];
    struct lyd_node* dnode = lyd_new_path(m_node, m_ctx, start_seg.c_str(),
                                          segments.size() == 1 ? value.c_str():nullptr,0);
    
    
    
    if( dnode == nullptr){
        throw ydk::core::YDKInvalidArgumentException{"Path is invalid."};
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
	throw ydk::core::YDKInvalidArgumentException{"Invalid value being assigned to root."};
}

std::string
ydk::core::RootDataImpl::get() const
{
	return "";
}

std::vector<ydk::core::DataNode*>
ydk::core::RootDataImpl::find(const std::string& path) const
{
	return std::vector<DataNode*>{};
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
            
        } while (iter != m_node);
    }
    
    return ret;
}

const ydk::core::DataNode*
ydk::core::RootDataImpl::root() const
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




//////////////////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////

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
		throw ydk::core::YDKInvalidArgumentException{"Path is empty."};
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
            throw YDKPathException{YDKPathException::Error::PATH_AMBIGUOUS};
	    } else {
	    	dn = dynamic_cast<DataNodeImpl*>(r[0]);
	    	if (dn == nullptr) {
	    		throw YDKCoreException{};
	    	}
	        ++iter;
            start_index++;
	    }
	}

	if (segments.empty()) {
		throw ydk::core::YDKInvalidArgumentException{"path points to existing node."};
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
				throw ydk::core::YDKInvalidArgumentException{"invalid path"};
			}

		} else if (!first_node_created) {
			first_node_created = cn;
		}
	}

    auto p = new DataNodeImpl{dn, first_node_created};
    dn->child_map.insert(std::make_pair(first_node_created, p));
    
    DataNodeImpl* rdn = p;
    
	while(!rdn->children().empty() && rdn->m_node != cn){
        rdn = dynamic_cast<DataNodeImpl*>(rdn->children()[0]);
	}

	return rdn;

}

void
ydk::core::DataNodeImpl::set(const std::string& value)
{
	//set depends on the kind of the node
	struct lys_node* s_node = m_node->schema;

	if (s_node->nodetype == LYS_LEAF || s_node->nodetype == LYS_LEAFLIST) {
		struct lyd_node_leaf_list* leaf= reinterpret_cast<struct lyd_node_leaf_list *>(m_node);
		if(lyd_change_leaf(leaf, value.c_str())) {
			throw ydk::core::YDKInvalidArgumentException{"Invalid value"};
		}

	} else if (s_node->nodetype == LYS_ANYXML) {
		struct lyd_node_anyxml* anyxml = reinterpret_cast<struct lyd_node_anyxml *>(m_node);
		anyxml->xml_struct = 0;
		anyxml->value.str = value.c_str();

	}else {
		throw ydk::core::YDKInvalidArgumentException{"Cannot set value for this Data Node"};
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
    
    const struct lys_node* found_snode =
        ly_ctx_get_node(m_node->schema->module->ctx, m_node->schema, path.c_str());
    
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
    
	for( auto p : nodes)
    {
		auto res = parent->child_map.find(p);
        
	   if(res != parent->child_map.end()) {
		   //DataNode is already present
		   dn = res->second;

	   } else {
           throw YDKCoreException{};
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
ydk::core::ValidationService::validate(const ydk::core::DataNode* dn, ydk::core::ValidationService::Option option)
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
ydk::core::CodecService::encode(const ydk::core::DataNode* dn, ydk::core::CodecService::Format format, bool pretty)
{
    std::string ret{};
    
    
    LYD_FORMAT scheme = LYD_XML;
  
    
    if(format == ydk::core::CodecService::Format::JSON) {
        scheme = LYD_JSON;
    }
    struct lyd_node* m_node = nullptr;
    
    
    //first determine what kind of node this is
    if(dn->parent()) {
        //note a root node
        const DataNodeImpl* impl = dynamic_cast<const DataNodeImpl *>(dn);
        if( !impl) {
            throw YDKCoreException{};
        }
        m_node = impl->m_node;
        
    } else {
        //TODO
        return ret;
    }
    //int rc = lyd_validate(&m_node, LYD_OPT_GET);
    //if (rc ) {
      //  throw YDKCoreException();
    //}

    xml_print_data(ret, m_node, pretty ? LYP_FORMAT : 0);

    return ret;
}

ydk::core::DataNode*
ydk::core::CodecService::decode(const RootSchemaNode* root_schema, const std::string& buffer, CodecService::Format format)
{
    LYD_FORMAT scheme = LYD_XML;
    if (format == CodecService::Format::JSON) {
        scheme = LYD_JSON;
    }
    const RootSchemaNodeImpl* rs_impl = dynamic_cast<const RootSchemaNodeImpl*>(root_schema);
    if(!rs_impl){
        throw YDKCoreException{};
    }
    
    struct lyd_node *root = lyd_parse_mem(rs_impl->m_ctx, buffer.c_str(), scheme, LYD_OPT_TRUSTED | LYD_OPT_GET);
    if( ly_errno ) {
        std::cout << ly_errmsg() << std::endl;
        throw YDKCodecException{YDKCodecException::Error::XML_INVAL};
    }

    RootDataImpl* rd = new RootDataImpl{rs_impl, rs_impl->m_ctx, "/"};
    DataNodeImpl* nodeImpl = new DataNodeImpl{rd,root};
    return nodeImpl;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// class ydk::Repository
//////////////////////////////////////////////////////////////////////////

ydk::core::Repository::Repository(const std::string& search_dir) : m_search_dir{search_dir}
{
    ly_verb(LY_LLDBG);
}

ydk::core::RootSchemaNode*
ydk::core::Repository::create_root_schema(const std::vector<ydk::core::Capability> capabilities)
{
	struct ly_ctx* ctx = ly_ctx_new(m_search_dir.c_str());

	if(!ctx) {
		throw std::bad_alloc{};
	}
    
    for (auto c : capabilities) {
        auto p = ly_ctx_load_module(ctx, c.module.c_str(), c.revision.empty()?0:c.revision.c_str());
        if (!p) {
            ly_ctx_destroy(ctx, nullptr);
            throw YDKException{"Unable to parse module"};
        }
        for (auto f : c.features)
            lys_features_enable(p, f.c_str());
        
    }

	RootSchemaNodeImpl* rs = new RootSchemaNodeImpl{ctx};

	return rs;
}

////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// class RpcImpl
////////////////////////////////////////////////////////////////////////////////

    
ydk::core::RpcImpl::RpcImpl(SchemaNodeImpl* sn, struct ly_ctx* ctx) : m_sn{sn}
{
    std::string path = sn->path() + "/";

    m_input_dn = new RootDataImpl{sn, ctx, path};
    
}

ydk::core::RpcImpl::~RpcImpl()
{
    if(m_input_dn){
        delete m_input_dn;
        m_input_dn = nullptr;
    }
    
}
    
ydk::core::DataNode*
ydk::core::RpcImpl::operator()(const ydk::core::ServiceProvider& provider)
{
    return provider.invoke(this);
}
    

ydk::core::DataNode*
ydk::core::RpcImpl::input() const
{
    return m_input_dn;
}
    
ydk::core::SchemaNode*
ydk::core::RpcImpl::schema() const
{
    return m_sn;
}
        



////////////////////////////////////////////////////////////////////////////////////////
/// xml printing fixes
/////////////////////////////////////////////////////////////////////////////////////////

#define INDENT ""
#define LEVEL (level ? level*2-2 : 0)


int
ydk::core::modlist_add(struct mlist **mlist, const struct lys_module *mod)
{
    struct mlist *iter;
    
    for (iter = *mlist; iter; iter = iter->next) {
        if (mod == iter->module) {
            break;
        }
    }
    
    if (!iter) {
        iter = (struct mlist *)std::malloc(sizeof *iter);
        if (!iter) {
            return EXIT_FAILURE;
        }
        iter->next = *mlist;
        iter->module = (struct lys_module *)mod;
        *mlist = iter;
    }
    
    return EXIT_SUCCESS;
}

void
ydk::core::xml_print_ns(std::string& out, const struct lyd_node *node)
{
    struct lyd_node *next, *cur, *node2;
    struct lyd_attr *attr;
    const struct lys_module *wdmod = NULL;
    struct mlist *mlist = NULL, *miter;
    
    
    /* add node attribute modules */
    for (attr = node->attr; attr; attr = attr->next) {
        if (modlist_add(&mlist, attr->module)) {
            goto print;
        }
    }
    
    /* add node children nodes and attribute modules */
    if (!(node->schema->nodetype & (LYS_LEAF | LYS_LEAFLIST | LYS_ANYXML))) {
        /* get with-defaults module */
        wdmod = ly_ctx_get_module(node->schema->module->ctx, "ietf-netconf-with-defaults", NULL);
        
        LY_TREE_FOR(node->child, node2) {
            LY_TREE_DFS_BEGIN(node2, next, cur) {
                if (cur->dflt && wdmod) {
                    if (modlist_add(&mlist, wdmod)) {
                        goto print;
                    }
                }
                for (attr = cur->attr; attr; attr = attr->next) {
                    if (modlist_add(&mlist, attr->module)) {
                        goto print;
                    }
                }
                LY_TREE_DFS_END(node2, next, cur)}
        }
    }
    
print:
    /* print used namespaces */
    while (mlist) {
        miter = mlist;
        mlist = mlist->next;
        
        ly_print(out, " xmlns:%s=\"%s\"", miter->module->prefix, miter->module->ns);
        free(miter);
    }
}

void
ydk::core::xml_print_attrs(std::string& out, const struct lyd_node *node)
{
    struct lyd_attr *attr;
    const char **prefs, **nss;
    const char *xml_expr;
    uint32_t ns_count, i;
    int rpc_filter = 0;
    const struct lys_module *wdmod = NULL;
    
    /* with-defaults */
    if (node->dflt) {
        /* get with-defaults module */
        wdmod = ly_ctx_get_module(node->schema->module->ctx, "ietf-netconf-with-defaults", NULL);
        if (wdmod) {
            /* print attribute only if context include with-defaults schema */
            ly_print(out, " %s:default=\"true\"", wdmod->prefix);
        }
    }
    /* technically, check for the extension get-filter-element-attributes from ietf-netconf */
    if (!strcmp(node->schema->name, "filter")
        && (!strcmp(node->schema->module->name, "ietf-netconf") || !strcmp(node->schema->module->name, "notifications"))) {
        rpc_filter = 1;
    }
    
    for (attr = node->attr; attr; attr = attr->next) {
        if (rpc_filter && !strcmp(attr->name, "type")) {
            ly_print(out, " %s=\"", attr->name);
        } else if (rpc_filter && !strcmp(attr->name, "select")) {
            xml_expr = transform_json2xml(node->schema->module, attr->value, &prefs, &nss, &ns_count);
            if (!xml_expr) {
                /* error */
                ly_print(out, "\"(!error!)\"");
                return;
            }
            
            for (i = 0; i < ns_count; ++i) {
                ly_print(out, " xmlns:%s=\"%s\"", prefs[i], nss[i]);
            }
            free(prefs);
            free(nss);
            
            ly_print(out, " %s=\"", attr->name);
            lyxml_dump_text(out, xml_expr);
            ly_print(out, "\"");
            
            lydict_remove(node->schema->module->ctx, xml_expr);
            continue;
        } else {
            ly_print(out, " %s:%s=\"", attr->module->prefix, attr->name);
        }
        lyxml_dump_text(out, attr->value);
        ly_print(out, "\"");
    }
}

void
ydk::core::xml_print_leaf(std::string& out, int level, const struct lyd_node *node, int toplevel)
{
    const struct lyd_node_leaf_list *leaf = (struct lyd_node_leaf_list *)node;
    const char *ns;
    const char **prefs, **nss;
    const char *xml_expr;
    uint32_t ns_count, i;
    
    if (toplevel || !node->parent || nscmp(node, node->parent)) {
        /* print "namespace" */
        ns = lyd_node_module(node)->ns;
        ly_print(out, "%*s<%s xmlns=\"%s\"", LEVEL, INDENT, node->schema->name, ns);
    } else {
        ly_print(out, "%*s<%s", LEVEL, INDENT, node->schema->name);
    }
    
    if (toplevel) {
        xml_print_ns(out, node);
    }
    
    xml_print_attrs(out, node);
    
    switch (leaf->value_type & LY_DATA_TYPE_MASK) {
        case LY_TYPE_BINARY:
        case LY_TYPE_STRING:
        case LY_TYPE_BITS:
        case LY_TYPE_ENUM:
        case LY_TYPE_BOOL:
        case LY_TYPE_DEC64:
        case LY_TYPE_INT8:
        case LY_TYPE_INT16:
        case LY_TYPE_INT32:
        case LY_TYPE_INT64:
        case LY_TYPE_UINT8:
        case LY_TYPE_UINT16:
        case LY_TYPE_UINT32:
        case LY_TYPE_UINT64:
            if (!leaf->value_str || !leaf->value_str[0]) {
                ly_print(out, "/>");
            } else {
                ly_print(out, ">");
                lyxml_dump_text(out, leaf->value_str);
                ly_print(out, "</%s>", node->schema->name);
            }
            break;
            
        case LY_TYPE_IDENT:
        case LY_TYPE_INST:
            xml_expr = transform_json2xml(node->schema->module, ((struct lyd_node_leaf_list *)node)->value_str,
                                          &prefs, &nss, &ns_count);
            if (!xml_expr) {
                /* error */
                ly_print(out, "\"(!error!)\"");
                return;
            }
            
            for (i = 0; i < ns_count; ++i) {
                ly_print(out, " xmlns:%s=\"%s\"", prefs[i], nss[i]);
            }
            free(prefs);
            free(nss);
            
            if (xml_expr[0]) {
                ly_print(out, ">");
                lyxml_dump_text(out, xml_expr);
                ly_print(out, "</%s>", node->schema->name);
            } else {
                ly_print(out, "/>");
            }
            lydict_remove(node->schema->module->ctx, xml_expr);
            break;
            
        case LY_TYPE_LEAFREF:
            if (leaf->value.leafref) {
                lyxml_dump_text(out, ((struct lyd_node_leaf_list *)(leaf->value.leafref))->value_str);
            } else if (leaf->value_str) {
                if(strchr(leaf->value_str ,':')) {
                    xml_expr = transform_json2xml(node->schema->module, ((struct lyd_node_leaf_list *)node)->value_str,
                                                  &prefs, &nss, &ns_count);
                    if (!xml_expr) {
                        /* error */
                        ly_print(out, "\"(!error!)\"");
                        return;
                    }
                    
                    for (i = 0; i < ns_count; ++i) {
                        ly_print(out, " xmlns:%s=\"%s\"", prefs[i], nss[i]);
                    }
                    free(prefs);
                    free(nss);
                    
                    if (xml_expr[0]) {
                        ly_print(out, ">");
                        lyxml_dump_text(out, xml_expr);
                        ly_print(out, "</%s>", node->schema->name);
                    } else {
                        ly_print(out, "/>");
                    }
                    lydict_remove(node->schema->module->ctx, xml_expr);
                    break;
                } else {
                    ly_print(out, ">");
                    lyxml_dump_text(out, leaf->value_str);
                    ly_print(out, "</%s>", node->schema->name);
                }
            }else {
                 ly_print(out, "</%s>", node->schema->name);
            }
            
            break;
            
        case LY_TYPE_EMPTY:
            ly_print(out, "/>");
            break;
            
        default:
            /* error */
            ly_print(out, "\"(!error!)\"");
    }
    
    if (level) {
        ly_print(out, "\n");
    }
}

void
ydk::core::xml_print_container(std::string& out, int level, const struct lyd_node *node, int toplevel)
{
    struct lyd_node *child;
    const char *ns;
    
    if (toplevel || !node->parent || nscmp(node, node->parent)) {
        /* print "namespace" */
        ns = lyd_node_module(node)->ns;
        ly_print(out, "%*s<%s xmlns=\"%s\"", LEVEL, INDENT, node->schema->name, ns);
    } else {
        ly_print(out, "%*s<%s", LEVEL, INDENT, node->schema->name);
    }
    
    if (toplevel) {
        xml_print_ns(out, node);
    }
    
    xml_print_attrs(out, node);
    
    if (!node->child) {
        ly_print(out, "/>%s", level ? "\n" : "");
        return;
    }
    ly_print(out, ">%s", level ? "\n" : "");
    
    LY_TREE_FOR(node->child, child) {
        xml_print_node(out, level ? level + 1 : 0, child, 0);
    }
    
    ly_print(out, "%*s</%s>%s", LEVEL, INDENT, node->schema->name, level ? "\n" : "");
}

void
ydk::core::xml_print_list(std::string& out, int level, const struct lyd_node *node, int is_list, int toplevel)
{
    struct lyd_node *child;
    const char *ns;
    
    if (is_list) {
        /* list print */
        if (toplevel || !node->parent || nscmp(node, node->parent)) {
            /* print "namespace" */
            ns = lyd_node_module(node)->ns;
            ly_print(out, "%*s<%s xmlns=\"%s\"", LEVEL, INDENT, node->schema->name, ns);
        } else {
            ly_print(out, "%*s<%s", LEVEL, INDENT, node->schema->name);
        }
        
        if (toplevel) {
            xml_print_ns(out, node);
        }
        xml_print_attrs(out, node);
        
        if (!node->child) {
            ly_print(out, "/>%s", level ? "\n" : "");
            return;
        }
        ly_print(out, ">%s", level ? "\n" : "");
        
        LY_TREE_FOR(node->child, child) {
            xml_print_node(out, level ? level + 1 : 0, child, 0);
        }
        
        ly_print(out, "%*s</%s>%s", LEVEL, INDENT, node->schema->name, level ? "\n" : "");
    } else {
        /* leaf-list print */
        xml_print_leaf(out, level, node, toplevel);
    }
}

int
ydk::core::nscmp(const struct lyd_node *node1, const struct lyd_node *node2)
{
    /* we have to cover submodules belonging to the same module */
    if (lys_node_module(node1->schema) == lys_node_module(node2->schema)) {
        /* belongs to the same module */
        return 0;
    } else {
        /* different modules */
        return 1;
    }
}

void
ydk::core::xml_print_anyxml(std::string& out, int level, const struct lyd_node *node, int toplevel)
{
    //char *buf;
    struct lyd_node_anyxml *axml = (struct lyd_node_anyxml *)node;
    const char *ns;
    
    if (toplevel || !node->parent || nscmp(node, node->parent)) {
        /* print "namespace" */
        ns = lyd_node_module(node)->ns;
        ly_print(out, "%*s<%s xmlns=\"%s\"", LEVEL, INDENT, node->schema->name, ns);
    } else {
        ly_print(out, "%*s<%s", LEVEL, INDENT, node->schema->name);
    }
    
    if (toplevel) {
        xml_print_ns(out, node);
    }
    xml_print_attrs(out, node);
    ly_print(out, ">");
    

    if (axml->value.str) {
        ly_print(out, "%s", axml->value.str);
    }
    
    
    /* closing tag */
    ly_print(out, "%*s</%s>%s", LEVEL, INDENT, node->schema->name, level ? "\n" : "");
}

void
ydk::core::xml_print_node(std::string& out, int level, const struct lyd_node *node, int toplevel)
{
    switch (node->schema->nodetype) {
        case LYS_NOTIF:
        case LYS_RPC:
        case LYS_CONTAINER:
            xml_print_container(out, level, node, toplevel);
            break;
        case LYS_LEAF:
            xml_print_leaf(out, level, node, toplevel);
            break;
        case LYS_LEAFLIST:
            xml_print_list(out, level, node, 0, toplevel);
            break;
        case LYS_LIST:
            xml_print_list(out, level, node, 1, toplevel);
            break;
        case LYS_ANYXML:
            xml_print_anyxml(out, level, node, toplevel);
            break;
        default:
            
            break;
    }
}

int
ydk::core::xml_print_data(std::string& out, const struct lyd_node *root, int options)
{
    const struct lyd_node *node;
    
    /* content */
    LY_TREE_FOR(root, node) {
        xml_print_node(out, (options & LYP_FORMAT ? 1 : 0), node, 1);
        if (!(options & LYP_WITHSIBLINGS)) {
            break;
        }
    }
    
    return EXIT_SUCCESS;
}

int
ydk::core::ly_print(std::string& out, const char *format, ...)
{
    
    char *msg = NULL;
    va_list ap;
    
    va_start(ap, format);
    
    int count = vasprintf(&msg, format, ap);
    out+=msg;
   
    free(msg);

    va_end(ap);
    return count;
}

const char *
ydk::core::transform_json2xml(const struct lys_module *module, const char *expr, const char ***prefixes, const char ***namespaces,
                   uint32_t *ns_count)
{
    return transform_json2xml(module, expr, 0, prefixes, namespaces, ns_count);
}



const char*
ydk::core::transform_json2xml(const struct lys_module *module, const char *expr, int schema, const char ***prefixes,
                    const char ***namespaces, uint32_t *ns_count)
{
    const char *in, *id, *prefix;
    char *out, *col, *name;
    size_t out_size, out_used, id_len;
    const struct lys_module *mod;
    uint32_t i;
    
    //assert(module && expr && ((!prefixes && !namespaces && !ns_count) || (prefixes && namespaces && ns_count)));
    
    if (ns_count) {
        *ns_count = 0;
        *prefixes = NULL;
        *namespaces = NULL;
    }
    
    in = expr;
    out_size = strlen(in) + 1;
    out = (char *)std::malloc(out_size);
    if (!out) {
        //LOGMEM;
        return NULL;
    }
    out_used = 0;
    
    while (1) {
        col = strchr(in, ':');
        /* we're finished, copy the remaining part */
        if (!col) {
            strcpy(&out[out_used], in);
            out_used += strlen(in) + 1;
            //assert(out_size == out_used);
            return lydict_insert_zc(module->ctx, out);
        }
        id = strpbrk_backwards(col - 1, "/ [\'\"", (col - in) - 1);
        if ((id[0] == '/') || (id[0] == ' ') || (id[0] == '[') || (id[0] == '\'') || (id[0] == '\"')) {
            ++id;
        }
        id_len = col - id;
        
        /* get the module */
        if (!schema) {
            name = strndup(id, id_len);
            mod = ly_ctx_get_module(module->ctx, name, NULL);
            free(name);
            if (!mod) {
//                LOGVAL(LYE_INMOD_LEN, LY_VLOG_NONE, NULL, id_len, id);
                goto fail;
            }
            prefix = mod->prefix;
        } else {
            name = strndup(id, id_len);
            prefix = transform_module_name2import_prefix(module, name);
            free(name);
            if (!prefix) {
//                LOGVAL(LYE_INMOD_LEN, LY_VLOG_NONE, NULL, id_len, id);
                goto fail;
            }
        }
        
        /* remember the namespace definition (only if it's new) */
        if (!schema && ns_count) {
            for (i = 0; i < *ns_count; ++i) {
                if (ly_strequal((*namespaces)[i], mod->ns, 1)) {
                    break;
                }
            }
            if (i == *ns_count) {
                ++(*ns_count);
                *prefixes = (const char **)ly_realloc(*prefixes, *ns_count * sizeof **prefixes);
                if (!(*prefixes)) {
//                    LOGMEM;
                    goto fail;
                }
                *namespaces = (const char **)ly_realloc(*namespaces, *ns_count * sizeof **namespaces);
                if (!(*namespaces)) {
 //                   LOGMEM;
                    goto fail;
                }
                (*prefixes)[*ns_count - 1] = mod->prefix;
                (*namespaces)[*ns_count - 1] = mod->ns;
            }
        }
        
        /* adjust out size */
        out_size += strlen(prefix) - id_len;
        out = (char *)ly_realloc(out, out_size);
        if (!out) {
//            LOGMEM;
            goto fail;
        }
        
        /* copy the data before prefix */
        strncpy(&out[out_used], in, id-in);
        out_used += id - in;
        
        /* copy the model prefix */
        strcpy(&out[out_used], prefix);
        out_used += strlen(prefix);
        
        /* copy ':' */
        out[out_used] = ':';
        ++out_used;
        
        /* finally adjust in pointer for next round */
        in = col + 1;
    }
    
    /* unreachable */

    
fail:
    if (!schema && ns_count) {
        free(*prefixes);
        free(*namespaces);
    }
    free(out);
    return NULL;

}

int
ydk::core::lyxml_dump_text(std::string& out, const char *text)
{
    unsigned int i, n;
    
    if (!text) {
        return 0;
    }
    
    for (i = n = 0; text[i]; i++) {
        switch (text[i]) {
            case '&':
                n += ly_print(out, "&amp;");
                break;
            case '<':
                n += ly_print(out, "&lt;");
                break;
            case '>':
                /* not needed, just for readability */
                n += ly_print(out, "&gt;");
                break;
            case '"':
                n += ly_print(out, "&quot;");
                break;
            default:
                out+=text[i];
                //ly_write(out, &text[i], 1);
                n++;
        }
    }
    
    return n;
}

const char *
ydk::core::transform_module_name2import_prefix(const struct lys_module *module, const char *module_name)
{
    uint16_t i;
    
    if (!std::strcmp(lys_main_module(module)->name, module_name)) {
        /* the same for module and submodule */
        return module->prefix;
    }
    
    for (i = 0; i < module->imp_size; ++i) {
        if (!std::strcmp(module->imp[i].module->name, module_name)) {
            return module->imp[i].prefix;
        }
    }
    
    return NULL;
}

const char *
ydk::core::strpbrk_backwards(const char *s, const char *accept, unsigned int s_len)
{
    const char *sc;
    
    for (; *s != '\0' && s_len; --s, --s_len) {
        for (sc = accept; *sc != '\0'; ++sc) {
            if (*s == *sc) {
                return s;
            }
        }
    }
    return s;
}

void *
ydk::core::ly_realloc(void *ptr, size_t size)
{
    void *new_mem;
    
    new_mem = std::realloc(ptr, size);
    if (!new_mem) {
        free(ptr);
    }
    
    return new_mem;
}

int
ydk::core::ly_strequal_(const char *s1, const char *s2)
{
    if (s1 == s2) {
        return 1;
    } else if (!s1 || !s2) {
        return 0;
    } else {
        for ( ; *s1 == *s2; s1++, s2++) {
            if (*s1 == '\0') {
                return 1;
            }
        }
        return 0;
    }
}



//int
//ydk::corely_write(std::string& out, const char *buf, size_t count)
//{
//    char *aux;
//    
//    
//    switch(out->type) {
//        case LYOUT_FD:
//            return write(out->method.fd, buf, count);
//        case LYOUT_STREAM:
//            return fwrite(buf, sizeof *buf, count, out->method.f);
//        case LYOUT_MEMORY:
//            if (out->method.mem.len + count + 1 > out->method.mem.size) {
//                aux = ly_realloc(out->method.mem.buf, out->method.mem.len + count + 1);
//                if (!aux) {
//                    out->method.mem.buf = NULL;
//                    out->method.mem.len = 0;
//                    out->method.mem.size = 0;
//                    LOGMEM;
//                    return -1;
//                }
//                out->method.mem.buf = aux;
//                out->method.mem.size = out->method.mem.len + count + 1;
//            }
//            memcpy(&out->method.mem.buf[out->method.mem.len], buf, count + 1);
//            out->method.mem.len += count;
//            return count;
//        case LYOUT_CALLBACK:
//            return out->method.clb.f(out->method.clb.arg, buf, count);
//    }
//    
//    return 0;
//}


