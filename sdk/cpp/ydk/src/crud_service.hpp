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

#ifndef CRUD_SERVICE_HPP
#define CRUD_SERVICE_HPP

#include <string>
#include <memory>

#include "entity.hpp"

namespace ydk {
class Service {
};
namespace core {
class ServiceProvider;
}

class CrudService : public Service {
	public:
		CrudService();

		bool create(core::ServiceProvider & provider, Entity & entity);
		bool update(core::ServiceProvider & provider, Entity & entity);
		bool del(core::ServiceProvider & provider, Entity & entity);
		std::unique_ptr<Entity> read(core::ServiceProvider & provider, Entity & filter);
};

}

#endif /* CRUD_SERVICE_HPP */
