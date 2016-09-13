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

namespace fs = boost::filesystem;
namespace io = boost::iostreams;

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
/// YDKCoreException
/////////////////////////////////////////////////////////////////////////
ydk::core::YDKCoreException::YDKCoreException(const std::string& msg) : ydk::YDKException{msg}
{

}

//////////////////////////////////////////////////////////////////////////
/// YDKIllegalStateException
//////////////////////////////////////////////////////////////////////////
ydk::YDKIllegalStateException::YDKIllegalStateException(const std::string& msg) : ydk::YDKException{msg}
{

}

//////////////////////////////////////////////////////////////////////////
/// YDKInvalidArgumentException
//////////////////////////////////////////////////////////////////////////
ydk::YDKInvalidArgumentException::YDKInvalidArgumentException(const std::string& msg) : ydk::YDKException{msg}
{

}

//////////////////////////////////////////////////////////////////////////
/// YDKOperationNotSupportedException
//////////////////////////////////////////////////////////////////////////
ydk::YDKOperationNotSupportedException::YDKOperationNotSupportedException(const std::string& msg) : ydk::YDKException{msg}
{

}

//////////////////////////////////////////////////////////////////////////
/// YDKDataValidationException
//////////////////////////////////////////////////////////////////////////
ydk::core::YDKDataValidationException::YDKDataValidationException() : ydk::core::YDKCoreException{"Data Validation Exception"}
{

}

//////////////////////////////////////////////////////////////////////////
/// YDKPathException
//////////////////////////////////////////////////////////////////////////
ydk::core::YDKPathException::YDKPathException(ydk::core::YDKPathException::Error error_code) : ydk::core::YDKCoreException{"Data Validation Exception"}, err{error_code}
{

}



/////////////////////////////////////////////////////////////////////////
/// YDKCodecException
/////////////////////////////////////////////////////////////////////////
ydk::core::YDKCodecException::YDKCodecException(YDKCodecException::Error ec) : YDKCoreException(ly_errmsg()), err{ec}
{

}

////////////////////////////////////////////////////////////////////////////
/// Annotation
///////////////////////////////////////////////////////////////////////////
ydk::core::Annotation::Annotation(const std::string& ns, const std::string& name, const std::string& val) : m_ns{ns}, m_name{name}, m_val{val}
{

}

ydk::core::Annotation::Annotation(const Annotation& an) : m_ns{an.m_ns}, m_name{an.m_name}, m_val{an.m_val}
{

}

ydk::core::Annotation::Annotation(Annotation&& an) : m_ns{std::move(an.m_ns)}, m_name{std::move(an.m_name)}, m_val{std::move(an.m_val)}
{

}

ydk::core::Annotation&
ydk::core::Annotation::operator=(const ydk::core::Annotation& an)
{
    m_ns = an.m_ns;
    m_name = an.m_name;
    m_val = an.m_val;

    return *this;
}

ydk::core::Annotation&
ydk::core::Annotation::operator=(ydk::core::Annotation&& an)
{
    m_ns = std::move(an.m_ns);
    m_name = std::move(an.m_name);
    m_val = std::move(an.m_val);

    return *this;
}

bool
ydk::core::Annotation::operator==(const Annotation& an) const
{
    return m_ns == an.m_ns && m_name == an.m_name;
}

///////////////////////////////////////////////////////////////////////////
/// Statement
///////////////////////////////////////////////////////////////////////////

ydk::core::Statement::Statement(): keyword{}, arg{}
{

}

ydk::core::Statement::Statement(const std::string& mkeyword, const std::string& marg) : keyword{mkeyword}, arg{marg}
{

}

ydk::core::Statement::Statement(const ydk::core::Statement& stmt): keyword{stmt.keyword}, arg{stmt.arg}
{

}

ydk::core::Statement::Statement(ydk::core::Statement&& stmt): keyword{std::move(stmt.keyword)}, arg{std::move(stmt.arg)}
{

}

ydk::core::Statement::~Statement()
{

}

ydk::core::Statement&
ydk::core::Statement::operator=(const ydk::core::Statement& stmt)
{
    keyword = stmt.keyword;
    arg = stmt.arg;
    return *this;
}

