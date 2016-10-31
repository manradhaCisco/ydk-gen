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
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>

#include "ydk/types.hpp"
#include "ydk/netconf_provider.hpp"
#include "ydk/crud_service.hpp"
#include "ydk_cisco_ios_xr/Cisco_IOS_XR_ipv4_bgp_cfg.hpp"
#include "ydk_cisco_ios_xr/Cisco_IOS_XR_ipv4_bgp_datatypes.hpp"

#include "args_parser.h"

using namespace ydk;
using namespace std;

void config_bgp(Cisco_IOS_XR_ipv4_bgp_cfg::Bgp* bgp)
{
	auto instance = make_unique<Cisco_IOS_XR_ipv4_bgp_cfg::Bgp::Instance>();
	instance->instance_name = "default";
	auto instance_as = make_unique<Cisco_IOS_XR_ipv4_bgp_cfg::Bgp::Instance::InstanceAs>();
	instance_as->as = 65001;
	auto four_byte_as = make_unique<Cisco_IOS_XR_ipv4_bgp_cfg::Bgp::Instance::InstanceAs::FourByteAs>();
	four_byte_as->as = 65001;
	four_byte_as->bgp_running = Empty{};
	// global address family
	auto global_af = make_unique<Cisco_IOS_XR_ipv4_bgp_cfg::Bgp::Instance::InstanceAs::FourByteAs::DefaultVrf::Global::GlobalAfs::GlobalAf>();
	global_af->af_name = Cisco_IOS_XR_ipv4_bgp_datatypes::BgpAddressFamilyEnum::ipv4_unicast;
	global_af->enable = Empty{};
	four_byte_as->default_vrf->global->global_afs->global_af.push_back(move(global_af));
	instance_as->four_byte_as.push_back(move(four_byte_as));
	instance->instance_as.push_back(move(instance_as));
	bgp->instance.push_back(move(instance));

	// configure IBGP neighbor group
//	neighbor_groups = four_byte_as.default_vrf.bgp_entity.neighbor_groups;
//	neighbor_group = neighbor_groups.NeighborGroup();
//	neighbor_group.neighbor_group_name = "IBGP";
//	neighbor_group.create = Empty();
//	// remote AS
//	neighbor_group.remote_as.as_xx = 0;
//	neighbor_group.remote_as.as_yy = 65001;
//	neighbor_group.update_source_interface = "Loopback0";
//	neighbor_groups.neighbor_group.append(neighbor_group);
//	// ipv4 unicast
//	neighbor_group_af = neighbor_group.neighbor_group_afs.NeighborGroupAf();
//	neighbor_group_af.af_name = BgpAddressFamilyEnum.IPV4_UNICAST;
//	neighbor_group_af.activate = Empty();
//	neighbor_group_afs = neighbor_group.neighbor_group_afs;
//	neighbor_group_afs.neighbor_group_af.append(neighbor_group_af);
//
//	// configure IBGP neighbor
//	neighbor = four_byte_as.default_vrf.bgp_entity.neighbors.Neighbor();
//	neighbor.neighbor_address = "172.16.255.3";
//	neighbor.neighbor_group_add_member = "IBGP";
//	four_byte_as.default_vrf.bgp_entity.neighbors.neighbor.append(neighbor);
}

int main(int argc, char* argv[])
{
	boost::log::core::get()->set_filter(
		        boost::log::trivial::severity >= boost::log::trivial::debug
		    );

	vector<string> args = parse_args(argc, argv);
	if(args.empty()) return 1;
	string host, username, password;
	int port;
	username = args[0]; password = args[1]; host = args[2]; port = stoi(args[3]);

	ydk::core::Repository repo{};
	NetconfServiceProvider provider{&repo, host, username, password, port};
	CrudService crud{};

	auto bgp = make_unique<Cisco_IOS_XR_ipv4_bgp_cfg::Bgp>();
	config_bgp(bgp.get());
	bool reply = crud.create(provider, *bgp);

	if(reply) cout << "Create operation success" << endl << endl; else cout << "Operation failed" << endl << endl;

}
