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

#include "netconf_private.hpp"
#include "entity.hpp"

#include <fstream>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <vector>


using namespace std;

//using namespace boost::python;
using namespace ydk;

typedef vector<string> StringVec;

//BOOST_PYTHON_MODULE(ydk_client)
//{
//	class_<vector<string> >("StringVec")
//	        .def(vector_indexing_suite<std::vector<string> >())
//	    ;

//    class_<NetconfClient>("NetconfClient", init<string , string , string , int, int>())
//        .def("connect", &NetconfClient::connect)
//        .def("execute_payload", &NetconfClient::execute_payload)
//		.def("close", &NetconfClient::close)
//		.def("get_capabilities", &NetconfClient::get_capabilities);
//};


namespace ydk
{

map<pair<string, string>, string> NetconfClient::password_lookup;

NetconfClient::NetconfClient(string  username, string  password,
		string  hostname, int port, int verbosity) :
		username(username), hostname(hostname), port(port), sock(-1), return_status(
				EXIT_SUCCESS)
{
	nc_verbosity((NC_VERB_LEVEL) verbosity);
	nc_callback_print(clb_print);
	nc_callback_sshauth_password(clb_set_password);
	nc_callback_ssh_host_authenticity_check(clb_ssh_host_authenticity_check);

	password_lookup.insert(make_pair(make_pair(username, hostname), password));
	session=NULL;
}

int NetconfClient::connect()
{
	session = nc_session_connect(hostname.c_str(), port, username.c_str(), NULL);
	if (session == NULL)
	{
		return_status = EXIT_FAILURE;
		return (EXIT_FAILURE);
	}

	init_capabilities();
	return EXIT_SUCCESS;
}

void NetconfClient::init_capabilities()
{
	struct nc_cpblts* capabilities_list;
	const char *cap;

	capabilities_list = nc_session_get_cpblts(session);
	nc_cpblts_iter_start(capabilities_list);

	while ((cap = nc_cpblts_iter_next(capabilities_list)) != NULL)
	{
		capabilities.push_back(cap);
	}
}

const string NetconfClient::execute_payload(string  payload)
{
	if(session==NULL)
	{
		return_status = EXIT_FAILURE;
		return "";
	}
	nc_reply *reply;
	NC_MSG_TYPE reply_type;
	nc_rpc *rpc;
	string reply_payload;

	rpc = build_rpc_request(move(payload));
	if (rpc == NULL || return_status != EXIT_SUCCESS || NC_RPC_UNKNOWN==nc_rpc_get_type(rpc))
	{
		return_status = EXIT_FAILURE;
		return "";
	}

	reply_type = nc_session_send_recv(session, rpc, &reply);
	process_rpc_reply(reply_type);

	if (NC_MSG_REPLY == reply_type)
	{
		reply_payload = nc_reply_dump(reply);
	}

	nc_reply_free(reply);
	nc_rpc_free(rpc);

	return reply_payload;
}

int NetconfClient::close()
{
	if(session==NULL)
	{
		return_status = EXIT_FAILURE;
		return EXIT_FAILURE;

	}

	nc_session_free(session);
	return EXIT_SUCCESS;
}

nc_rpc* NetconfClient::build_rpc_request(string  payload)
{
	nc_rpc* rpc = nc_rpc_build(payload.c_str(), session);
	if (rpc == NULL)
	{
		return_status = EXIT_FAILURE;
	}
	return rpc;
}

void NetconfClient::process_rpc_reply(int reply_type)
{
	switch (reply_type)
	{
	case NC_MSG_NONE:
	case NC_MSG_UNKNOWN:
		return_status = EXIT_FAILURE;
		break;

	case NC_MSG_REPLY:
		return_status = EXIT_SUCCESS;
		break;

	default:
		return_status = EXIT_FAILURE;
		break;
	}
}

StringVec NetconfClient::get_capabilities()
{
	return capabilities;
}

void NetconfClient::clb_print(NC_VERB_LEVEL level, const char* msg)
{
	switch (level)
	{
	case NC_VERB_ERROR:
		cerr << "libnetconf ERROR: " << msg << endl;
		break;
	case NC_VERB_WARNING:
		cerr << "libnetconf WARNING: " << msg << endl;
		break;
	case NC_VERB_VERBOSE:
		cerr << "libnetconf VERBOSE: " << msg << endl;
		break;
	case NC_VERB_DEBUG:
		cerr << "libnetconf DEBUG: " << msg << endl;
		break;
	}
}

char* NetconfClient::clb_set_password(const char* user_name,
		const char* host_name)
{
	string password_string = password_lookup[make_pair(user_name, host_name)];
	char* password_buffer = (char*) malloc(
			sizeof(char) * (password_string.size() + 1));
	snprintf(password_buffer, password_string.size() + 1, "%s",
			password_string.c_str());

	return password_buffer;
}

int NetconfClient::clb_ssh_host_authenticity_check(const char *hostname,
		ssh_session session)
{
	return EXIT_SUCCESS;
}

int NetconfClient::get_status()
{
	return return_status;
}

NetconfServiceProvider::NetconfServiceProvider(std::initializer_list<std::string> args)
{
	if(args.size()==0)
	{
		cout<<"- address - The address of the netconf server"
            "- port  - The port to use default is 830"
            "- username - The name of the user"
            "- password - The password to use"
            "- protocol - one of either ssh or tcp"
            "- timeout  - Default to 60"<<endl;
		return;
	}

	std::vector<std::string> argsv;
	argsv.insert(argsv.end(), args.begin(), args.end());


	username = argsv[2];
	address = argsv[0];
	port = argsv[1];
	password = argsv[3];
	protocol = argsv[4];
	timeout = argsv[5];
}

std::string NetconfServiceProvider::encode(Entity & entity)
{
	return "";
}

std::unique_ptr<Entity> NetconfServiceProvider::decode(std::string & payload)
{
	return nullptr;
}

bool NetconfServiceProvider::execute_payload(std::string & payload)
{
	char connect_command[500];

	ofstream myfile;
	myfile.open ("payload.xml");
	myfile << payload;
	myfile.close();

	snprintf(connect_command,sizeof(connect_command),
			"ssh %s@%s -p %s -s netconf < payload.xml > debug.log",
			username.c_str(),
			address.c_str(),
			port.c_str()
			);

	int res=system(connect_command);
	cout << "Connected to device with command: "<< connect_command<<". Result: " << res << endl;
	cout << "Debug in debug.log"<<endl;
	return res==0;
}


}