ydk::core::Statement&
ydk::core::Statement::operator=(ydk::core::Statement&& stmt)
{
    keyword = std::move(stmt.keyword);
    arg = std::move(stmt.arg);
    return *this;
}

///////////////////////////////////////////////////////////////////////////////////////////
/// SchemaValueType
///////////////////////////////////////////////////////////////////////////////////////////

ydk::core::SchemaValueType::~SchemaValueType()
{

}



/////////////////////////////////////////////////////////////////////////////
// ydk::core::SchemaValueBinaryType
/////////////////////////////////////////////////////////////////////////////

ydk::core::SchemaValueBinaryType::SchemaValueBinaryType(): length{Range<uint64_t>{0,18446744073709551615UL}}
{
    
}



ydk::core::SchemaValueBinaryType::~SchemaValueBinaryType()
{

}


ydk::core::DiagnosticNode<std::string, ydk::core::ValidationError>
ydk::core::SchemaValueBinaryType::validate(const std::string& value) const
{
    DiagnosticNode<std::string, ydk::core::ValidationError> diag {};

    if(value.empty()){
        diag.errors.push_back(ydk::core::ValidationError::INVALATTR);
    }
    return diag;
};



///////////////////////////////////////////////////////////////////////////////////
/// ydk::core::SchemaValueBitsType::Bit
//////////////////////////////////////////////////////////////////////////////////
ydk::core::SchemaValueBitsType::Bit::Bit(std::string m_name, uint32_t m_pos) : name{m_name} ,
pos{m_pos} {

}


namespace ydk {
    namespace core {
        static std::vector<std::string> split(const std::string &s, char delim) {
            std::stringstream ss(s);
            std::string item;
            std::vector<std::string> tokens;
            while (std::getline(ss, item, delim)) {
                tokens.push_back(item);
            }
            return tokens;
        }
    }
}


////////////////////////////////////////////////////////////////////
// ydk::core::SchemaValueBitsType
////////////////////////////////////////////////////////////////////////

ydk::core::SchemaValueBitsType::~SchemaValueBitsType()
{

}

ydk::core::DiagnosticNode<std::string, ydk::core::ValidationError>
ydk::core::SchemaValueBitsType::validate(const std::string& value) const
{
    DiagnosticNode<std::string, ydk::core::ValidationError> diag {};

    if(value.empty()){
        BOOST_LOG_TRIVIAL(debug) << "Empty attribute error for SchemaValueBits";
        diag.errors.push_back(ydk::core::ValidationError::INVALATTR);
    } else {
        //tokenize and make sure all tokens are accounted for
        auto tokens = ydk::core::split(value, ' ');
        std::map<std::string, ydk::core::SchemaValueBitsType::Bit*> name_bit_map{};
        for(auto bit : bits){
            name_bit_map.insert(std::make_pair(bit.name,&bit));
        }
        for(auto token : tokens) {
            if(name_bit_map.find(token) == name_bit_map.end()){
                BOOST_LOG_TRIVIAL(debug) << "Invalid bits value " << value;
                diag.errors.push_back(ydk::core::ValidationError::INVALATTR);
            }
        }

    }
    return diag;
}

////////////////////////////////////////////////////////////////////
// ydk::core::SchemaValueDec64Type
////////////////////////////////////////////////////////////////////////

ydk::core::SchemaValueDec64Type::~SchemaValueDec64Type()
{

}


ydk::core::DiagnosticNode<std::string, ydk::core::ValidationError>
ydk::core::SchemaValueDec64Type::validate(const std::string& value) const
{
    DiagnosticNode<std::string, ydk::core::ValidationError> diag {};

    if(value.empty()){
        BOOST_LOG_TRIVIAL(debug) << "Empty attribute error for SchemaValueDec64Type";
        diag.errors.push_back(ydk::core::ValidationError::INVALATTR);
    }
    return diag;
}

//////////////////////////////////////////////////////////////////////
/// ydk::core::SchemaValueEnumerationType::Enum
/////////////////////////////////////////////////////////////////////
ydk::core::SchemaValueEnumerationType::Enum::Enum(std::string m_name, int32_t m_value) : name{m_name}, value{m_value}
{

}



