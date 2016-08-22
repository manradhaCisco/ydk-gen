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
#include "entity.hpp"
#include "netconf_client.hpp"
#include "netconf_provider.hpp"
#include "core.hpp"
#include "entity_data_node_walker.hpp"
#include "make_unique.hpp"
#include "top_entity_lookup.hpp"

using namespace std;
using namespace ydk;

namespace ydk
{
static string get_value(char* tag_value);
static string get_data_from_reply(const string & payload);
static string get_rpc_payload(const string & data_payload, const string & operation, bool read_config_only);
static string get_commit_rpc_payload();
static string get_data_payload(Entity & entity, core::RootSchemaNode & root_schema);

NetconfServiceProvider::NetconfServiceProvider(string address, string username, string password, string port, string protocol, string timeout, string searchdir)
	: client(make_unique<NetconfClient>(username, password, address, atoi(port.c_str()), 0))
{
	client->connect();
	//TODO
//	vector<string> caps = client->get_capabilities();
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
	capabilities=    {{"openconfig-bgp-types", "" },
		    {"openconfig-bgp", ""},
		    {"openconfig-extensions", ""},
		    {"openconfig-interfaces", ""},
		    {"openconfig-policy-types", ""},
		    {"openconfig-routing-policy", ""},
		    {"openconfig-types", ""},
		    {"ietf-interfaces", ""},
		    {"ydk", ""}};


	auto repo = ydk::core::Repository{searchdir};
	root_schema = unique_ptr<ydk::core::RootSchemaNode>(repo.create_root_schema(capabilities));
}

NetconfServiceProvider::~NetconfServiceProvider()
{
	client->close();
}

string NetconfServiceProvider::encode(Entity & entity, const string & operation, bool read_config_only) const
{
	std::string data_payload = get_data_payload(entity, *root_schema);
	return get_rpc_payload(data_payload, operation, read_config_only);
}

string NetconfServiceProvider::encode(Entity & entity, const string & operation) const
{
	std::string data_payload = get_data_payload(entity, *root_schema);
	return get_rpc_payload(data_payload, operation, false);
}

unique_ptr<Entity> NetconfServiceProvider::decode(const string & payload) const
{
	string data = get_data_from_reply(payload);
	cerr<<data<<endl;
	if(data.size()==0)
	{
		return nullptr;
	}

	auto s = ydk::core::CodecService{};
	core::DataNode* top_data_node = s.decode(root_schema.get(), data, ydk::core::CodecService::Format::XML);
	auto top_entity = lookup_path(top_data_node->path());
	walk_data_node(top_data_node, top_entity.get());

	return top_entity;
}

string NetconfServiceProvider::execute_payload(const string & payload, const string & operation) const
{
	string reply = client->execute_payload(payload);
	if(strstr(reply.c_str(),"<ok/>")!=NULL)
	{
		if(operation=="create" || operation=="update")
		{
			reply = client->execute_payload(get_commit_rpc_payload());
		}
	}
	return reply;
}

static string get_data_payload(Entity & entity, core::RootSchemaNode & root_schema)
{
	const ydk::core::DataNode* data_node = get_data_node(entity, root_schema);
	if (data_node==nullptr)
		return "";
	auto s = ydk::core::CodecService{};
	return s.encode(data_node, ydk::core::CodecService::Format::XML, true);
}

static string get_rpc_payload(const string & data_payload, const string & operation, bool read_config_only)
{
	if(operation=="create" || operation=="update")
	{
		return "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
			   "<edit-config>"
			   "<target><candidate/></target>"
			   "<config>"+data_payload+"</config>"
				"</edit-config>"
				"</rpc>";
	}
	else if(operation=="read")
	{
		if (read_config_only)
		{
			return "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
				   "<get-config>"
				   "<source><candidate/></source>"
				   "<filter>"+data_payload+"</filter>"
					"</get-config>"
					"</rpc>";
		}
		else
		{
			return "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
				   "<get>"
				   "<filter>"+data_payload+"</filter>"
					"</get>"
					"</rpc>";
		}
	}

	return "";
}

static string get_commit_rpc_payload()
{
	return "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
		   "<commit/>"
			"</rpc>";
}

static string get_data_from_reply(const string & payload)
{
	static const string start ="<data>";
	static const string end   ="</data>";
	size_t starting = payload.find(start);
	if(starting == string::npos)
	{
		return "";
	}

	size_t ending = payload.find(end);
	if(ending == string::npos)
	{
		return "";
	}

	return payload.substr(starting + start.size(),
								 ending - starting - start.size());
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
