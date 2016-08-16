#define BOOST_TEST_MODULE OCBgpTest
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../../src/core.hpp"
#include "../../src/netconf_private.hpp"
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

    
	ydk::core::RootSchemaNode* get_root_schema()
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
    {"ietf-interfaces", ""}
    
};

BOOST_AUTO_TEST_CASE( bgp )
{
    std::string searchdir{TEST_HOME};
    searchdir+="/openconfig";
    std::cout << searchdir << std::endl;
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
    auto xml = s.encode(bgp, ydk::core::CodecService::Format::XML, true);
    auto json = s.encode(bgp, ydk::core::CodecService::Format::JSON, true);
   
    BOOST_CHECK_MESSAGE( !xml.empty(),
                           "XML output :" << xml);
     
    BOOST_CHECK_MESSAGE( !json.empty(),
                           "JSON output :" << json);

      
    
    //codec service bugs
    auto new_bgp = s.decode(schema.get(), xml, ydk::core::CodecService::Format::XML);
    BOOST_REQUIRE( new_bgp != nullptr);
    //if (new_bgp) {
    //    std::cout << "deserialized successfully" << std::endl;
    //}

    //TODO fix rpc
    //std::unique_ptr<ydk::core::Rpc> create_rpc { schema->rpc("ydk:create") };
    //create_rpc->input()->create("config", xml);

    //call create
    //(*create_rpc)(sp);
}

BOOST_AUTO_TEST_CASE( bgp_netconf_create  )
{
	std::string searchdir{TEST_HOME};
	    searchdir+="/openconfig";
	    std::cout << searchdir << std::endl;
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

	    auto s = ydk::core::CodecService{};
	    auto xml = s.encode(bgp, ydk::core::CodecService::Format::XML, true);
	    auto json = s.encode(bgp, ydk::core::CodecService::Format::JSON, true);

	    BOOST_TEST_MESSAGE( xml);
	    BOOST_TEST_MESSAGE(json);

	    BOOST_CHECK_MESSAGE( !xml.empty(),
	                           "XML output :" << xml);

	    BOOST_CHECK_MESSAGE( !json.empty(),
	                           "JSON output :" << json);

	    ydk::NetconfClient client{ "admin", "admin", "127.0.0.1", 12022, 0};
	    client.connect();
	    std::string payload="<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
	       "<edit-config>"
	       "<target><candidate/></target>"
	       "<config>"
	      +xml+
	      "</config>"
	    "</edit-config>"
	    "</rpc>";
	    std::string reply = client.execute_payload(payload);
	    BOOST_TEST_MESSAGE(payload);
	    BOOST_TEST_MESSAGE(reply);

	    BOOST_REQUIRE(NULL != strstr(reply.c_str(), "<ok/>"));

	    reply = client.execute_payload("<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\"><commit/></rpc>");
		BOOST_TEST_MESSAGE(reply);

		BOOST_REQUIRE(NULL != strstr(reply.c_str(), "<ok/>"));

}

//void test_read(ydk::ServiceProvider *sp, ydk::SchemaNode* schema)
//{
//   const char *bgp_xml = "\
//		   <bgp xmlns=\"http://openconfig.net/yang/bgp\">\
//             <global>\
//              <config>\
//               <as>65172</as>\
//              </config>\
//             <afi-safi>\
//              <afi-safi>\
//                <afi-safi-name>l3vpn-ipv4-unicast</afi-safi-name>\
//                <config>\
//                  <afi-safi-name>l3vpn-ipv4-unicast</afi-safi-name>\
//                  <enabled>true</enabled>\
//                 </config>\
//              </afi-safi>\
//            </afi-safis>\
//            </global>
////  <neighbors>
////    <neighbor>
////      <neighbor-address>172.16.255.2</neighbor-address>
////      <config>
////        <neighbor-address>172.16.255.2</neighbor-address>
////        <peer-as>65172</peer-as>
////      </config>
////      <afi-safis>
////        <afi-safi>
////          <afi-safi-name>l3vpn-ipv4-unicast</afi-safi-name>
////          <config>
////            <afi-safi-name>l3vpn-ipv4-unicast</afi-safi-name>
////            <enabled>true</enabled>
////          </config>
////        </afi-safi>
////      </afi-safis>
////    </neighbor>
////  </neighbors>
////</bgp>";
//}