////////////////////////////////////////////////////////////////////
// ydk::core::SchemaValueEnumerationType
////////////////////////////////////////////////////////////////////////

ydk::core::SchemaValueEnumerationType::~SchemaValueEnumerationType()
{

}

ydk::core::DiagnosticNode<std::string, ydk::core::ValidationError>
ydk::core::SchemaValueEnumerationType::validate(const std::string& value) const
{
    DiagnosticNode<std::string, ydk::core::ValidationError> diag {};

    if(value.empty()){
         BOOST_LOG_TRIVIAL(debug) << "Empty attribute error for SchemaValueEnumerationType";
        diag.errors.push_back(ydk::core::ValidationError::INVALATTR);

    } else {

        for(auto e : enums){
            if(e.name == value){
                return diag;
            }
        }
    }
     BOOST_LOG_TRIVIAL(debug) << "Invalid enum value " << value;
    diag.errors.push_back(ydk::core::ValidationError::INVALATTR);

    return diag;
}

////////////////////////////////////////////////////////////////////
// ydk::core::SchemaValueIdentityType
////////////////////////////////////////////////////////////////////////

ydk::core::SchemaValueIdentityType::~SchemaValueIdentityType()
{
    for(auto identity : derived) {
        delete identity;
    }
}

ydk::core::DiagnosticNode<std::string, ydk::core::ValidationError>
ydk::core::SchemaValueIdentityType::validate(const std::string& value) const
{
    DiagnosticNode<std::string, ydk::core::ValidationError> diag {};

    if(value.empty()){
         BOOST_LOG_TRIVIAL(debug) << "Empty attribute error for SchemaValueIdentityType";
        diag.errors.push_back(ydk::core::ValidationError::INVALATTR);
        return diag;
    }
    
    auto tokens = ydk::core::split(value, ':');
    if(tokens.size() == 1) {
        //no module name just compare the name
        if(tokens[0] == name) {
            return diag;
        }
    } else if(tokens[0] == module_name && tokens[1] == name){
            return diag;
    }
    
    for(auto ident : derived) {
        if(!ident->validate(value).has_errors()){
            return diag;
        }
    }
    
    BOOST_LOG_TRIVIAL(debug) << "Invalid identity" << value;
    diag.errors.push_back(ydk::core::ValidationError::INVALID_IDENTITY);
    return diag;
}

////////////////////////////////////////////////////////////////////
// ydk::core::SchemaValueInstanceIdType
////////////////////////////////////////////////////////////////////////

ydk::core::SchemaValueInstanceIdType::~SchemaValueInstanceIdType()
{

}

ydk::core::DiagnosticNode<std::string, ydk::core::ValidationError>
ydk::core::SchemaValueInstanceIdType::validate(const std::string& value) const
{
    DiagnosticNode<std::string, ydk::core::ValidationError> diag {};

    if(value.empty()){
        BOOST_LOG_TRIVIAL(debug) << "Empty attribute error for SchemaValueInstanceIdType";
        diag.errors.push_back(ydk::core::ValidationError::INVALATTR);
    }
    
    
    return diag;
}

////////////////////////////////////////////////////////////////////
// ydk::core::SchemaValueStringType
////////////////////////////////////////////////////////////////////////

ydk::core::SchemaValueStringType::SchemaValueStringType(): length{Range<uint64_t>{0,18446744073709551615UL}}
{
    
}


ydk::core::SchemaValueStringType::~SchemaValueStringType()
{

}

ydk::core::DiagnosticNode<std::string, ydk::core::ValidationError>
ydk::core::SchemaValueStringType::validate(const std::string& value) const
{
    DiagnosticNode<std::string, ydk::core::ValidationError> diag {};

    if(value.empty()){
        BOOST_LOG_TRIVIAL(debug) << "Empty attribute error for SchemaStringType";
        diag.errors.push_back(ydk::core::ValidationError::INVALATTR);
    }

    /// first do a length check
    auto size = value.length();
    if(length.intervals.empty()){
        if(size < length.default_range.min || size > length.default_range.max) {
            BOOST_LOG_TRIVIAL(debug) << "Invalid length for string size is " << size;
            diag.errors.push_back(ValidationError::INVALID_LENGTH);
            
        }
    } else {
        bool constraint_satisfied = false;
        for(auto interval : length.intervals) {
            if( size >= interval.min && size <= interval.max) {
                constraint_satisfied = true;
                break;
            }
        }
        
        if(!constraint_satisfied){
            BOOST_LOG_TRIVIAL(debug) << "Invalid length for string size is " << size;
            diag.errors.push_back(ValidationError::INVALID_LENGTH);
        }
    }
    
    
    /// then a pattern check
    /// all patterns have to be matched
    for(auto p : patterns) {
        std::regex r {p};
        if(!std::regex_match(p, r)){
           BOOST_LOG_TRIVIAL(debug) << "String " << value << " failed pattern " << p << " match";
            diag.errors.push_back(ValidationError::INVALID_PATTERN);
        }
    }
    
    return diag;
}

