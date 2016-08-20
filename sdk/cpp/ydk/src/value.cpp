#include <iostream>

#include "core.hpp"
#include "value.hpp"

namespace ydk {

std::string to_str(Type t)
{
#define TOSTRING(t) case Type::t: return #t
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
    }
    return "";
}

Value::Value(Type type, std::string name):
		is_set(false),
		name(name),
		value(""),
		type(type)
{
}

std::string & Value::get()
{
	return value;
}

std::pair<std::string, std::string> Value::get_name_value()
{
	return {name, value};
}

void Value::operator = (uint8 val)
{
	value_buffer << val;
//	std::cerr<<"uint8"<<std::endl;
	store_value();
}

void Value::operator = (uint32 val)
{
	value_buffer << val;
//	std::cerr<<"uint32"<<std::endl;
	store_value();
}

void Value::operator = (uint64 val)
{
	value_buffer << val;
	std::cerr<<"uint64"<<std::endl;
	store_value();
}

void Value::operator = (int8 val)
{
	value_buffer << val;
//	std::cerr<<"int8"<<std::endl;
	store_value();
}

void Value::operator = (int32 val)
{
	value_buffer << val;
//	std::cerr<<"int32"<<std::endl;
	store_value();
}

void Value::operator = (int64 val)
{
	value_buffer << val;
//	std::cerr<<"int64"<<std::endl;
	store_value();
}

void Value::operator = (Empty val)
{
	value_buffer << name;
//	std::cerr<<"empty"<<std::endl;
	store_value();
}

void Value::operator = (Identity val)
{
	value_buffer << val.get_tag();
//	std::cerr<<"identiy"<<std::endl;
	store_value();
}

void Value::operator = (Enum val)
{
	value_buffer << val.get_tag();
//	std::cerr<<"enum"<<std::endl;
	store_value();
}

void Value::operator = (std::string val)
{
	value_buffer << val;
//	std::cerr<<"str"<<std::endl;
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
	if(type == Type::boolean)
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
}
