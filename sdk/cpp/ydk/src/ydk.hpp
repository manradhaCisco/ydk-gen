//
// @file ydk.hpp
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

#ifndef YDK_HPP
#define YDK_HPP

#include <string>
#include <vector>
#include <memory>

namespace std {

template<typename T, typename ...Args>
std::unique_ptr<T> make_unique( Args&& ...args )
{
    return std::unique_ptr<T>( new T( std::forward<Args>(args)... ) );
}

}

namespace ydk {
  class Entity {

  };

class ServiceProvider {
	public:
		virtual std::string encode(Entity & entity)=0;
		virtual std::unique_ptr<Entity> decode(std::string & payload)=0;
		virtual bool execute_payload(std::string & payload)=0;
};
class Service {

};


class CRUDService : public Service {
	public:
		CRUDService();

		bool create(ServiceProvider & provider, Entity & entity);
		std::unique_ptr<Entity> read(ServiceProvider & provider, Entity & entity);
		bool update(ServiceProvider & provider, Entity & entity);
		bool del(ServiceProvider & provider, Entity & entity);
};


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



}

#endif /* YDK_HPP */