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
#include "../../src/core.hpp"
#include "../config.hpp"


namespace mock {
class MockServiceProvider : public ydk::core::ServiceProvider
{
public:
    MockServiceProvider(const std::string searchdir, const std::vector<ydk::core::Capability> capabilities) : m_searchdir{searchdir}, m_capabilities{capabilities}
    {
        
    }
    
	virtual ~MockServiceProvider()
	{

	}

    
	ydk::core::RootSchemaNode* get_root_schema() const
	{
		auto repo = ydk::core::Repository{m_searchdir};
        
		return repo.create_root_schema(m_capabilities);
	}

	ydk::core::DataNode* invoke(ydk::core::Rpc* rpc) const
	{
        auto s = ydk::core::CodecService{};
        
        std::cout << s.encode(rpc->input(), ydk::core::CodecService::Format::XML, true) << std::endl;

		return nullptr;
	}
private:
    std::string m_searchdir;
    std::vector<ydk::core::Capability> m_capabilities;
    
};
}




std::vector<ydk::core::Capability> test_openconfig {
    {"openconfig-bgp-types", "" },
    {"openconfig-bgp", ""},
    {"openconfig-extensions", ""},
    {"openconfig-interfaces", ""},
    {"openconfig-policy-types", ""},
    {"openconfig-routing-policy", ""},
    {"openconfig-types", ""},
    {"ietf-interfaces", ""},
    {"ydk", ""}
    
};
const char* m = "\
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
<neighbor-address\"(!error!)\"\
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

const char* expected_bgp_json = "\
{\
\"openconfig-bgp:bgp\":{\
\"global\":{\
\"config\":{\
\"as\":65172\
},\
\"afi-safis\":{\
\"afi-safi\":[\
{\
\"afi-safi-name\":\"openconfig-bgp-types:L3VPN_IPV4_UNICAST\",\
\"config\":{\
\"afi-safi-name\":\"openconfig-bgp-types:L3VPN_IPV4_UNICAST\",\
\"enabled\":true\
}\
}\
]\
}\
},\
\"neighbors\":{\
\"neighbor\":[\
{\
\"neighbor-address\":\"172.16.255.2\",\
\"config\":{\
\"neighbor-address\":\"172.16.255.2\",\
\"peer-as\":65172\
},\
\"afi-safis\":{\
\"afi-safi\":[\
{\
\"afi-safi-name\":\"openconfig-bgp-types:L3VPN_IPV4_UNICAST\",\
\"config\":{\
\"afi-safi-name\":\"openconfig-bgp-types:L3VPN_IPV4_UNICAST\",\
\"enabled\":true\
}\
}\
]\
}\
}\
]\
}\
}\
}";

const char* expected_bgp_peer_xml = "<bgp xmlns=\"http://openconfig.net/yang/bgp\"><global><config><as>65172</as></config><afi-safis><afi-safi><afi-safi-name xmlns:oc-bgp-types=\"http://openconfig.net/yang/bgp-types\">oc-bgp-types:L3VPN_IPV4_UNICAST</afi-safi-name><config><afi-safi-name xmlns:oc-bgp-types=\"http://openconfig.net/yang/bgp-types\">oc-bgp-types:L3VPN_IPV4_UNICAST</afi-safi-name><enabled>true</enabled></config></afi-safi></afi-safis></global><peer-groups><peer-group><peer-group-name>IBGP</peer-group-name><config><peer-group-name>IBGP</peer-group-name><peer-as>65001</peer-as></config><afi-safis><afi-safi><afi-safi-name xmlns:oc-bgp-types=\"http://openconfig.net/yang/bgp-types\">oc-bgp-types:L3VPN_IPV4_UNICAST</afi-safi-name><config><afi-safi-name xmlns:oc-bgp-types=\"http://openconfig.net/yang/bgp-types\">oc-bgp-types:L3VPN_IPV4_UNICAST</afi-safi-name><enabled>true</enabled></config><apply-policy><config><export-policy>POLICY2</export-policy></config></apply-policy></afi-safi></afi-safis></peer-group></peer-groups><neighbors><neighbor><neighbor-address>172.16.255.2</neighbor-address><config><neighbor-address>172.16.255.2</neighbor-address><peer-group>IBGP</peer-group></config></neighbor></neighbors></bgp>";
const char* expected_bgp_peer_json = "{\"openconfig-bgp:bgp\":{\"global\":{\"config\":{\"as\":65172},\"afi-safis\":{\"afi-safi\":[{\"afi-safi-name\":\"openconfig-bgp-types:L3VPN_IPV4_UNICAST\",\"config\":{\"afi-safi-name\":\"openconfig-bgp-types:L3VPN_IPV4_UNICAST\",\"enabled\":true}}]}},\"peer-groups\":{\"peer-group\":[{\"peer-group-name\":\"IBGP\",\"config\":{\"peer-group-name\":\"IBGP\",\"peer-as\":65001},\"afi-safis\":{\"afi-safi\":[{\"afi-safi-name\":\"openconfig-bgp-types:L3VPN_IPV4_UNICAST\",\"config\":{\"afi-safi-name\":\"openconfig-bgp-types:L3VPN_IPV4_UNICAST\",\"enabled\":true},\"apply-policy\":{\"config\":{\"export-policy\":[\"POLICY2\"]}}}]}}]},\"neighbors\":{\"neighbor\":[{\"neighbor-address\":\"172.16.255.2\",\"config\":{\"neighbor-address\":\"172.16.255.2\",\"peer-group\":\"IBGP\"}}]}}}";

