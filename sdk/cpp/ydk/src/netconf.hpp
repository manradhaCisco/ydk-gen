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

#ifndef _YDK_PROVIDERS_H_
#define _YDK_PROVIDERS_H_

#include <initializer_list>
#include <memory>
#include <string>

#include "ydk.hpp"

namespace ydk {


class NetconfServiceProvider : public ServiceProvider {
	public:
		NetconfServiceProvider(std::initializer_list<std::string> args);

		std::string encode(Entity & entity) override;
		std::unique_ptr<Entity> decode(std::string & payload) override;
		bool execute_payload(std::string & payload) override;

	private:
		std::string address;
		std::string username;
		std::string password;
		std::string port;
		std::string protocol;
		std::string timeout;
};

}

#endif /*_YDK_PROVIDERS_H_*/