////////////////////////////////////////////////////////////////////
// ydk::core::SchemaValueUnionType
////////////////////////////////////////////////////////////////////////


ydk::core::SchemaValueUnionType::~SchemaValueUnionType()
{
    for(auto type : types) {
        delete type;
    }
}


ydk::core::DiagnosticNode<std::string, ydk::core::ValidationError>
ydk::core::SchemaValueUnionType::validate(const std::string& value) const
{

    for(auto type : types){
        auto diag = type->validate(value);
        if(!diag.has_errors()){
            return diag;
        }
    }

    DiagnosticNode<std::string, ydk::core::ValidationError> diag{};
    BOOST_LOG_TRIVIAL(debug) << "Union type validation failed for value " << value;
    diag.errors.push_back(ValidationError::INVALATTR);

    return diag;
}

////////////////////////////////////////////////////////////////////
// ydk::core::SchemaValueEmptyType
////////////////////////////////////////////////////////////////////////

ydk::core::SchemaValueEmptyType::SchemaValueEmptyType(const std::string& mleaf_name) : leaf_name{mleaf_name}
{
    
}


ydk::core::SchemaValueEmptyType::~SchemaValueEmptyType()
{

}

ydk::core::DiagnosticNode<std::string, ydk::core::ValidationError>
ydk::core::SchemaValueEmptyType::validate(const std::string& value) const
{
    DiagnosticNode<std::string, ydk::core::ValidationError> diag {};

    if(value.empty()){
        BOOST_LOG_TRIVIAL(debug) << "Value is empty for SchemaValueEmptyType" ;
        diag.errors.push_back(ydk::core::ValidationError::INVALID_EMPTY_VAL);
    } else if(value != leaf_name){
        BOOST_LOG_TRIVIAL(debug) << "Mismatch between leaf name " << leaf_name << " and value " << value;
        diag.errors.push_back(ydk::core::ValidationError::INVALID_EMPTY_VAL);
    }
    return diag;
}

////////////////////////////////////////////////////////////////////
// ydk::core::SchemaValueBoolType
////////////////////////////////////////////////////////////////////////

ydk::core::SchemaValueBoolType::~SchemaValueBoolType()
{

}


ydk::core::DiagnosticNode<std::string, ydk::core::ValidationError>
ydk::core::SchemaValueBoolType::validate(const std::string& value) const
{
    DiagnosticNode<std::string, ydk::core::ValidationError> diag {};

    if(value.empty()){
        BOOST_LOG_TRIVIAL(debug) << "Value is empty for SchemaValueBoolType" ;
        diag.errors.push_back(ydk::core::ValidationError::INVALATTR);
    } else {

        if(value != "true" && value != "false") {
            BOOST_LOG_TRIVIAL(debug) << "Invalid boolean value " << value;
            diag.errors.push_back(ValidationError::INVALID_BOOL_VAL);
        }
    }

    return diag;
}


///////////////////////////////////////////////////////////////////////////////
/// SchemaNode
///////////////////////////////////////////////////////////////////////////////
ydk::core::SchemaNode::~SchemaNode()
{

}

//////////////////////////////////////////////////////////////////////////////
/// RootSchemaNode
/////////////////////////////////////////////////////////////////////////////
ydk::core::RootSchemaNode::~RootSchemaNode()
{

}

std::string
ydk::core::RootSchemaNode::path() const
{
    return "/";
}

