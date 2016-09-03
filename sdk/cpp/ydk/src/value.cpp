#include <iostream>

#include "core.hpp"
#include "errors.hpp"
#include "types.hpp"

namespace ydk {

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

const std::string & Value::get() const
{
	return value;
}

std::pair<std::string, std::string> Value::get_name_value() const
{
	return {name, value};
}

void Value::operator = (uint8 val)
{
	value_buffer.clear();
	value_buffer.str("");
	value_buffer << val;
//	std::cerr<<"uint8"<<std::endl;
	store_value();
}

void Value::operator = (uint32 val)
{
	value_buffer.clear();
	value_buffer.str("");
	value_buffer << val;
//	std::cerr<<"uint32"<<std::endl;
	store_value();
}

void Value::operator = (uint64 val)
{
	value_buffer.clear();
	value_buffer.str("");
	value_buffer << val;
	std::cerr<<"uint64"<<std::endl;
	store_value();
}

void Value::operator = (int8 val)
{
	value_buffer.clear();
	value_buffer.str("");
	value_buffer << val;
//	std::cerr<<"int8"<<std::endl;
	store_value();
}

void Value::operator = (int32 val)
{
	value_buffer.clear();
	value_buffer.str("");
	if(is_enum())
	{
		if(enum_to_string_func == nullptr)
			throw YDKException("Enum to string function not set for " + name);
		value_buffer << enum_to_string_func(val);
	}
	else
	{
		value_buffer << val;
	}
//	std::cerr<<"int32"<<std::endl;
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
//	std::cerr<<"int64"<<std::endl;
	store_value();
}

void Value::operator = (Empty val)
{
	value_buffer.clear();
	value_buffer.str("");
	value_buffer << name;
//	std::cerr<<"empty"<<std::endl;
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
	store_value();
}

//void Value::operator = (bool val)
//{
//	value_buffer << val;
//	std::cerr<<"bools"<<std::endl;
//	store_value();
//}

void Value::store_value()
{
	is_set=true;
	if(type == YType::boolean)
	{
		if(value_buffer.str()=="1")
		{
			value = "true";
		}
		else
		{
			value = "false";
		}
	}
	else
	{
		value = value_buffer.str();
	}
//	std::cerr<<"stoing "<<value<<std::endl;
//	std::cerr<<to_str(type)<<">>-----<<"<<std::endl;
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

std::ostream& operator<< (std::ostream& stream, const Value& value)
{
	stream << value.get();
	return stream;
}
}
