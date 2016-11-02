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
#include <iostream>
#include <fstream>
#include <sstream>
#include <boost/log/trivial.hpp>


////////////////////////////////////////////////////////////////////
/// Function segmentalize()
////////////////////////////////////////////////////////////////////
std::vector<std::string>
ydk::core::segmentalize(const std::string& path)
{
    const std::string token {"/"};
    std::vector<std::string> output;
    size_t pos = std::string::npos; // size_t to avoid improbable overflow
    size_t prev_pos = 0;
    std::string data{path};
    do
    {
        pos = data.find(token);
        size_t first_quote_pos = data.find("'", prev_pos+1);
        size_t second_quote_pos = data.find("'", first_quote_pos+1);
        while((pos<second_quote_pos) && (pos>first_quote_pos))
        	pos = data.find(token, pos+1);
        output.push_back(data.substr(0, pos));
        if (std::string::npos != pos)
            data = data.substr(pos + token.size());
        prev_pos = pos;
    } while (std::string::npos != pos);
    return output;
}

////////////////////////////////////////////////////////////////////
/// ServiceProvider
///////////////////////////////////////////////////////////////////
ydk::core::ServiceProvider::~ServiceProvider()
{

}



/////////////////////////////////////////////////////////////////////////
namespace ydk {
    namespace core {

        template<typename T>
        void parse_range_intervals(LengthRangeIntervals<T>& intervals, const char* str_restr)
        {
            const char* seg_ptr = str_restr;
            while(1) {
                // min
                const char* ptr = seg_ptr;
                Range<T> range{intervals.default_range.min, intervals.default_range.max};

                if(ptr) {
                    // start processing min
                    while(isspace(ptr[0])) {
                        ++ptr;
                    }

                    if (isdigit(ptr[0]) || (ptr[0] == '+') ||  (ptr[0] == '-')) {
                        range.min = atoll(ptr);
                    if((ptr[0] == '+') || (ptr[0] == '-')) {
                        ++ptr;
                    }

                    while (isdigit(ptr[0])) {
                        ++ptr;
                    }

                    } else if (!strncmp(ptr, "min", 3)) {
                        ptr += 3;
                    } else if(!strncmp(ptr, "max", 3)) {
                        ptr += 3;
                    } else {
                        BOOST_LOG_TRIVIAL(error) << "Error parsing range " << str_restr;
                        throw YDKIllegalStateException{"Error parsing range"};
                    }

                    while (isspace(ptr[0])) {
                        ptr++;
                    }

                    //no interval or interval
                    if ((ptr[0] == '|' || !ptr[0])) {
                        range.max = atoll(ptr);

                    } else if( !strncmp(ptr, "..", 2)) {
                        // skip ..
                        ptr += 2;
                        while (isspace(ptr[0])) {
                            ++ptr;
                        }

                        // max
                        if (isdigit(ptr[0]) || (ptr[0] == '+') || (ptr[0] == '-')) {
                            range.max = atoll(ptr);
                        } else if (!strncmp(ptr, "max", 3)) {
                            // do nothing since max is already set

                        } else {
                            BOOST_LOG_TRIVIAL(error) << "Error parsing range " << str_restr;
                            throw YDKIllegalStateException{"Error parsing range"};
                        }
                    } else {
                        BOOST_LOG_TRIVIAL(error) << "Error parsing range " << str_restr;
                        throw YDKIllegalStateException{"Error parsing range"};
                    }
                    intervals.intervals.push_back(range);

                    /* next segment (next OR) */
                    seg_ptr = strchr(seg_ptr, '|');
                    if (!seg_ptr) {
                        break;
                    }
                    seg_ptr++;
                }
            }

        }


        SchemaValueIdentityType* create_identity_type(struct ly_set *ident)
        {

            SchemaValueIdentityType* identity_type = new SchemaValueIdentityType{};
            if(!ident) return identity_type;

            for(unsigned int i=0;i<ident->number && i<=272;i++) //272 temporary workaround for ietf-interfaces
            {
				identity_type->module_name = ident->set.s[i]->module->name;
				identity_type->name = ident->set.s[i]->name;
                                struct lys_ident* iden = ((struct lys_ident*)ident->set.s[i]);

                if(iden->der && iden->der->number > 0)
				{
					identity_type->derived.push_back(create_identity_type(iden->der));
				}

            }
            return identity_type;


        }



