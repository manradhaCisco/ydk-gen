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
#define BOOST_TEST_MODULE NetconfProviderTest
#include <boost/test/unit_test.hpp>
#include <string.h>
#include "../ydk/src/netconf_provider.hpp"
#include "../ydk/src/errors.hpp"
#include <iostream>
#include "config.hpp"

using namespace ydk;
using namespace std;



BOOST_AUTO_TEST_CASE(Create)
{
	ydk::core::Repository repo{};
	NetconfServiceProvider provider{&repo, "localhost", "admin", "admin", 1220};

	auto root_schema = provider.get_root_schema();
	BOOST_REQUIRE(root_schema != nullptr);
}
