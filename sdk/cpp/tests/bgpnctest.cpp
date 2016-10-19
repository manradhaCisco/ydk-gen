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

#define BOOST_TEST_MODULE OCBgpTest
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../ydk/src/core.hpp"
#include "../ydk/tests/config.hpp"
#include "../ydk/src/netconf_provider.hpp"

const char* expected_bgp_output ="\
<bgp xmlns=\"http://openconfig.net/yang/bgp\">\
<global>\
<config>\
<as>65172</as>\
</config>\
<afi-safis>\
<afi-safi>\
<afi-safi-name xmlns:oc-bgp-types=\"http://openconfig.net/yang/bgp-types\">oc-bgp-types:L3VPN_IPV4_UNICAST</afi-safi-name>\
<config>\
<afi-safi-name xmlns:oc-bgp-types=\"http://openconfig.net/yang/bgp-types\">oc-bgp-types:L3VPN_IPV4_UNICAST</afi-safi-name>\
<enabled>true</enabled>\
</config>\
</afi-safi>\
</afi-safis>\
</global>\
<neighbors>\
<neighbor>\
<neighbor-address>172.16.255.2</neighbor-address>\
<config>\
<neighbor-address>172.16.255.2</neighbor-address>\
<peer-as>65172</peer-as>\
</config>\
<afi-safis>\
<afi-safi>\
<afi-safi-name xmlns:oc-bgp-types=\"http://openconfig.net/yang/bgp-types\">oc-bgp-types:L3VPN_IPV4_UNICAST</afi-safi-name>\
<config>\
<afi-safi-name xmlns:oc-bgp-types=\"http://openconfig.net/yang/bgp-types\">oc-bgp-types:L3VPN_IPV4_UNICAST</afi-safi-name>\
<enabled>true</enabled>\
</config>\
</afi-safi>\
</afi-safis>\
</neighbor>\
</neighbors>\
</bgp>";


const char* expected_bgp_read ="\
<bgp xmlns=\"http://openconfig.net/yang/bgp\">\
<global>\
<config>\
<as>65172</as>\
</config>\
<use-multiple-paths>\
<state/>\
<ebgp>\
<state/>\
</ebgp>\
<ibgp>\
<state/>\
</ibgp>\
</use-multiple-paths>\
<route-selection-options>\
<state/>\
</route-selection-options>\
<afi-safis>\
<afi-safi>\
<afi-safi-name xmlns:oc-bgp-types=\"http://openconfig.net/yang/bgp-types\">oc-bgp-types:L3VPN_IPV4_UNICAST</afi-safi-name>\
<config>\
<afi-safi-name xmlns:oc-bgp-types=\"http://openconfig.net/yang/bgp-types\">oc-bgp-types:L3VPN_IPV4_UNICAST</afi-safi-name>\
<enabled>true</enabled>\
</config>\
<state/>\
<graceful-restart>\
<state/>\
</graceful-restart>\
<route-selection-options>\
<state/>\
</route-selection-options>\
<use-multiple-paths>\
<state/>\
<ebgp>\
<state/>\
</ebgp>\
<ibgp>\
<state/>\
</ibgp>\
</use-multiple-paths>\
<apply-policy>\
<state/>\
</apply-policy>\
</afi-safi>\
</afi-safis>\
<apply-policy>\
<state/>\
</apply-policy>\
</global>\
<neighbors>\
<neighbor>\
<neighbor-address>172.16.255.2</neighbor-address>\
<config>\
<neighbor-address>172.16.255.2</neighbor-address>\
<peer-as>65172</peer-as>\
</config>\
<state/>\
<timers>\
<state>\
<connect-retry>30.0</connect-retry>\
<hold-time>90.0</hold-time>\
<keepalive-interval>30.0</keepalive-interval>\
<minimum-advertisement-interval>30.0</minimum-advertisement-interval>\
</state>\
</timers>\
<transport>\
<state/>\
</transport>\
<error-handling><state/></error-handling>\
<logging-options><state/></logging-options>\
<ebgp-multihop><state/></ebgp-multihop>\
<route-reflector><state/></route-reflector>\
<as-path-options><state/></as-path-options>\
<add-paths><state/></add-paths>\
<use-multiple-paths><state/>\
<ebgp><state/></ebgp>\
</use-multiple-paths><apply-policy><state/></apply-policy>\
<afi-safis>\
<afi-safi>\
<afi-safi-name xmlns:oc-bgp-types=\"http://openconfig.net/yang/bgp-types\">oc-bgp-types:L3VPN_IPV4_UNICAST</afi-safi-name>\
<config>\
<afi-safi-name xmlns:oc-bgp-types=\"http://openconfig.net/yang/bgp-types\">oc-bgp-types:L3VPN_IPV4_UNICAST</afi-safi-name>\
<enabled>true</enabled>\
</config>\
<state/>\
<graceful-restart><state/></graceful-restart>\
<apply-policy><state/></apply-policy>\
<use-multiple-paths><state/><ebgp><state/></ebgp></use-multiple-paths>\
</afi-safi></afi-safis></neighbor></neighbors></bgp>";