        SchemaValueIdentityType* create_identity_type(struct lys_ident **ident, int count)
        {

            SchemaValueIdentityType* identity_type = new SchemaValueIdentityType{};
            if(!ident) return identity_type;

            for(int i=0;i<count;i++)
            {
				identity_type->module_name = ident[i]->module->name;
				identity_type->name = ident[i]->name;

                if(ident[i]->der && ident[i]->der->number > 0)
				{
					identity_type->derived.push_back(create_identity_type(ident[i]->der));
				}

            }
            return identity_type;


        }


        SchemaValueType* create_schema_value_type(struct lys_node_leaf* leaf,
                                                  struct lys_type* type)
        {

            SchemaValueType* m_type = nullptr;

            LY_DATA_TYPE data_type = type->base;

            switch(data_type){
                case LY_TYPE_BINARY: {
                    if(type->info.binary.length){
                        SchemaValueBinaryType* binary = new SchemaValueBinaryType{};
                        m_type = binary;
                        parse_range_intervals(binary->length, type->info.binary.length->expr);

                    } else if(type->der){
                        m_type = create_schema_value_type(leaf, &(type->der->type));
                    } else {
                        SchemaValueBinaryType* binary = new SchemaValueBinaryType{};
                        m_type = binary;
                    }

                    break;
                    }
                case LY_TYPE_BITS: {
                    SchemaValueBitsType* bits = new SchemaValueBitsType{};
                    m_type = bits;
                    break;
                }
                case LY_TYPE_BOOL: {
                    SchemaValueBoolType* boolean = new SchemaValueBoolType{};
                    m_type = boolean;
                    break;
                }
                case LY_TYPE_DEC64: {
                    SchemaValueDec64Type* dec64 = new SchemaValueDec64Type{};
                    m_type = dec64;
                    break;
                }
                case LY_TYPE_EMPTY: {
                    SchemaValueEmptyType* empty = new SchemaValueEmptyType{leaf->name};
                    m_type = empty;
                    break;
                }
                case LY_TYPE_ENUM: {
                    if(type->info.enums.count > 0) {
                        SchemaValueEnumerationType* enum_type = new SchemaValueEnumerationType{};
                        m_type = enum_type;
                        for(int i=0; i<type->info.enums.count; i++){
                            SchemaValueEnumerationType::Enum enum_ {type->info.enums.enm[i].name, type->info.enums.enm[i].value};
                            enum_type->enums.push_back(enum_);
                        }
                    } else if(type->der){
                        m_type = create_schema_value_type(leaf, &(type->der->type));
                    } else {
                        BOOST_LOG_TRIVIAL(error) << "Unable to determine union's types: " << leaf->name <<", module: "<< leaf->module->name;
                        throw ydk::YDKIllegalStateException{"Unable to determine union's types"};
                    }
                    break;
                }
                case LY_TYPE_IDENT: {
                    if(type->info.ident.ref) {
                        m_type = create_identity_type(type->info.ident.ref, type->info.ident.count);
                    } else if(type->der){
                        m_type = create_schema_value_type(leaf, &(type->der->type));
                    } else {
                        BOOST_LOG_TRIVIAL(error) << "Unable to determine identity type: " << leaf->name <<", module: "<< leaf->module->name;
                        std::ostringstream os;
                        os << "Unable to determine identity type: " << leaf->name <<", module: "<< leaf->module->name;
                        throw ydk::YDKIllegalStateException{os.str()};
                    }
                    break;
                }
                case LY_TYPE_INST: {
                    SchemaValueInstanceIdType* instance_id = new SchemaValueInstanceIdType{};
                    m_type = instance_id;
                    break;
                }
                case LY_TYPE_LEAFREF: {
                    if(type->info.lref.target) {
                        m_type = create_schema_value_type(type->info.lref.target, &(type->info.lref.target->type));
                    } else if(type->der) {
                        m_type = create_schema_value_type(leaf, &(type->der->type));
                    } else {
                    	SchemaValueStringType* stringType = new SchemaValueStringType{};
                    	m_type = stringType; //TODO temporary workaround
//                        BOOST_LOG_TRIVIAL(error) << "Unable to determine leafref type: " << leaf->name <<", module: "<< leaf->module->name;
//                        std::ostringstream os;
//                        os << "Unable to determine leafref type: " << leaf->name <<", module: "<< leaf->module->name;
//                        throw ydk::YDKIllegalStateException{os.str()};
                    }
                    break;
                }
                case LY_TYPE_STRING: {
                    if(type->info.str.length) {
                        SchemaValueStringType* stringType = new SchemaValueStringType{};
                        m_type = stringType;
                        parse_range_intervals(stringType->length, type->info.str.length->expr);

                        if(type->info.str.pat_count != 0){
                            for(int i =0; i < type->info.str.pat_count; i++) {
                                stringType->patterns.push_back(type->info.str.patterns[i].expr);
                            }
                        }

                    } else if(type->der){
                        m_type = create_schema_value_type(leaf, &(type->der->type));
                    } else {
                        SchemaValueStringType* stringType = new SchemaValueStringType{};
                        m_type = stringType;

                        if(type->info.str.pat_count != 0){
                            for(int i=0; i < type->info.str.pat_count; i++) {
                                stringType->patterns.push_back(type->info.str.patterns[i].expr);
                            }
                        }

                    }
                    break;
                }
                case LY_TYPE_UNION: {

                    if(type->info.uni.count != 0) {
                        SchemaValueUnionType* unionType = new SchemaValueUnionType{};
                        m_type = unionType;
                        for(int i=0; i< type->info.uni.count; ++i) {
                            SchemaValueType* child_type =
                                create_schema_value_type(leaf,&(type->info.uni.types[i]));
                            unionType->types.push_back(child_type);
                        }
                    } else if(type->der){
                        m_type = create_schema_value_type(leaf, &(type->der->type));
                    } else {
                        BOOST_LOG_TRIVIAL(error) << "Unable to determine union's types: " << leaf->name <<", module: "<< leaf->module->name;
                        std::ostringstream os;
                        os<<"Unable to determine union's types: " << leaf->name <<", module: "<< leaf->module->name;
                        throw ydk::YDKIllegalStateException{os.str()};
                    }


                    break;
                }
                case LY_TYPE_INT8: {
                    if(type->info.num.range) {
                        SchemaValueNumberType<int8_t>* int8_type = new SchemaValueNumberType<int8_t>{ static_cast<int8_t>(-128),
                            static_cast<int8_t>(127) };
                        m_type = int8_type;
                        parse_range_intervals(int8_type->range, type->info.num.range->expr);
                    } else if(type->der) {
                        m_type = create_schema_value_type(leaf, &(type->der->type));
                    } else {
                        SchemaValueNumberType<int8_t>* int8_type = new SchemaValueNumberType<int8_t>{ static_cast<int8_t>(-128),
                            static_cast<int8_t>(127) };
                        m_type = int8_type;
                    }
                    break;
                }
                case LY_TYPE_UINT8:
                {
                    if(type->info.num.range) {
                        SchemaValueNumberType<uint8_t>* uint8_type = new SchemaValueNumberType<uint8_t>{ static_cast<uint8_t>(0),static_cast<uint8_t>(255) };
                        m_type = uint8_type;
                        parse_range_intervals(uint8_type->range, type->info.num.range->expr);
                    } else if(type->der) {
                        m_type = create_schema_value_type(leaf, &(type->der->type));
                    } else {
                        SchemaValueNumberType<uint8_t>* uint8_type = new SchemaValueNumberType<uint8_t>{ static_cast<uint8_t>(0),static_cast<uint8_t>(255) };
                        m_type = uint8_type;
                    }
                    break;
                }

                case LY_TYPE_INT16:
                {
                    if(type->info.num.range) {
                        SchemaValueNumberType<int16_t>* int16_type = new SchemaValueNumberType<int16_t>{ static_cast<int16_t>(-32768),static_cast<int16_t>(32767) };
                        m_type = int16_type;
                        parse_range_intervals(int16_type->range, type->info.num.range->expr);
                    } else if(type->der) {
                        m_type = create_schema_value_type(leaf, &(type->der->type));
                    } else {
                        SchemaValueNumberType<int16_t>* int16_type = new SchemaValueNumberType<int16_t>{ static_cast<int16_t>(-32768),static_cast<int16_t>(32767) };
                        m_type = int16_type;
                    }
                    break;
                }
                case LY_TYPE_UINT16:
                {
                    if(type->info.num.range) {
                        SchemaValueNumberType<uint16_t>* uint16_type = new SchemaValueNumberType<uint16_t>{ static_cast<uint16_t>(0),static_cast<uint16_t>(65535) };
                        m_type = uint16_type;
                        parse_range_intervals(uint16_type->range, type->info.num.range->expr);
                    } else if(type->der) {
                        m_type = create_schema_value_type(leaf, &(type->der->type));
                    } else {
                        SchemaValueNumberType<uint16_t>* uint16_type = new SchemaValueNumberType<uint16_t>{ static_cast<uint16_t>(0),static_cast<uint16_t>(65535) };
                        m_type = uint16_type;
                    }
                    break;
                }
                case LY_TYPE_INT32:
                {
                    if(type->info.num.range) {
                        SchemaValueNumberType<int32_t>* int32_type = new SchemaValueNumberType<int32_t>{ static_cast<int32_t>(-2147483648),static_cast<int32_t>(2147483647) };
                        m_type = int32_type;
                        parse_range_intervals(int32_type->range, type->info.num.range->expr);
                    } else if(type->der) {
                        m_type = create_schema_value_type(leaf, &(type->der->type));
                    } else {
                        SchemaValueNumberType<int32_t>* int32_type = new SchemaValueNumberType<int32_t>{ static_cast<int32_t>(-2147483648),static_cast<int32_t>(2147483647) };
                        m_type = int32_type;
                    }
                    break;
                }
                case LY_TYPE_UINT32:
                {
                    if(type->info.num.range) {
                        SchemaValueNumberType<uint32_t>* uint32_type = new SchemaValueNumberType<uint32_t>{ static_cast<uint32_t>(0),static_cast<uint32_t>(4294967295) };
                        m_type = uint32_type;
                        parse_range_intervals(uint32_type->range, type->info.num.range->expr);
                    } else if(type->der) {
                        m_type = create_schema_value_type(leaf, &(type->der->type));
                    } else {
                        SchemaValueNumberType<uint32_t>* uint32_type = new SchemaValueNumberType<uint32_t>{ static_cast<uint32_t>(0),static_cast<uint32_t>(4294967295) };
                        m_type = uint32_type;
                    }
                    break;
                }

                case LY_TYPE_INT64:
                {
                    if(type->info.num.range) {
                        SchemaValueNumberType<int64_t>* int64_type = new SchemaValueNumberType<int64_t>{ static_cast<int64_t>(-9223372036854775807),static_cast<int64_t>(9223372036854775807) };
                        m_type = int64_type;
                        parse_range_intervals(int64_type->range, type->info.num.range->expr);
                    } else if(type->der) {
                        m_type = create_schema_value_type(leaf, &(type->der->type));
                    } else {
                        SchemaValueNumberType<int64_t>* int64_type = new SchemaValueNumberType<int64_t>{ static_cast<int64_t>(-9223372036854775807),static_cast<int64_t>(9223372036854775807) };
                        m_type = int64_type;
                    }
                    break;
                }
                case LY_TYPE_UINT64: {
                    if(type->info.num.range) {
                        SchemaValueNumberType<uint64_t>* uint64_type = new SchemaValueNumberType<uint64_t>{ static_cast<uint64_t>(0),static_cast<uint64_t>(18446744073709551615ULL) };
                        m_type = uint64_type;
                        parse_range_intervals(uint64_type->range, type->info.num.range->expr);
                    } else if(type->der) {
                        m_type = create_schema_value_type(leaf, &(type->der->type));
                    } else {
                        SchemaValueNumberType<uint64_t>* uint64_type = new SchemaValueNumberType<uint64_t>{ static_cast<uint64_t>(0),static_cast<uint64_t>(18446744073709551615ULL) };
                        m_type = uint64_type;
                    }
                    break;
                }
                default:
                    BOOST_LOG_TRIVIAL(error) << "Unknown type to process for schema: " << leaf->name <<", module: "<< leaf->module->name;
                    std::ostringstream os;
                    os<<"Unknown type to process for schema: " << leaf->name <<", module: "<< leaf->module->name;
                    throw YDKIllegalStateException{os.str()};

            }

