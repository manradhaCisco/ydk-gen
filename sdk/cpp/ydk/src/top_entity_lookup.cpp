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

#include "top_entity_lookup.hpp"

#include "make_unique.hpp"
#include "../tests/models_gen/openconfig_bgp.h"

using namespace std;
using namespace ydk;

namespace ydk {

std::unique_ptr<Entity> lookup_path(std::string path)
{
	if(path == "/openconfig-bgp:bgp")
	{
		return make_unique<openconfig_bgp::Bgp>();
	}

	return nullptr;
}

void insert(std::string path, std::unique_ptr<Entity> entity)
{

}

TopEntityLookUp::TopEntityLookUp() : m_entity()
{

}

TopEntityLookUp::~TopEntityLookUp()
{

}

std::unique_ptr<Entity> TopEntityLookUp::lookup(std::string path)
{
    return std::move(m_entity.at(path));
}

void TopEntityLookUp::insert(std::string path, std::unique_ptr<Entity> top_entity) {
    m_entity.insert(std::make_pair(path, std::move(top_entity)));
}

}
