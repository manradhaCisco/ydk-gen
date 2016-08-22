//
// @file entity.hpp
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

#ifndef ENTITY_HPP
#define ENTITY_HPP

#include <string>
#include <vector>

namespace ydk {

namespace core {
class DataNode;
class RootSchemaNode;
}

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
	virtual std::vector<Entity*> & get_children()
	{
		return children;
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

}

#endif /* ENTITY_HPP */
