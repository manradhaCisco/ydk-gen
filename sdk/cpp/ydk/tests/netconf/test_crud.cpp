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

#define BOOST_TEST_MODULE CrudTest
#include <boost/test/unit_test.hpp>

#include <string.h>
#include <iostream>

#include "../../src/netconf_provider.hpp"
#include "../../src/crud_service.hpp"
#include "../../src/make_unique.hpp"
#include "../../models/openconfig_bgp.h"

using namespace ydk;
using namespace std;
#define MODELS_PATH "/Users/abhirame/Cisco/003/ydk-gen/sdk/cpp/ydk/tests/models/openconfig"

BOOST_AUTO_TEST_CASE(bgp_as)
{
	NetconfServiceProvider provider{ "127.0.0.1", "admin", "admin", "12022", "", "", MODELS_PATH};
	CrudService crud{};
	auto bgp = make_unique<openconfig_bgp::Bgp>();

	// Set the Global AS
	bgp->global_->config->as_ = 65001;
	bgp->global_->config->router_id = "1.2.3.4";

	auto afi_safi = make_unique<openconfig_bgp::Bgp::Global::AfiSafis::AfiSafi>();
	afi_safi->afi_safi_name = "openconfig-bgp-types:L3VPN_IPV4_UNICAST";
	afi_safi->config->afi_safi_name = "openconfig-bgp-types:L3VPN_IPV4_UNICAST";
	afi_safi->config->enabled = true;
	afi_safi->parent = bgp->global_->afi_safis.get();
	bgp->global_->afi_safis->afi_safi.push_back(move(afi_safi));

	auto neighbor = make_unique<openconfig_bgp::Bgp::Neighbors::Neighbor>();
	neighbor->neighbor_address = "6.7.8.9";
	neighbor->config->neighbor_address = "6.7.8.9";
	neighbor->config->enabled = true;
	neighbor->config->peer_as = 65001;
	neighbor->config->local_as = 65001;
	neighbor->parent = bgp->neighbors.get();
	bgp->neighbors->neighbor.push_back(move(neighbor));

	string reply = crud.create(provider, *bgp);
	BOOST_REQUIRE(strstr(reply.c_str(),"<ok/>") != NULL);
}