BOOST_AUTO_TEST_CASE( bgp )
{
    std::string searchdir{TEST_HOME};
    mock::MockServiceProvider sp{searchdir, test_openconfig};
    
    std::unique_ptr<ydk::core::RootSchemaNode> schema{sp.get_root_schema()};
   
    BOOST_REQUIRE(schema.get() != nullptr);
        
    auto bgp = schema->create("openconfig-bgp:bgp", "");

    BOOST_REQUIRE( bgp != nullptr );

    //get the root
    std::unique_ptr<const ydk::core::DataNode> data_root{bgp->root()};


    BOOST_REQUIRE( data_root != nullptr );

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

    auto s = ydk::core::CodecService{};
    
    
    //XML Codec Test
    auto xml = s.encode(bgp, ydk::core::CodecService::Format::XML, false);
    
    BOOST_CHECK_MESSAGE( !xml.empty(),
                        "XML output is empty");
    
    BOOST_REQUIRE(xml == expected_bgp_output);
    
    auto new_bgp = s.decode(schema.get(), xml, ydk::core::CodecService::Format::XML);
    
    BOOST_REQUIRE( new_bgp != nullptr);
    
    auto new_xml = s.encode(new_bgp, ydk::core::CodecService::Format::XML, false);
    BOOST_CHECK_MESSAGE(!new_xml.empty(),
                        "Deserialized XML output is empty.");
    
    BOOST_REQUIRE(new_xml == expected_bgp_output);
    
    
    //JSON codec test
    auto json = s.encode(bgp, ydk::core::CodecService::Format::JSON, false);
    
    BOOST_CHECK_MESSAGE( !json.empty(),
                           "JSON output :" << json);
    

    BOOST_REQUIRE(json == expected_bgp_json);
    
    auto new_bgp1 = s.decode(schema.get(), json, ydk::core::CodecService::Format::JSON);
    
    BOOST_REQUIRE( new_bgp1 != nullptr);
    
    auto new_json = s.encode(new_bgp1, ydk::core::CodecService::Format::JSON, false);
    
    
    BOOST_CHECK_MESSAGE(!new_json.empty(),
                        "Deserialized json output is empty.");
    
    BOOST_REQUIRE(new_json == expected_bgp_json);
    
    
    std::unique_ptr<ydk::core::Rpc> create_rpc { schema->rpc("ydk:create") };
    create_rpc->input()->create("entity", xml);

    //call create
    (*create_rpc)(sp);
    
    
}

BOOST_AUTO_TEST_CASE( bgp_validation )
{
    std::string searchdir{TEST_HOME};
    mock::MockServiceProvider sp{searchdir, test_openconfig};
    
    std::unique_ptr<ydk::core::RootSchemaNode> schema{sp.get_root_schema()};
    
    BOOST_REQUIRE(schema.get() != nullptr);
    
    auto bgp = schema->create("openconfig-bgp:bgp", "");
    
    BOOST_REQUIRE( bgp != nullptr );
    
    //get the root
    std::unique_ptr<const ydk::core::DataNode> data_root{bgp->root()};
    
    
    BOOST_REQUIRE( data_root != nullptr );
    
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
   
//TODO: Not working
//    auto neighbor_remove_as = neighbor->create("config/remove-private-as", 
//    "openconfig-bgp-types:PRIVATE_AS_REMOVE_ALL");
    
//    BOOST_REQUIRE( neighbor_remove_as != nullptr );
    
    //bgp/neighbors/neighbor/afi-safis/afi-safi
    auto neighbor_af = neighbor->create("afi-safis/afi-safi[afi-safi-name='openconfig-bgp-types:L3VPN_IPV4_UNICAST']", "");
    
    BOOST_REQUIRE( neighbor_af != nullptr );
    
    auto neighbor_afi_safi_name = neighbor_af->create("config/afi-safi-name" , "openconfig-bgp-types:L3VPN_IPV4_UNICAST");
    
    BOOST_REQUIRE( neighbor_afi_safi_name != nullptr );
    
    auto neighbor_enabled = neighbor_af->create("config/enabled","true");
    
    BOOST_REQUIRE( neighbor_enabled != nullptr );
    
    ydk::core::ValidationService validation_service{};
    
    //validation_service.validate(bgp, ydk::core::ValidationService::Option::EDIT_CONFIG);
    
    
    
}