ydk::core::SchemaNode*
ydk::core::RootSchemaNode::parent() const noexcept
{
    return nullptr;
}

const ydk::core::SchemaNode*
ydk::core::RootSchemaNode::root() const noexcept
{
    return this;
}

ydk::core::Statement
ydk::core::RootSchemaNode::statement() const
{
    return Statement{};
}

std::vector<ydk::core::Statement>
ydk::core::RootSchemaNode::keys() const
{
    return std::vector<Statement>{};

}

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

/////////////////////////////////////////////////////////////////////////////
/// Capability
/////////////////////////////////////////////////////////////////////////////

ydk::core::Capability::Capability(const std::string& mod, const std::string& rev): module{mod}, revision{rev}
{

}

ydk::core::Capability::Capability(const std::string& mod, const std::string& rev, const std::vector<std::string>& f,
           const std::vector<std::string>& d): module{mod}, revision{rev}, features{f}, deviations({d})
{

}

ydk::core::Capability::Capability(const Capability& cap): module{cap.module}, revision{cap.revision}, features{cap.features}, deviations{cap.deviations}
{

}

ydk::core::Capability::Capability(ydk::core::Capability&& cap): module{std::move(cap.module)}, revision{std::move(cap.revision)},
features{std::move(cap.features)}, deviations{std::move(cap.deviations)}
{

}

ydk::core::Capability&
ydk::core::Capability::operator=(const ydk::core::Capability& cap)
{
    module = cap.module;
    revision = cap.revision;
    features = cap.features;
    deviations = cap.deviations;

    return *this;
}


ydk::core::Capability&
ydk::core::Capability::operator=(ydk::core::Capability&& cap)
{
    module = std::move(cap.module);
    revision = std::move(cap.revision);
    features = std::move(cap.features);
    deviations = std::move(cap.deviations);

    return *this;
}

bool
ydk::core::Capability::operator==(const ydk::core::Capability& cap)
{
    if( cap.module != module || cap.revision != revision ) {
        return false;
    }

    if (cap.features.size() != features.size()){
        return false;
    } else if(cap.features.size() != 0){

        //sort and compare
        std::vector<std::string> cap_features{cap.features};
        std::sort(cap_features.begin(), cap_features.end());

        //sort ourselves
        std::sort(features.begin(), features.end());

        if(cap_features != features) {
            return false;
        }

    }

    if (cap.deviations.size() != deviations.size()) {
        return false;
    } else if(cap.deviations.size() != 0){

        //sort and compare
        std::vector<std::string> cap_deviations{cap.deviations};
        std::sort(cap_deviations.begin(), cap_deviations.end());

        //sort ourselves
        std::sort(deviations.begin(), deviations.end());

        if(cap_deviations != deviations) {
            return false;
        }

    }

    return true;
}

////////////////////////////////////////////////////////////////////
/// ServiceProvider
///////////////////////////////////////////////////////////////////
ydk::core::ServiceProvider::~ServiceProvider()
{

}

