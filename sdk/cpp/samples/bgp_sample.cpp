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

#include <unistd.h>
#include <iostream>

#include "ydk/netconf_provider.hpp"
#include "ydk/crud_service.hpp"
#include "ydk_openconfig/openconfig_bgp.h"


using namespace ydk;
using namespace std;

#define MODELS_DIR "/Users/abhirame/Cisco/003/ydk-gen/sdk/cpp/ydk/tests/models"

void config_bgp(openconfig_bgp::Bgp* bgp)
{
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
	neighbor->config->peer_group = "IBGP";
	neighbor->parent = bgp->neighbors.get();
	bgp->neighbors->neighbor.push_back(move(neighbor));

	auto peer_group = make_unique<openconfig_bgp::Bgp::PeerGroups::PeerGroup>();
	peer_group->peer_group_name = "IBGP";
	peer_group->config->peer_group_name = "IBGP";
	peer_group->config->auth_password = "password";
	peer_group->config->description = "test description";
	peer_group->config->peer_as = 65001;
	peer_group->config->local_as = 65001;
	bgp->peer_groups->peer_group.push_back(move(peer_group));
}

void config_new_bgp(openconfig_bgp::Bgp* bgp)
{
	// Set the Global AS
	bgp->global_->config->as_ = 65021;
	bgp->global_->config->router_id = "1.2.1.4";
//
//	auto afi_safi = make_unique<openconfig_bgp::Bgp::Global::AfiSafis::AfiSafi>();
//	afi_safi->afi_safi_name = "openconfig-bgp-types:L3VPN_IPV6_UNICAST";
//	afi_safi->config->afi_safi_name = "openconfig-bgp-types:L3VPN_IPV6_UNICAST";
//	afi_safi->config->enabled = true;
//	afi_safi->parent = bgp->global_->afi_safis.get();
//	bgp->global_->afi_safis->afi_safi.push_back(move(afi_safi));
//
//	auto neighbor = make_unique<openconfig_bgp::Bgp::Neighbors::Neighbor>();
//	neighbor->neighbor_address = "6.7.1.9";
//	neighbor->config->neighbor_address = "6.7.1.9";
//	neighbor->config->enabled = true;
//	neighbor->config->peer_as = 65021;
//	neighbor->config->local_as = 65021;
//	neighbor->config->peer_group = "IBGP";
//	neighbor->parent = bgp->neighbors.get();
//	bgp->neighbors->neighbor.push_back(move(neighbor));
//
//	auto peer_group = make_unique<openconfig_bgp::Bgp::PeerGroups::PeerGroup>();
//	peer_group->peer_group_name = "IBGP";
//	peer_group->config->peer_group_name = "IBGP";
//	peer_group->config->auth_password = "password";
//	peer_group->config->description = "testing description";
//	peer_group->config->peer_as = 65021;
//	peer_group->config->local_as = 65021;
//	bgp->peer_groups->peer_group.push_back(move(peer_group));
}

void run_create_delete(CrudService & crud, NetconfServiceProvider & provider)
{
	auto bgp = make_unique<openconfig_bgp::Bgp>();
	bool reply = crud.del(provider, *bgp);
	if(reply) cout << "Delete operation success" << endl; else cout << "Operation failed" << endl;

	config_bgp(bgp.get());
	reply = crud.create(provider, *bgp);
	if(reply) cout << "Create operation success" << endl << endl; else cout << "Operation failed" << endl << endl;
	cout << "=================================================="<<endl<<endl;
}

