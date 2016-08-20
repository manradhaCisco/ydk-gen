/*  ----------------------------------------------------------------
 Copyright 2016 Cisco Systems

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
------------------------------------------------------------------*/

#ifndef _NETCONF_PROVIDER_H_
#define _NETCONF_PROVIDER_H_

#include <initializer_list>
#include <memory>
#include <string>

#include "entity.hpp"

namespace ydk {
namespace core {
struct Capability;
}

class NetconfClient;
class NetconfServiceProvider {
	public:
		NetconfServiceProvider(std::string address,
				std::string username,
				std::string password,
				std::string port,
				std::string protocol,
				std::string timeout,
				std::string searchdir);
		~NetconfServiceProvider();

		std::string encode(Entity & entity, std::string  operation);
		std::unique_ptr<Entity> decode(std::string & payload);
		std::string execute_payload(std::string payload, std::string operation);

	private:
		std::unique_ptr<NetconfClient> client;
		std::unique_ptr<ydk::core::RootSchemaNode> root_schema;
		std::vector<ydk::core::Capability> capabilities;
};
}

#endif /*_NETCONF_PROVIDER_H_*/