/////////////////////////////////////////////////////////////////////////
/// Rpc
////////////////////////////////////////////////////////////////////////
ydk::core::Rpc::~Rpc()
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
        
        
        SchemaValueIdentityType* create_identity_type(struct lys_ident *ident)
        {
            
            SchemaValueIdentityType* identity_type = new SchemaValueIdentityType{};
            
            identity_type->module_name = ident->module->name;
            identity_type->name = ident->name;
            
            if(ident->der) {
                struct lys_ident *der;
                int i = 0;
                for(der = ident->der[i];ident->der[i]; der = ident->der[++i]) {
                    identity_type->derived.push_back(create_identity_type(der));
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
                        BOOST_LOG_TRIVIAL(error) << "Unable to determine union's types"; 
                        throw ydk::YDKIllegalStateException{"Unable to determine union's types"};
                    }
                    break;
                }
                case LY_TYPE_IDENT: {
                    if(type->info.ident.ref) {
                        m_type = create_identity_type(type->info.ident.ref);
                    } else if(type->der){
                        m_type = create_schema_value_type(leaf, &(type->der->type));
                    } else {
                        BOOST_LOG_TRIVIAL(error) << "Unable to determine identity type";
                        throw ydk::YDKIllegalStateException{"Unable to determine identity type"};
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
                        BOOST_LOG_TRIVIAL(error) << "Unable to determine leafref type";
                        throw ydk::YDKIllegalStateException{"Unable to determine leafref type"};
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
                        BOOST_LOG_TRIVIAL(error) << "Unable to determine union's types";
                        throw ydk::YDKIllegalStateException{"Unable to determine union's types"};
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
                    BOOST_LOG_TRIVIAL(error) << "Unknown type to process for schema";
                    throw YDKIllegalStateException{"Unknown type to process for schema"};

            }

            return m_type;
        }

        SchemaValueType* create_schema_value_type(struct lys_node_leaf* leaf)
        {
            return create_schema_value_type(leaf, &(leaf->type));
        }


    }
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
        BOOST_LOG_TRIVIAL(debug) << "path is empty";
        throw YDKInvalidArgumentException{"path is empty"};
    }
    
    //has to be a relative path
    if(path.at(0) == '/') {
        BOOST_LOG_TRIVIAL(debug) << "path must be a relative path";
        throw YDKInvalidArgumentException{"path must be a relative path"};
    }
    
    std::vector<SchemaNode*> ret;

    std::string full_path{"/"};
    full_path+=path;
    
    const struct lys_node* found_node = ly_ctx_get_node(m_ctx, nullptr, full_path.c_str());
    
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
    auto c = find(path);
    if(c.empty()){
        throw YDKInvalidArgumentException{"Path is invalid"};
    }

    bool found = false;
    SchemaNode* rpc_sn = nullptr;

    for(auto item : c) {
        auto s = item->statement();
        if(s.keyword == "rpc"){
            found = true;
            rpc_sn = item;
            break;
        }
    }

    if(!found){
        BOOST_LOG_TRIVIAL(debug) << "Path " << path << " does not refer to an rpc node.";
        throw YDKInvalidArgumentException{"Path does not refer to an rpc node"};
    }
    SchemaNodeImpl* sn = dynamic_cast<SchemaNodeImpl*>(rpc_sn);
    if(!sn){
        BOOST_LOG_TRIVIAL(error) << "Schema Node case failed";
        throw YDKIllegalStateException("Internal error occurred");
    }
    return new RpcImpl{sn, m_ctx};

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
		throw YDKInvalidArgumentException{"invalid path"};
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
        int rc = lyd_validate(&lynode,ly_option);
        if(rc) {
            BOOST_LOG_TRIVIAL(debug) << "Data validation failed";
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
        BOOST_LOG_TRIVIAL(debug) << "DataNode is nullptr";
        throw YDKCoreException{"DataNode is null"};
    }
    m_node = impl->m_node;


    if(m_node == nullptr){
        throw YDKInvalidArgumentException{"No data in data node"};
    }
    char* buffer;

    if(!lyd_print_mem(&buffer, m_node,scheme, pretty ? LYP_FORMAT : 0)) {
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
        BOOST_LOG_TRIVIAL(debug) << "Root Schema Node is nullptr";
        throw YDKCoreException{"Root Schema Node is null"};
    }

    struct lyd_node *root = lyd_parse_mem(rs_impl->m_ctx, buffer.c_str(), scheme, LYD_OPT_TRUSTED |  LYD_OPT_KEEPEMPTYCONT | LYD_WD_TRIM | LYD_OPT_GET);
    if( ly_errno ) {
     
        BOOST_LOG_TRIVIAL(debug) << "Parsing failed with message " << ly_errmsg();
        throw YDKCodecException{YDKCodecException::Error::XML_INVAL};
    }

    RootDataImpl* rd = new RootDataImpl{rs_impl, rs_impl->m_ctx, "/"};
    rd->m_node = root;

    struct lyd_node* dnode = root;
    do{
        DataNodeImpl* nodeImpl = new DataNodeImpl{rd, dnode};
        rd->child_map.insert(std::make_pair(root, nodeImpl));
        dnode = dnode->next;
    } while(dnode != nullptr && dnode != root);

    return rd;
}

///////////////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////////////
// class ydk::Repository
//////////////////////////////////////////////////////////////////////////

ydk::core::Repository::Repository()
{
    path = fs::temp_directory_path();
}


