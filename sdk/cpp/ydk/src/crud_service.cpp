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

#include "crud_service.hpp"
#include "entity.hpp"
#include "netconf_provider.hpp"
#include <iostream>

using namespace std;

namespace ydk {
CrudService::CrudService()
{

}

string CrudService::create(NetconfServiceProvider & provider, Entity & entity)
{
	string payload = provider.encode(entity, "create");
	std::cerr<<payload<<std::endl;
	string reply = provider.execute_payload(payload, "create");
	std::cerr<<reply<<std::endl;
	return reply;
}

std::unique_ptr<Entity> CrudService::read(NetconfServiceProvider & provider, Entity & entity)
{
	string payload = provider.encode(entity, "read", false);
	std::cerr<<payload<<std::endl;
	string reply=provider.execute_payload(payload, "read");;
	std::cerr<<reply<<std::endl;
	return provider.decode(reply);
}

std::unique_ptr<Entity> CrudService::read(NetconfServiceProvider & provider, Entity & entity, bool config_only)
{
	string payload = provider.encode(entity, "read", config_only);
	std::cerr<<payload<<std::endl;
	string reply=provider.execute_payload(payload, "read");;
	std::cerr<<reply<<std::endl;
	return provider.decode(reply);
}

string CrudService::update(NetconfServiceProvider & provider, Entity & entity)
{
	string payload = provider.encode(entity, "update");
	std::cerr<<payload<<std::endl;
	string reply = provider.execute_payload(payload, "update");
	std::cerr<<reply<<std::endl;
	return reply;
}

string CrudService::del(NetconfServiceProvider & provider, Entity & entity)
{
	string payload = provider.encode(entity, "delete");
	std::cerr<<payload<<std::endl;
	string reply = provider.execute_payload(payload, "delete");
	std::cerr<<reply<<std::endl;
	return reply;
}
}
