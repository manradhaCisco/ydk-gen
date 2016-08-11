//
//  bgp.cpp
//  ydk
//
//  Created by Manu Radhakrishna on 7/25/16.
//  Copyright © 2016 cisco. All rights reserved.
//



#include <iostream>
#include "../src/ydk.hpp"

const char* TEST_HOME = "/Users/manradha/Development/github/manradhaCisco-ydk-gen/test/yang";

namespace mock {
class MockServiceProvider : public ydk::ServiceProvider
{
public:
    MockServiceProvider(const std::string searchdir, const std::vector<ydk::Capability> capabilities) : m_searchdir{searchdir}, m_capabilities{capabilities}
    {
        
    }
    
	virtual ~MockServiceProvider()
	{

	}

    
	ydk::RootSchemaNode* get_root_schema()
	{
		auto repo = ydk::Repository{m_searchdir};
        
        

		return repo.create_root_schema(m_capabilities);
	}

	ydk::DataNode* invoke(ydk::Rpc* rpc) const
	{
        auto s = ydk::CodecService{};
        
        std::cout << s.encode(rpc->input(), ydk::CodecService::Format::XML, true) << std::endl;

		return nullptr;
	}
private:
    std::string m_searchdir;
    std::vector<ydk::Capability> m_capabilities;
    
};
}



std::vector<ydk::Capability> test_capabilities{
    {"inherit", "2015-11-17"},
    {"main-aug1", "2015-11-17"},
    {"main-aug2", "2015-11-17"},
    {"main", "2015-11-17"},
    {"oc-pattern", "2015-11-17"},
    {"ydktest-filterread", "2015-11-17"},
    //{"ydktest-sanity-augm", "2015-11-17"},
    //{"ydktest-sanity-submodule","2016-04-25"},
    //{"ydktest-sanity-types", "2016-04-11"},
    {"ydktest-sanity", "2015-11-17"},
    {"ydktest-types", "2016-05-23"}
};

std::vector<ydk::Capability> test_openconfig {
    {"openconfig-bgp-types", "" },
    {"openconfig-bgp", ""},
    {"openconfig-extensions", ""},
    {"openconfig-interfaces", ""},
    {"openconfig-policy-types", ""},
    {"openconfig-routing-policy", ""},
    {"openconfig-types", ""},
    {"ietf-interfaces", ""}
    
};

void print_paths(ydk::SchemaNode* sn)
{
    std::cout << sn->path() << std::endl;
    for(auto p : sn->children())
        print_paths(p);
}

void test_bgp_create()
{
    std::string searchdir{TEST_HOME};
    searchdir+="/openconfig";
    mock::MockServiceProvider sp{searchdir, test_openconfig};
    
    std::unique_ptr<ydk::RootSchemaNode> schema{sp.get_root_schema()};
    
    print_paths(schema.get());
        
    auto bgp = schema->create("openconfig-bgp:bgp", "");

    //get the root
    std::unique_ptr<const ydk::DataNode> data_root{bgp->root()};

    bgp->create("global/config/as", "65172");
    

    auto l3vpn_ipv4_unicast = bgp->create("global/afi-safis/afi-safi[afi-safi-name='openconfig-bgp-types:L3VPN_IPV4_UNICAST']", "");
    l3vpn_ipv4_unicast->create("config/afi-safi-name", "openconfig-bgp-types:L3VPN_IPV4_UNICAST");


    //set the enable flag
    l3vpn_ipv4_unicast->create("config/enabled","true");

    //bgp/neighbors/neighbor
    auto neighbor = bgp->create("neighbors/neighbor[neighbor-address='172.16.255.2']", "");
    neighbor->create("config/neighbor-address", "172.16.255.2");
    neighbor->create("config/peer-as","65172");


    //bgp/neighbors/neighbor/afi-safis/afi-safi
    auto neighbor_af = neighbor->create("afi-safis/afi-safi[afi-safi-name='openconfig-bgp-types:L3VPN_IPV4_UNICAST']", "");
    neighbor_af->create("config/afi-safi-name" , "openconfig-bgp-types:L3VPN_IPV4_UNICAST");
    neighbor_af->create("config/enabled","true");

    auto s = ydk::CodecService{};
    auto xml = s.encode(bgp, ydk::CodecService::Format::XML, true);
    auto json = s.encode(bgp, ydk::CodecService::Format::JSON, true);
    
    std::cout << "Testing encoding" << std::endl;
    
    std::cout << "*********************************************************" << std::endl;
    std::cout << "*********************************************************" << std::endl;
    std::cout <<  xml << std::endl;
    std::cout << "*********************************************************" << std::endl;
    std::cout << "*********************************************************" << std::endl;
    std::cout <<  json << std::endl;
    std::cout << "*********************************************************" << std::endl;
    std::cout << "*********************************************************" << std::endl;
    
    std::cout << "Testing decoding" << std::endl;
    
    //todo enable after fixing bugs
    
    //codec service bugs
    auto new_bgp = s.decode(schema.get(), xml, ydk::CodecService::Format::XML);
    
    //TODO fix rpc
    std::unique_ptr<ydk::Rpc> create_rpc { schema->rpc("ydk:create") };
    create_rpc->input()->create("config", xml);

    //call create
    (*create_rpc)(sp);
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




int main() {

	
    
    //create
    test_bgp_create();



    //begin read


	return 0;
}
