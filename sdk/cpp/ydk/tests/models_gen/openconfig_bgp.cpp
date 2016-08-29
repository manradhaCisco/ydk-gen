
#include <sstream>
#include <iostream>
#include "openconfig_bgp.h"
#include "../../src/make_unique.hpp"

namespace ydk {
namespace openconfig_bgp {

Bgp::Global::Config::Config()
	:as_{Type::int32, "as"}, router_id{Type::str, "router-id"}, parent(nullptr)
{
}

Bgp::Global::Config::~Config() {
}

bool Bgp::Global::Config::has_data() const
{
	return as_.is_set || router_id.is_set;
}

EntityPath Bgp::Global::Config::get_entity_path() const
{
	std::ostringstream path;
	path << "config";
	EntityPath entity_path {path.str(), {as_.get_name_value(), router_id.get_name_value()}};
	return entity_path;
}

Entity* Bgp::Global::Config::set_child(std::string path)
{
	return nullptr;
}

void Bgp::Global::Config::set_value(std::string value_path, std::string value)
{
	if(value_path == "/openconfig-bgp:bgp/global/config/as")
	{
		as_ = value;
	}
	else if(value_path == "/openconfig-bgp:bgp/global/config/router-id")
	{
		router_id = value;
	}
}

Bgp::Global::AfiSafis::AfiSafi::Config::Config()
	: afi_safi_name(Type::str, "afi-safi-name"), enabled(Type::boolean, "enabled"), parent(nullptr)
{
}

Bgp::Global::AfiSafis::AfiSafi::Config::~Config() {
}

bool Bgp::Global::AfiSafis::AfiSafi::Config::has_data() const
{
	return afi_safi_name.is_set || enabled.is_set;
}

EntityPath Bgp::Global::AfiSafis::AfiSafi::Config::get_entity_path() const
{
	std::ostringstream path;
	path << "config";
	EntityPath entity_path {path.str(), {afi_safi_name.get_name_value(), enabled.get_name_value()}};
	return entity_path;
}

Entity* Bgp::Global::AfiSafis::AfiSafi::Config::set_child(std::string path)
{
	return nullptr;
}

void Bgp::Global::AfiSafis::AfiSafi::Config::set_value(std::string value_path, std::string value)
{
	if(value_path ==  "/openconfig-bgp:bgp/global/afi-safis/afi-safi/config/afi-safi-name")
	{
		afi_safi_name = value;
	}
	else if(value_path ==  "/openconfig-bgp:bgp/global/afi-safis/afi-safi/config/enabled")
	{
		enabled = value;
	}
}

Bgp::Global::AfiSafis::AfiSafi::AfiSafi()
	: afi_safi_name(Type::str, "afi-safi-name"), parent(nullptr)
{
	config = std::make_unique<Bgp::Global::AfiSafis::AfiSafi::AfiSafi::Config>();
	config->parent=this;

	add_child(config.get());
}

Bgp::Global::AfiSafis::AfiSafi::~AfiSafi() {
}

bool Bgp::Global::AfiSafis::AfiSafi::has_data() const
{
	return afi_safi_name.is_set || config->has_data();
}

EntityPath Bgp::Global::AfiSafis::AfiSafi::get_entity_path() const
{
	std::ostringstream path;
	path << "afi-safi" << "[afi-safi-name='" << afi_safi_name.get() << "']";
	EntityPath entity_path {path.str(), {afi_safi_name.get_name_value()}};
	return entity_path;
}

Entity* Bgp::Global::AfiSafis::AfiSafi::set_child(std::string path)
{
	if(path == "/openconfig-bgp:bgp/global/afi-safis/afi-safi/config")
	{
		config = std::make_unique<Bgp::Global::AfiSafis::AfiSafi::AfiSafi::Config>();
		config->parent=this;
		add_child(config.get());
		return config.get();
	}
	return nullptr;
}

void Bgp::Global::AfiSafis::AfiSafi::set_value(std::string value_path, std::string value)
{
	if(value_path ==  "/openconfig-bgp:bgp/global/afi-safis/afi-safi/afi-safi-name")
	{
		afi_safi_name = value;
	}
}

Bgp::Global::AfiSafis::AfiSafis()
	:  parent(nullptr)
{
}

Bgp::Global::AfiSafis::~AfiSafis() {
}

bool Bgp::Global::AfiSafis::has_data() const
{
	for (std::size_t index=0; index<afi_safi.size(); index++)
	{
		if (afi_safi[index]->has_data())
		{
			return true;
		}
	}
	return false;
}

EntityPath Bgp::Global::AfiSafis::get_entity_path() const
{
	std::ostringstream path;
	path << "afi-safis";
	EntityPath entity_path {path.str(), {}};
	return entity_path;
}

std::vector<Entity*> & Bgp::Global::AfiSafis::get_children()
{
	for (std::size_t index=0; index<afi_safi.size(); index++)
	{
		children.push_back(afi_safi[index].get());
	}
	return children;
}

Entity* Bgp::Global::AfiSafis::set_child(std::string path)
{
	if(path == "/openconfig-bgp:bgp/global/afi-safis/afi-safi")
	{
		auto child = std::make_unique<Bgp::Global::AfiSafis::AfiSafi>();
		Entity* child_ptr = child.get();
		add_child(child_ptr);
		afi_safi.push_back(std::move(child));
		return child_ptr;
	}
	return nullptr;
}

void Bgp::Global::AfiSafis::set_value(std::string value_path, std::string value)
{

}

Bgp::Global::Global()
	: parent(nullptr)
{
	config = std::make_unique<Bgp::Global::Config>();
	config->parent=this;

	afi_safis = std::make_unique<Bgp::Global::AfiSafis>();
	afi_safis->parent=this;

	add_child(config.get());
	add_child(afi_safis.get());
}

Bgp::Global::~Global() {
}

bool Bgp::Global::has_data() const
{
	return config->has_data();
}

EntityPath Bgp::Global::get_entity_path() const
{
	std::ostringstream path;
	path << "global";
	EntityPath entity_path {path.str(), {}};
	return entity_path;
}

Entity* Bgp::Global::set_child(std::string path)
{
	if(path == "/openconfig-bgp:bgp/global/config")
	{
		config= std::make_unique<Bgp::Global::Config>();
		config->parent=this;
		add_child(config.get());
		return config.get();
	}
	else if(path == "/openconfig-bgp:bgp/global/afi-safis")
	{
		afi_safis = std::make_unique<Bgp::Global::AfiSafis>();
		afi_safis->parent=this;
		add_child(afi_safis.get());
		return afi_safis.get();
	}
	return nullptr;
}

void Bgp::Global::set_value(std::string value_path, std::string value)
{
}

Bgp::Neighbors::Neighbor::Config::Config()
	:  neighbor_address(Type::str, "neighbor-address"), enabled(Type::boolean, "enabled"), local_as(Type::int32, "local-as"), peer_as(Type::int32, "peer-as"), parent(nullptr)
{
}

Bgp::Neighbors::Neighbor::Config::~Config() {
}

bool Bgp::Neighbors::Neighbor::Config::has_data() const
{
	return neighbor_address.is_set || enabled.is_set || local_as.is_set || peer_as.is_set;
}

EntityPath Bgp::Neighbors::Neighbor::Config::get_entity_path() const
{
	std::ostringstream path;
	path << "config";
	EntityPath entity_path {path.str(), {neighbor_address.get_name_value(), enabled.get_name_value(), local_as.get_name_value(), peer_as.get_name_value()}};
	return entity_path;
}

Entity* Bgp::Neighbors::Neighbor::Config::set_child(std::string path)
{
	return nullptr;
}

void Bgp::Neighbors::Neighbor::Config::set_value(std::string value_path, std::string value)
{
	if(value_path == "/openconfig-bgp:bgp/neighbors/neighbor/config/neighbor-address")
	{
		neighbor_address = value;
	}
	else if(value_path == "/openconfig-bgp:bgp/neighbors/neighbor/config/enabled")
	{
		enabled = value;
	}
	else if(value_path == "/openconfig-bgp:bgp/neighbors/neighbor/config/local-as")
	{
		local_as = value;
	}
	else if(value_path == "/openconfig-bgp:bgp/neighbors/neighbor/config/peer-as")
	{
		peer_as = value;
	}
}

Bgp::Neighbors::Neighbor::Neighbor()
	:  neighbor_address(Type::str, "neighbor-address"), parent(nullptr)
{
	config = std::make_unique<Bgp::Neighbors::Neighbor::Config>();
	config->parent = this;
	add_child(config.get());
}

Bgp::Neighbors::Neighbor::~Neighbor() {
}

bool Bgp::Neighbors::Neighbor::has_data() const
{
	return neighbor_address.is_set;
}

EntityPath Bgp::Neighbors::Neighbor::get_entity_path() const
{
	std::ostringstream path;
	path << "neighbor" << "[neighbor-address='" << neighbor_address.get() << "']";
	EntityPath entity_path {path.str(), {neighbor_address.get_name_value()}};
	return entity_path;
}

Entity* Bgp::Neighbors::Neighbor::set_child(std::string path)
{
	if(path == "/openconfig-bgp:bgp/neighbors/neighbor/config")
	{
		config = std::make_unique<Bgp::Neighbors::Neighbor::Config>();
		add_child(config.get());
		return config.get();
	}
	return nullptr;
}

void Bgp::Neighbors::Neighbor::set_value(std::string value_path, std::string value)
{
	if(value_path == "/openconfig-bgp:bgp/neighbors/neighbor/neighbor-address")
	{
		neighbor_address = value;
	}
}

Bgp::Neighbors::Neighbors()
	: parent(nullptr)
{
}

Bgp::Neighbors::~Neighbors() {
}

bool Bgp::Neighbors::has_data() const
{
	for (std::size_t index=0; index<neighbor.size(); index++)
	{
		if (neighbor[index]->has_data())
		{
			return true;
		}
	}
	return false;
}

EntityPath Bgp::Neighbors::get_entity_path() const
{
	std::ostringstream path;
	path << "neighbors";
	EntityPath entity_path {path.str(), {}};
	return entity_path;
}

std::vector<Entity*> &  Bgp::Neighbors::get_children()
{
	for (std::size_t index=0; index<neighbor.size(); index++)
	{
		children.push_back(neighbor[index].get());
	}
	return children;
}

Entity* Bgp::Neighbors::set_child(std::string path)
{
	if(path == "/openconfig-bgp:bgp/neighbors/neighbor")
	{
		auto child = std::make_unique<Bgp::Neighbors::Neighbor>();
		Entity* child_ptr = child.get();
		add_child(child_ptr);
		neighbor.push_back(std::move(child));
		return child_ptr;
	}
	return nullptr;
}

void Bgp::Neighbors::set_value(std::string value_path, std::string value)
{

}

Bgp::PeerGroups::PeerGroup::Config::Config()
	: auth_password(Type::str, "auth-password"), description(Type::str, "description"), peer_group_name(Type::str, "peer-group-name"), local_as(Type::int32, "local-as"), peer_as(Type::int32, "peer-as"), parent(nullptr)
{
}

Bgp::PeerGroups::PeerGroup::Config::~Config() {
}

bool Bgp::PeerGroups::PeerGroup::Config::has_data() const
{
	return auth_password.is_set || description.is_set || local_as.is_set || peer_as.is_set || peer_group_name.is_set;
}

EntityPath Bgp::PeerGroups::PeerGroup::Config::get_entity_path() const
{
	std::ostringstream path;
	path << "config";
	EntityPath entity_path {path.str(), {auth_password.get_name_value(), description.get_name_value(), local_as.get_name_value(), peer_as.get_name_value(), peer_group_name.get_name_value()}};
	return entity_path;
}

Entity* Bgp::PeerGroups::PeerGroup::Config::set_child(std::string path)
{
	return nullptr;
}

void Bgp::PeerGroups::PeerGroup::Config::set_value(std::string value_path, std::string value)
{
	if(value_path == "/openconfig-bgp:bgp/peer-groups/peer-group/config/peer-group-name")
	{
		peer_group_name = value;
	}
	else if(value_path == "/openconfig-bgp:bgp/peer-groups/peer-group/config/auth-password")
	{
		auth_password = value;
	}
	else if(value_path == "/openconfig-bgp:bgp/peer-groups/peer-group/config/local-as")
	{
		local_as = value;
	}
	else if(value_path == "/openconfig-bgp:bgp/peer-groups/peer-group/config/peer-as")
	{
		peer_as = value;
	}
	else if(value_path == "/openconfig-bgp:bgp/peer-groups/peer-group/config/description")
	{
		description = value;
	}
}

Bgp::PeerGroups::PeerGroup::PeerGroup()
	:  peer_group_name(Type::str, "peer-group-name"), parent(nullptr)
{
	config = std::make_unique<Bgp::PeerGroups::PeerGroup::Config>();
	config->parent = this;
	add_child(config.get());
}

Bgp::PeerGroups::PeerGroup::~PeerGroup() {
}

bool Bgp::PeerGroups::PeerGroup::has_data() const
{
	return peer_group_name.is_set;
}

EntityPath Bgp::PeerGroups::PeerGroup::get_entity_path() const
{
	std::ostringstream path;
	path << "peer-group" << "[peer-group-name='" << peer_group_name.get() << "']";
	EntityPath entity_path {path.str(), {peer_group_name.get_name_value()}};
	return entity_path;
}

Entity* Bgp::PeerGroups::PeerGroup::set_child(std::string path)
{
	if(path == "/openconfig-bgp:bgp/peer-groups/peer-group/config")
	{
		config = std::make_unique<Bgp::PeerGroups::PeerGroup::Config>();
		add_child(config.get());
		return config.get();
	}
	return nullptr;
}

void Bgp::PeerGroups::PeerGroup::set_value(std::string value_path, std::string value)
{
	if(value_path == "/openconfig-bgp:bgp/peer-groups/peer-group/peer-group-name")
	{
		peer_group_name = value;
	}
}

Bgp::PeerGroups::PeerGroups()
	: parent(nullptr)
{
}

Bgp::PeerGroups::~PeerGroups() {
}

bool Bgp::PeerGroups::has_data() const
{
	for (std::size_t index=0; index<peer_group.size(); index++)
	{
		if (peer_group[index]->has_data())
		{
			return true;
		}
	}
	return false;
}

EntityPath Bgp::PeerGroups::get_entity_path() const
{
	std::ostringstream path;
	path << "peer-groups";
	EntityPath entity_path {path.str(), {}};
	return entity_path;
}

std::vector<Entity*> &  Bgp::PeerGroups::get_children()
{
	for (std::size_t index=0; index<peer_group.size(); index++)
	{
		children.push_back(peer_group[index].get());
	}
	return children;
}

Entity* Bgp::PeerGroups::set_child(std::string path)
{
	if(path == "/openconfig-bgp:bgp/peer-groups/peer-group")
	{
		auto child = std::make_unique<Bgp::PeerGroups::PeerGroup>();
		Entity* child_ptr = child.get();
		add_child(child_ptr);
		peer_group.push_back(std::move(child));
		return child_ptr;
	}
	return nullptr;
}

void Bgp::PeerGroups::set_value(std::string value_path, std::string value)
{
}

Bgp::Bgp()
	: parent(nullptr)
{
    global_ = std::make_unique<Bgp::Global>();
    global_->parent=this;

    neighbors = std::make_unique<Bgp::Neighbors>();
    neighbors->parent=this;

    peer_groups = std::make_unique<Bgp::PeerGroups>();
    peer_groups->parent=this;

    add_child(global_.get());
    add_child(neighbors.get());
}

Bgp::~Bgp() {
}

bool Bgp::has_data() const
{
	return global_->has_data()  || neighbors->has_data();
}

EntityPath Bgp::get_entity_path() const
{
	std::ostringstream path;
	path << "openconfig-bgp:bgp";
	EntityPath entity_path {path.str(), {}};
	return entity_path;
}

Entity* Bgp::set_child(std::string path)
{
	if(path == "/openconfig-bgp:bgp/global")
	{
		global_ = std::make_unique<Bgp::Global>();
		global_->parent=this;
		add_child(global_.get());
		return global_.get();
	}
	else if(path == "/openconfig-bgp:bgp/neighbors")
	{
		neighbors = std::make_unique<Bgp::Neighbors>();
		neighbors->parent=this;
		add_child(neighbors.get());
		return neighbors.get();
	}
	else if(path == "/openconfig-bgp:bgp/peer-groups")
	{
		peer_groups = std::make_unique<Bgp::PeerGroups>();
		peer_groups->parent=this;
		add_child(peer_groups.get());
		return peer_groups.get();
	}
	return nullptr;
}

void Bgp::set_value(std::string value_path, std::string value)
{

}

std::unique_ptr<Entity> Bgp::clone_ptr()
{
	return std::make_unique<Bgp>();
}

}
}