void print_tree(ydk::core::DataNode* dn, const std::string& indent)
{
    ydk::core::Statement s = dn->schema()->statement();
    if(s.keyword == "leaf" || s.keyword == "leaf-list" || s.keyword == "anyxml") {
        auto val = dn->get();
        std::cout << indent << "<" << s.arg << ">" << val << "</" << s.arg << ">" << std::endl;
    } else {
        std::string child_indent{indent};
        child_indent+="  ";
        std::cout << indent << "<" << s.arg << ">" << std::endl;
        for(auto c : dn->children())
            print_tree(c, child_indent);
        std::cout << indent << "</" << s.arg << ">" << std::endl;
        
    }
}



BOOST_AUTO_TEST_CASE( bgp_netconf_create  )
{
    ydk::core::Repository repo{};
    
    ydk::NetconfServiceProvider sp{&repo,"127.0.0.1", "admin", "admin",  12022};
    ydk::core::RootSchemaNode* schema = sp.get_root_schema();

    BOOST_REQUIRE(schema != nullptr);
    
    auto s = ydk::core::CodecService{};

    auto bgp = schema->create("openconfig-bgp:bgp", "");

    BOOST_REQUIRE( bgp != nullptr );

    //get the root
    std::unique_ptr<const ydk::core::DataNode> data_root{bgp->root()};

    BOOST_REQUIRE( data_root != nullptr );
    
    //first delete
    std::unique_ptr<ydk::core::Rpc> delete_rpc { schema->rpc("ydk:delete") };
    
    auto xml = s.encode(bgp, ydk::core::CodecService::Format::XML, false);
    
    delete_rpc->input()->create("entity", xml);
    
    //call delete
    (*delete_rpc)(sp);
    

    auto as = bgp->create("global/config/as", "65172");

    BOOST_REQUIRE( as != nullptr );

    auto l3vpn_ipv4_unicast = bgp->create("global/afi-safis/afi-safi[afi-safi-name='openconfig-bgp-types:L3VPN_IPV4_UNICAST']", "");

    BOOST_REQUIRE( l3vpn_ipv4_unicast != nullptr );


    auto afi_safi_name = l3vpn_ipv4_unicast->create("config/afi-safi-name", "openconfig-bgp-types:L3VPN_IPV4_UNICAST");

    BOOST_REQUIRE( afi_safi_name != nullptr );


    //set the enable flag
    auto enable = l3vpn_ipv4_unicast->create("config/enabled","true");

    BOOST_REQUIRE( enable != nullptr );

    //bgp/neighbors/neighbor
    auto neighbor = bgp->create("neighbors/neighbor[neighbor-address='172.16.255.2']", "");

    BOOST_REQUIRE( neighbor != nullptr );

    auto neighbor_address = neighbor->create("config/neighbor-address", "172.16.255.2");

    BOOST_REQUIRE( neighbor_address != nullptr );

    auto peer_as = neighbor->create("config/peer-as","65172");

    BOOST_REQUIRE( peer_as != nullptr );

    //bgp/neighbors/neighbor/afi-safis/afi-safi
    auto neighbor_af = neighbor->create("afi-safis/afi-safi[afi-safi-name='openconfig-bgp-types:L3VPN_IPV4_UNICAST']", "");

    BOOST_REQUIRE( neighbor_af != nullptr );

    auto neighbor_afi_safi_name = neighbor_af->create("config/afi-safi-name" , "openconfig-bgp-types:L3VPN_IPV4_UNICAST");

    BOOST_REQUIRE( neighbor_afi_safi_name != nullptr );

    auto neighbor_enabled = neighbor_af->create("config/enabled","true");

    BOOST_REQUIRE( neighbor_enabled != nullptr );
    
    xml = s.encode(bgp, ydk::core::CodecService::Format::XML, false);
    
    BOOST_CHECK_MESSAGE( !xml.empty(),
                        "XML output :" << xml);

    BOOST_REQUIRE(xml == expected_bgp_output);
    
    
    //call create
    std::unique_ptr<ydk::core::Rpc> create_rpc { schema->rpc("ydk:create") };
    create_rpc->input()->create("entity", xml);
    (*create_rpc)(sp);
    
    //call read
    std::unique_ptr<ydk::core::Rpc> read_rpc { schema->rpc("ydk:read") };
    auto bgp_read = schema->create("openconfig-bgp:bgp", "");
    BOOST_REQUIRE( bgp_read != nullptr );
    std::unique_ptr<const ydk::core::DataNode> data_root2{bgp_read->root()};
    
    xml = s.encode(bgp_read, ydk::core::CodecService::Format::XML, false);
    BOOST_REQUIRE( !xml.empty() );
    read_rpc->input()->create("filter", xml);
    
    auto read_result = (*read_rpc)(sp);
    
    BOOST_REQUIRE(read_result != nullptr);

    print_tree(read_result,"");
    
    xml = s.encode(read_result, ydk::core::CodecService::Format::XML, false);
    
    std::cout << xml << std::endl;
    
    BOOST_REQUIRE(xml == expected_bgp_read);
    
    peer_as->set("6500");
    
    //call update
    std::unique_ptr<ydk::core::Rpc> update_rpc { schema->rpc("ydk:update") };
    xml = s.encode(bgp, ydk::core::CodecService::Format::XML, false);
    BOOST_REQUIRE( !xml.empty() );
    update_rpc->input()->create("entity", xml);
    (*update_rpc)(sp);
    
    

}