void run_read_delete(CrudService & crud, NetconfServiceProvider & provider)
{
	auto bgp_set = make_unique<openconfig_bgp::Bgp>();
	bool reply = crud.del(provider, *bgp_set);
	if(reply) cout << "Delete operation success" << endl; else cout << "Operation failed" << endl;

	config_bgp(bgp_set.get());
	reply = crud.create(provider, *bgp_set);
	if(reply) cout << "Create operation success"; else cout << "Operation failed" << endl;

	auto bgp_filter = make_unique<openconfig_bgp::Bgp>();
	auto bgp_read = crud.read(provider, *bgp_filter);
	openconfig_bgp::Bgp * bgp_read_ptr = dynamic_cast<openconfig_bgp::Bgp*>(bgp_read.get());

	if(bgp_set->global_->config->as_ == bgp_read_ptr->global_->config->as_)
		 cout << "Read operation success" << endl; else cout << "Operation failed" << endl;
	if(bgp_set->neighbors->neighbor[0]->neighbor_address == bgp_read_ptr->neighbors->neighbor[0]->neighbor_address)
		 cout << "Read operation success" << endl; else cout << "Operation failed" << endl;
	if(bgp_set->neighbors->neighbor[0]->config->local_as == bgp_read_ptr->neighbors->neighbor[0]->config->local_as)
		 cout << "Read operation success" << endl; else cout << "Operation failed" << endl;
	if(bgp_set->global_->afi_safis->afi_safi[0]->afi_safi_name == bgp_read_ptr->global_->afi_safis->afi_safi[0]->afi_safi_name)
		 cout << "Read operation success" << endl; else cout << "Operation failed" << endl;
	if(bgp_set->global_->afi_safis->afi_safi[0]->config->afi_safi_name == bgp_read_ptr->global_->afi_safis->afi_safi[0]->config->afi_safi_name)
		 cout << "Read operation success" << endl << endl; else cout << "Operation failed" << endl << endl;
	cout << "=================================================="<<endl<<endl;
}

void run_update_delete(CrudService & crud, NetconfServiceProvider & provider)
{
	auto bgp = make_unique<openconfig_bgp::Bgp>();
	bool reply = crud.del(provider, *bgp);
	if(reply) cout << "Delete operation success" << endl; else cout << "Operation failed" << endl;

	config_bgp(bgp.get());
	reply = crud.create(provider, *bgp);
	if(reply) cout << "Create operation success" << endl; else cout << "Operation failed" << endl;

	config_new_bgp(bgp.get());
	reply = crud.update(provider, *bgp);
	if(reply) cout << "Update operation success" << endl << endl; else cout << "Operation failed" << endl << endl;

        auto bgp_filter = make_unique<openconfig_bgp::Bgp>();
	auto bgp_read = crud.read(provider, *bgp_filter);
	openconfig_bgp::Bgp * bgp_read_ptr = dynamic_cast<openconfig_bgp::Bgp*>(bgp_read.get());
	if(bgp->global_->config->as_ == bgp_read_ptr->global_->config->as_)
		 cout << "Update read operation success" << endl; else cout << "Operation failed" << endl;
	cout << "=================================================="<<endl<<endl;
}

static void show_usage(string name)
{
	cerr << "Usage:\n\t"<< name << " ssh://user:password@host:port" <<endl;
}

static vector<string> parse_args(string arg)
{
	vector<string> ret;
	size_t s = arg.find("ssh://") + sizeof("ssh://")-1;
	size_t col1 = arg.find(":",s);
	size_t amp = arg.find("@")-1;
	size_t col2 = arg.find(":",amp);
	ret.push_back(arg.substr(s,col1-s));
	ret.push_back(arg.substr(col1+1, amp-col1));
	ret.push_back(arg.substr(amp+2, col2-amp-2));
	ret.push_back(arg.substr(col2+1));
	return ret;
}

int main(int argc, char* argv[])
{
	string host, username, password;
	int port;

	if (argc < 2) {
		show_usage(argv[0]);
		return 1;
	}

	string arg = argv[1];
	if ((arg == "-h") || (arg == "--help")) {
		show_usage(argv[0]);
		return 0;
	}
	vector<string> args = parse_args(argv[1]);
	username = args[0]; password = args[1]; host = args[2]; port = stoi(args[3]);

	ydk::core::Repository repo{MODELS_DIR};
	NetconfServiceProvider provider{&repo, host, username, password, port};
	CrudService crud{};

	run_create_delete(crud, provider);
	run_read_delete(crud, provider);
	run_update_delete(crud, provider);
}