            return m_type;
        }

        SchemaValueType* create_schema_value_type(struct lys_node_leaf* leaf)
        {
            return create_schema_value_type(leaf, &(leaf->type));
        }


    }
}

//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// class ydk::ValidationService
//////////////////////////////////////////////////////////////////////////
void
ydk::core::ValidationService::validate(const ydk::core::DataNode* dn, ydk::core::ValidationService::Option option)
{
    std::string option_str = "";
    int ly_option = 0;
    switch(option) {
        case ValidationService::Option::DATASTORE:
            option_str="DATATSTORE";
            ly_option = LYD_OPT_CONFIG;
            break;
        case ValidationService::Option::EDIT_CONFIG:
            option_str="EDIT-CONFIG";
            ly_option = LYD_OPT_EDIT;
            break;
        case ValidationService::Option::GET:
            option_str="GET";
            ly_option = LYD_OPT_GET;
            break;
        case ValidationService::Option::GET_CONFIG:
            option_str="GET-CONFIG";
            ly_option = LYD_OPT_GETCONFIG;
            break;

    }
    ly_option = ly_option | LYD_OPT_NOAUTODEL;

    BOOST_LOG_TRIVIAL(debug) << "Validation called on " << dn->path() << " with option " << option_str;

    //what kind of a DataNode is this
    const ydk::core::DataNodeImpl* dn_impl = dynamic_cast<const ydk::core::DataNodeImpl*>(dn);
    if(dn_impl){
        struct lyd_node* lynode = dn_impl->m_node;
        int rc = lyd_validate(&lynode,ly_option, NULL);
        if(rc) {
            BOOST_LOG_TRIVIAL(error) << "Data validation failed";
            throw ydk::core::YDKDataValidationException{};
        }

    } else {
        BOOST_LOG_TRIVIAL(error) << "Cast of DataNode to impl failed!!";
        throw YDKIllegalStateException{"Illegal state"};
    }

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

    const DataNodeImpl* impl = dynamic_cast<const DataNodeImpl *>(dn);
    if( !impl) {
        BOOST_LOG_TRIVIAL(error) << "DataNode is nullptr";
        throw YDKCoreException{"DataNode is null"};
    }
    m_node = impl->m_node;

    if(m_node == nullptr){
        throw YDKInvalidArgumentException{"No data in data node"};
    }
    char* buffer;
    BOOST_LOG_TRIVIAL(trace) << "Performing encode operation";

    if(!lyd_print_mem(&buffer, m_node,scheme, (pretty ? LYP_FORMAT : 0)|LYP_WD_ALL|LYP_KEEPEMPTYCONT)) {
    	if(!buffer)
    	{
    		std::ostringstream os;
    		os << "Could not encode datanode: "<< m_node->schema->name;
			BOOST_LOG_TRIVIAL(error) << os.str();
			throw YDKCoreException{os.str()};
    	}
        ret = buffer;
        std::free(buffer);
    }

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
        BOOST_LOG_TRIVIAL(error) << "Root Schema Node is nullptr";
        throw YDKCoreException{"Root Schema Node is null"};
    }

    struct lyd_node *root = lyd_parse_mem(rs_impl->m_ctx, buffer.c_str(), scheme, LYD_OPT_TRUSTED |  LYD_OPT_GET);
    if( root == nullptr || ly_errno ) {

        BOOST_LOG_TRIVIAL(error) << "Parsing failed with message " << ly_errmsg();
        throw YDKCodecException{YDKCodecException::Error::XML_INVAL};
    }
    BOOST_LOG_TRIVIAL(trace) << "Performing decode operation";
    RootDataImpl* rd = new RootDataImpl{rs_impl, rs_impl->m_ctx, "/"};
    rd->m_node = root;

    struct lyd_node* dnode = root;
    do{
        DataNodeImpl* nodeImpl = new DataNodeImpl{rd, dnode};
        rd->child_map.insert(std::make_pair(root, nodeImpl));
        dnode = dnode->next;
    } while(dnode && dnode != nullptr && dnode != root);

    return rd;
}

///////////////////////////////////////////////////////////////////////////