ydk::core::Repository::Repository(const std::string& search_dir) : path{search_dir}
{
    
    if(!fs::exists(path) || !fs::is_directory(path)) {
        BOOST_LOG_TRIVIAL(debug) << "Path " << search_dir << " is not a valid directory.";
        throw YDKInvalidArgumentException{"path is not a valid directory"};
    }
    
    ly_verb(LY_LLSILENT);
}


namespace ydk {
    namespace core {
        
        extern "C" void cpp_free_data(void *model_data)
        {
            delete [] static_cast<char*>(model_data);
        }
        
        extern "C" void c_free_data(void *model_data)
        {
            std::free(model_data);
        }
        
        char* convert_str_to_buffer(const std::string& str)
        {
            auto data = str.c_str();
            auto length = std::strlen(data);
            char* copy = new char[length+1];
            strcpy(copy, data);
            copy[length] = '\0';
            BOOST_LOG_TRIVIAL(trace) << "String to buffer:-" << copy;
            return copy;
            
        }
        
        void sink_to_file(const std::string& filename, const std::string& contents)
        {
            std::ofstream sink {filename, std::ios::binary};
            if (sink.is_open()) {
                sink << contents ;
                sink.close();
            } else {
                BOOST_LOG_TRIVIAL(debug) << "Cannot sink to file " << filename;
            }
        }
    
        extern "C" char* get_module_callback(const char* name, const char* revision, void* user_data,
                                       LYS_INFORMAT* format, void (**free_module_data)(void *model_data))
        {
            BOOST_LOG_TRIVIAL(debug) << "Getting module " << name;
            
            if(user_data != nullptr){
                ModelProvider::Format m_format = ModelProvider::Format::YANG;
                *format = LYS_IN_YANG;
                auto pair = reinterpret_cast<std::pair<struct ly_ctx* , const ydk::core::Repository*>*>(user_data);
                struct ly_ctx* ctx = pair->first;
                const Repository* repo = pair->second;
                
                //check if the module is avaliable
                const struct lys_module* module = ly_ctx_get_module(ctx, name, revision);
                if(module != nullptr){
                    char* strp = nullptr;
                    if(!lys_print_mem(&strp, module, LYS_OUT_YANG, nullptr)){
                        *free_module_data = nullptr;
                        return strp;
                    }
                    
                }
                
                //first check our directory for a file of the form <module-name>@<revision-date>.yang
                
                std::string yang_file_path_str{repo->path.string()};
                yang_file_path_str += '/';
                yang_file_path_str += name;
                if(revision){
                    yang_file_path_str += "@";
                    yang_file_path_str += revision;
                }
                yang_file_path_str += ".yang";
                
                fs::path yang_file_path{yang_file_path_str};
                if(fs::is_regular_file(yang_file_path)) {
                    //open the file read the data and return it
                    std::string model_data {""};
                    std::ifstream yang_file {yang_file_path.string()};
                    if(yang_file.is_open()) {
                        std::string line;
                        while(std::getline(yang_file, line)){
                            model_data+=line;
                            model_data+='\n';
                        }
                        
                        yang_file.close();
                        
                        *free_module_data = c_free_data;
                        char *enlarged_data = nullptr;
                        /* enlarge data by 2 bytes for flex */
                        auto data = model_data.c_str();
                        auto len = std::strlen(data);
                        enlarged_data = static_cast<char*>(std::malloc((len + 2) * sizeof *enlarged_data));
                        if (!enlarged_data) {
                            throw std::bad_alloc{};
                        }
                        memcpy(enlarged_data, data, len);
                        enlarged_data[len] = enlarged_data[len + 1] = '\0';
                        return enlarged_data;
                    } else {
                        BOOST_LOG_TRIVIAL(debug) << "Cannot open file " << yang_file_path_str;
                        throw YDKIllegalStateException("Cannot open file");
                    }
                    
                }

                
                for(auto model_provider : repo->get_model_providers()) {
                    std::string model_data = model_provider->get_model(name, revision != nullptr ? revision : "", m_format);
                    if(!model_data.empty()){
                       
                        sink_to_file(yang_file_path_str, model_data);
                        *free_module_data = c_free_data;
                        char *enlarged_data = nullptr;
                        /* enlarge data by 2 bytes for flex */
                        auto data = model_data.c_str();
                        auto len = std::strlen(data);
                        enlarged_data = static_cast<char*>(std::malloc((len + 2) * sizeof *enlarged_data));
                        if (!enlarged_data) {
                            throw std::bad_alloc{};
                        }
                        memcpy(enlarged_data, data, len);
                        enlarged_data[len] = enlarged_data[len + 1] = '\0';
                        return enlarged_data;
                    } else {
                        BOOST_LOG_TRIVIAL(error) << "Cannot find model with name:- " << name << " revision:-" << (revision !=nullptr ? revision : ""); 
                        throw YDKIllegalStateException{"Cannot find model"};
                    }
                }
            }
            BOOST_LOG_TRIVIAL(error) << "Cannot find model with name:- " << name;
            throw YDKIllegalStateException{"Cannot find model"};
        }
    }
    
}

