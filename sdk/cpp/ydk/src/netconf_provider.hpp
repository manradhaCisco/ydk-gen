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

#include <memory>
#include <string>

namespace ydk {
class Entity;
namespace core {
class RootSchemaNode;
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

		std::string encode(Entity & entity, const std::string & operation, bool read_config_only) const;
		std::string encode(Entity & entity, const std::string & operation) const;
		std::unique_ptr<Entity> decode(const std::string & payload) const;
		std::string execute_payload(const std::string & payload, const std::string & operation) const;

	private:
		std::unique_ptr<NetconfClient> client;
		std::unique_ptr<ydk::core::RootSchemaNode> root_schema;
		std::vector<ydk::core::Capability> capabilities;
};
}

#endif /*_NETCONF_PROVIDER_H_*/
