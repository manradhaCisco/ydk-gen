//
// @file types.hpp
// @brief Header for ydk entity
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

#ifndef _TYPES_HPP_
#define _TYPES_HPP_

#include <string>
#include <vector>
#include <memory>
#include <sstream>

namespace ydk {

typedef unsigned short uint8;
typedef unsigned int uint16;
typedef unsigned int uint32;
typedef unsigned long long uint64;

typedef signed short int8;
typedef signed int int16;
typedef signed int int32;
typedef signed long long int64;

typedef struct Empty {
    bool set;
} Empty;

namespace core {
class DataNode;
class RootSchemaNode;
}

class Entity;

struct EntityPath {
	std::string path;
	std::vector<std::pair<std::string, std::string>> value_paths;

	EntityPath(std::string path, std::vector<std::pair<std::string, std::string>> value_paths)
		: path(path), value_paths(value_paths)
	{
	}

	~EntityPath()
	{
	}

	inline bool operator == (EntityPath & other) const
	{
		return path == other.path && value_paths == other.value_paths;
	}

	inline bool operator == (const EntityPath & other) const
	{
		return path == other.path && value_paths == other.value_paths;
	}
};

class Entity {
  public:
	Entity():parent(nullptr){}
	virtual ~Entity(){}

  public:
	virtual bool has_data() const = 0;
	virtual EntityPath get_entity_path() const = 0;

    virtual Entity* set_child(std::string path) = 0;

    virtual void set_value(std::string value_path, std::string value) = 0;

	virtual std::vector<Entity*> & get_children()
	{
		return children;
	}

	virtual std::unique_ptr<Entity> clone_ptr()
	{
		return nullptr;
	}

	void add_child(Entity* child)
	{
		children.push_back(child);
	}

  public:
	Entity* parent;

  private:
	std::vector<Entity*> children;
};


class Enum {
  public:
	Enum(std::string tag) : tag(tag)
	{
	}

	virtual ~Enum(){}

	std::string get_tag()
	{
		return tag;
	}

  private:
	std::string tag;
};


class Identity {
  public:
	Identity(std::string tag) : tag(tag)
	{
	}

	virtual ~Identity(){}

	std::string get_tag()
	{
		return tag;
	}

  private:
	std::string tag;
};

enum class YType {
	uint8,
	uint16,
	uint32,
	uint64,
	int8,
	int16,
	int32,
	int64,
	empty,
	identityref,
	str,
	boolean
};

class Value {
  public:
	Value(YType type, std::string name);
	~Value();

	const std::string & get() const;
	std::pair<std::string, std::string> get_name_value() const;

	void operator = (uint8 val);
	void operator = (uint32 val);
	void operator = (uint64 val);
	void operator = (int8 val);
	void operator = (int32 val);
	void operator = (int64 val);
	void operator = (Empty val);
	void operator = (Identity val);
	void operator = (Enum val);
	void operator = (std::string val);
//	void operator = (bool val);

	operator std::string() const;
	bool operator == (Value & other) const;
	bool operator == (const Value & other) const;

	bool is_set;

  private:
	void store_value();

	std::string name;
	std::ostringstream value_buffer;
	std::string value;
	YType type;
};

std::ostream& operator<< (std::ostream& stream, const Value& value);



}

#endif /* _TYPES_HPP_ */