ydk::core::RootSchemaNode*
ydk::core::Repository::create_root_schema(const std::vector<ydk::core::Capability> capabilities) const
{
    std::string path_str = path.string();
    struct ly_ctx* ctx = ly_ctx_new(path_str.c_str());
    
    if(!ctx) {
        throw std::bad_alloc{};
    }
    auto pair = new std::pair<struct ly_ctx* , const ydk::core::Repository*>{};
    pair->first = ctx;
    pair->second = this;
    
    //set module callback (only if there is a model provider)
    if(!model_providers.empty()){
        ly_ctx_set_module_clb(ctx, get_module_callback, pair);
    }
    
    for (auto c : capabilities) {
        if(c.module == "ietf-yang-library")
            continue;
        BOOST_LOG_TRIVIAL(trace) << "Module " << c.module.c_str() << " Revision " << c.revision.c_str();
        auto p = ly_ctx_get_module(ctx, c.module.c_str(), c.revision.empty() ? 0 : c.revision.c_str());
        
        if(!p) {
            p = ly_ctx_load_module(ctx, c.module.c_str(), c.revision.empty() ? 0 : c.revision.c_str());
        } else {
            BOOST_LOG_TRIVIAL(trace) << "Cache hit module name:-" << c.module;
        }
        
        if (!p) {
            BOOST_LOG_TRIVIAL(debug) << "Unable to parse module " << c.module;
            continue;
        }
        for (auto f : c.features)
            lys_features_enable(p, f.c_str());

    }
    
    RootSchemaNodeImpl* rs = new RootSchemaNodeImpl{ctx};
    return rs;
}

///
/// @brief Adds a model provider.
///
/// Adds a model provider to this Repository.
/// If the repository does not find a model while trying to create
/// a SchemaTree it calls on the model_provider to see if the said model
/// can be downloaded by one of them. If that fails it tries the next
///
/// @param[in] module_provider The Module Provider to add
///
void
ydk::core::Repository::add_model_provider(ydk::core::ModelProvider* model_provider)
{
    model_providers.push_back(model_provider);
}

///
/// @brief Removes a model provider.
///
/// Removes the given model provider from this Repository.
///
void
ydk::core::Repository::remove_model_provider(ydk::core::ModelProvider* model_provider)
{
    auto item = std::find(model_providers.begin(), model_providers.end(), model_provider);
    if(item != model_providers.end()) {
        model_providers.erase(item);
    }
}

///
/// @brief Get model providers
///
/// Gets all model providers registered with this repo.
///
/// @return vector of ModelProvider's
///
std::vector<ydk::core::ModelProvider*>
ydk::core::Repository::get_model_providers() const
{
    return model_providers;
}


////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////////////////////////////////////
// class RpcImpl
////////////////////////////////////////////////////////////////////////////////


ydk::core::RpcImpl::RpcImpl(SchemaNodeImpl* sn, struct ly_ctx* ctx) : m_sn{sn}
{


    struct lyd_node* dnode = lyd_new_path(nullptr, ctx, sn->path().c_str(), "", 0);

    if(!dnode){
        BOOST_LOG_TRIVIAL(debug) << "Cannot find DataNode with path " << sn->path();
        throw YDKIllegalStateException{"Illegal state"};
    }

    m_input_dn = new DataNodeImpl{nullptr, dnode};

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

