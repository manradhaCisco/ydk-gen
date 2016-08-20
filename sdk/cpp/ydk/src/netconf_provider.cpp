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

#include <iostream>

//#include <boost/python.hpp>
//#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <libnetconf.h>
#include <libnetconf_ssh.h>

#include "entity.hpp"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include "netconf_client.hpp"
#include "netconf_provider.hpp"
#include "core.hpp"
#include "make_unique.hpp"
#include "entity_walker.hpp"

using namespace std;
using namespace ydk;

static string get_value(char* tag_value);

namespace ydk
{

NetconfServiceProvider::NetconfServiceProvider(string address, string username, string password, string port, string protocol, string timeout, string searchdir)
	: client(make_unique<NetconfClient>(username, password, address, atoi(port.c_str()), 0))
{
	client->connect();
	vector<string> caps = client->get_capabilities();
	//TODO
	capabilities=    {{"openconfig-bgp-types", "" },
		    {"openconfig-bgp", ""},
		    {"openconfig-extensions", ""},
		    {"openconfig-interfaces", ""},
		    {"openconfig-policy-types", ""},
		    {"openconfig-routing-policy", ""},
		    {"openconfig-types", ""},
		    {"ietf-interfaces", ""},
		    {"ydk", ""}};
//	for(string & cap:caps)
//	{
//		string module, revision;
//		char* tok = strtok((char*)cap.c_str(), "&");
//		tok = strtok(NULL, "&");
//		if(tok != NULL)
//		{
//			char* module_start = strstr (tok, "module=");
//			tok = strtok(NULL, "&");
//			if(tok != NULL && module_start != NULL)
//			{
//				char* revision_start = strstr (tok, "revision=");
//				if(revision_start != NULL)
//				{
//					module = get_value(module_start);
//					revision = get_value(revision_start);
//					capabilities.push_back({module, revision});
//				}
//			}
//		}
//	}

	auto repo = ydk::core::Repository{searchdir};
	root_schema = std::unique_ptr<ydk::core::RootSchemaNode>(repo.create_root_schema(capabilities));
}

NetconfServiceProvider::~NetconfServiceProvider()
{
	client->close();
}

string NetconfServiceProvider::encode(Entity & entity, std::string  operation)
{
	const ydk::core::DataNode* data_node = get_data_node(entity, *root_schema);
	if (data_node==nullptr)
		return "";
	auto s = ydk::core::CodecService{};
	auto xml = s.encode(data_node, ydk::core::CodecService::Format::XML, true);
	string payload;
	if(operation=="create")
	{
		payload = "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
				       "<edit-config>"
				       "<target><candidate/></target>"
				       "<config>"+xml+"</config>"
					    "</edit-config>"
					    "</rpc>";
	}
	return payload;
}

unique_ptr<Entity> NetconfServiceProvider::decode(string & payload)
{
	return nullptr;
}

std::string NetconfServiceProvider::execute_payload(string  payload, string  operation)
{
	string reply = client->execute_payload(payload);
	if(operation =="create" && strstr(reply.c_str(),"<ok/>")!=NULL)
	{
		if(operation=="create")
			{
				reply = client->execute_payload("<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
						       "<commit/>"
							    "</rpc>");
			}
	}
	return reply;
}

}

static string get_value(char* tag_value)
{
	string m{tag_value};
	char* mod = (char*)m.c_str();
	char* ntok = strtok(mod, "=");
	ntok = strtok(NULL, "=");
	return ntok;
}
