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

#include <iostream>

#include "../ydk/src/crud_service.hpp"
#include "ydk/src/entity.hpp"
#include "ydk/models/openconfig_bgp.h"

using namespace std;
using namespace ydk;
using namespace ydk::openconfig_bgp;

int main(int argc, char* argv[])
{
	auto bgp = make_unique<Bgp>();

	// Set the Global AS
	bgp->global_->config->as_ = 65001;

	// Create an AFI SAFI config
	auto ipv4_afsf = make_unique<Bgp::Global::AfiSafis::AfiSafi>();
	ipv4_afsf->afi_safi_name = "ipv4-unicast";
	ipv4_afsf->config->afi_safi_name = "ipv4-unicast";
	ipv4_afsf->config->enabled = true;

	auto ipv6_afsf=make_unique<Bgp::Global::AfiSafis::AfiSafi>();
	ipv6_afsf->afi_safi_name = "ipv6-unicast";
	ipv6_afsf->config->afi_safi_name = "ipv6-unicast";
	ipv6_afsf->config->enabled = true;

	// add the AFI SAFI configs to the global AFI SAFI list
	bgp->global_->afi_safis->afi_safi.push_back(move(ipv4_afsf));
	bgp->global_->afi_safis->afi_safi.push_back(move(ipv4_afsf));

	auto nbr_ipv4 = make_unique<Bgp::Neighbors::Neighbor>();
	nbr_ipv4->neighbor_address = "192.168.1.1";
	nbr_ipv4->config->neighbor_address = "192.168.1.1";
	nbr_ipv4->config->peer_as = 65002;

	auto nbr_ipv4_afsf = make_unique<Bgp::Neighbors::Neighbor::AfiSafis::AfiSafi>();
	nbr_ipv4_afsf->afi_safi_name = "ipv4-unicast";
	nbr_ipv4_afsf->config->afi_safi_name = "ipv4-unicast";
	nbr_ipv4_afsf->config->enabled = true;

	nbr_ipv4->afi_safis->afi_safi.push_back(move(nbr_ipv4_afsf));
	bgp->neighbors->neighbor.push_back(move(nbr_ipv4));

//	NetconfServiceProvider provider{{"127.0.0.1", "12022", "admin", "admin"}};
//
//	CRUDService crud{};
//	crud.create(provider, *bgp);

}
