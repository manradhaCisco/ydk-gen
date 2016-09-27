//
// @file value.hpp
// @brief The main ydk public header.
//
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

#include <boost/log/trivial.hpp>

#include <iostream>

#include "core.hpp"
#include "errors.hpp"
#include "types.hpp"

namespace ydk {

static std::string get_bits_string(const std::map<std::string, bool> & bitmap);
static std::string get_bool_string(const std::string & value);

std::string to_str(YType t)
{
#define TOSTRING(t) case YType::t: return #t
    switch(t)
    {
        TOSTRING(uint8);
        TOSTRING(uint16);
    	TOSTRING(uint32);
    	TOSTRING(uint64);
    	TOSTRING(int8);
    	TOSTRING(int16);
    	TOSTRING(int32);
    	TOSTRING(int64);
    	TOSTRING(empty);
    	TOSTRING(identityref);
    	TOSTRING(str);
    	TOSTRING(boolean);
    	TOSTRING(enumeration);
    	TOSTRING(bits);
    	TOSTRING(decimal64);
    }
    return "";
}

Value::Value(YType type, std::string name):
		is_set(false),
		enum_to_string_func(nullptr),
		name(name),
		value(""),
		type(type)
{
}

Value::Value(const Value& val):
    is_set{val.is_set},
	enum_to_string_func(nullptr),
    name{val.name},
    value{val.value},
    type{val.type}
{

}


Value::Value(Value&& val):
    is_set{val.is_set},
    name{std::move(val.name)},
    value{std::move(val.value)},
    type{val.type}
{
    val.is_set = false;
}

ydk::Value&
Value::operator=(const Value& val)
{
    is_set = val.is_set;
    name = val.name;
    value = val.value;
    type = val.type;

    return *this;
}

ydk::Value&
Value::operator=(Value&& val)
{
    is_set = val.is_set;
    name = std::move(val.name);
    value = std::move(val.value);
    type = val.type;

    return *this;
}


Value::~Value()
{
}

const std::string  Value::get() const
{
	if(type == YType::bits)
	{
		return get_bits_string(bitmap);
	}
	BOOST_LOG_TRIVIAL(trace) <<"Returning value "<<value<<" for leaf "<<name;
	return value;
}

std::pair<std::string, std::string> Value::get_name_value() const
{
	BOOST_LOG_TRIVIAL(trace) <<name<<" : "<<get();
	return {name, get()};
}

void Value::operator = (uint8 val)
{
	value_buffer.clear();
	value_buffer.str("");
	value_buffer << val;
	BOOST_LOG_TRIVIAL(trace)<<"setting uint8";
	store_value();
}

void Value::operator = (uint32 val)
{
	value_buffer.clear();
	value_buffer.str("");
	value_buffer << val;
	BOOST_LOG_TRIVIAL(trace)<<"setting uint32: "<<val;
	store_value();
}

void Value::operator = (uint64 val)
{
	value_buffer.clear();
	value_buffer.str("");
	value_buffer << val;
	BOOST_LOG_TRIVIAL(trace)<<"setting uint64";
	store_value();
}

void Value::operator = (int8 val)
{
	value_buffer.clear();
	value_buffer.str("");
	value_buffer << val;
	BOOST_LOG_TRIVIAL(trace)<<"setting int8";
	store_value();
}

void Value::operator = (int32 val)
{
	value_buffer.clear();
	value_buffer.str("");
	if(is_enum())
	{
		if(enum_to_string_func == nullptr)
		{
			BOOST_LOG_TRIVIAL(error)<<"Enum to string function not set for " << name ;
			throw YDKException("Enum to string function not set for " + name);
		}
		value_buffer << enum_to_string_func(val);
	}
	else
	{
		value_buffer << val;
	}
	BOOST_LOG_TRIVIAL(trace)<<"setting int32";
	store_value();
}

bool Value::is_enum()
{
	return type == YType::enumeration;
}

void Value::operator = (int64 val)
{
	value_buffer.clear();
	value_buffer.str("");
	value_buffer << val;
	BOOST_LOG_TRIVIAL(trace)<<"setting int64";
	store_value();
}

void Value::operator = (Empty val)
{
	value_buffer.clear();
	value_buffer.str("");
	value_buffer << name;
	BOOST_LOG_TRIVIAL(trace)<<"setting empty";
	store_value();
}

void Value::operator = (Identity val)
{
	value_buffer << val.to_string();
	store_value();
}

void Value::operator = (std::string val)
{
	value_buffer.clear();
	value_buffer.str("");
	value_buffer << val;
	BOOST_LOG_TRIVIAL(trace)<<"setting string: "<<val;
	store_value();
}

void Value::store_value()
{
	is_set=true;
	if(type == YType::boolean)
	{
		value = get_bool_string(value_buffer.str());
	}
	else
	{
		value = value_buffer.str();
	}
	BOOST_LOG_TRIVIAL(trace)<<"storing "<<value;
	BOOST_LOG_TRIVIAL(trace)<<"type of leaf: "<<to_str(type);
}

Value::operator std::string() const
{
	return get();
}

bool Value::operator == (Value & other) const
{
	return get() == other.get();
}

bool Value::operator == (const Value & other) const
{
	return get() == other.get();
}

bool & Value::operator [] (std::string key)
{
	is_set = true;
	return bitmap[key];
}

std::ostream& operator<< (std::ostream& stream, const Value& value)
{
	stream << value.get();
	return stream;
}

std::string get_bool_string(const std::string & value)
{
	if(value == "1")
	{
		return "true";
	}
	else
	{
		return "false";
	}
}

std::string get_bits_string(const std::map<std::string, bool> & bitmap)
{
	std::string value;
	for(auto const & entry : bitmap)
	{
		if(entry.second)
		{
			value += entry.first + " ";
		}
	}
    BOOST_LOG_TRIVIAL(trace)<<value<<": value of bits";

	return (value);
}

}
