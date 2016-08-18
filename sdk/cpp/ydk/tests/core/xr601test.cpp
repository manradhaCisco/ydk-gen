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

#define BOOST_TEST_MODULE XR601Test
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




std::vector<ydk::core::Capability> test_xr601 {
    {"Cisco-IOS-XR-Ethernet-SPAN-cfg", ""},
    {"Cisco-IOS-XR-Ethernet-SPAN-datatypes", ""},
    {"Cisco-IOS-XR-Ethernet-SPAN-oper", ""},
    {"Cisco-IOS-XR-Ethernet-SPAN-subscriber-cfg", ""},
    {"Cisco-IOS-XR-aaa-lib-cfg", ""},
    {"Cisco-IOS-XR-aaa-locald-admin-cfg", ""},
    {"Cisco-IOS-XR-aaa-locald-cfg", ""},
    {"Cisco-IOS-XR-aaa-locald-oper", ""},
    {"Cisco-IOS-XR-aaa-protocol-radius-cfg", ""},
    {"Cisco-IOS-XR-aaa-protocol-radius-oper", ""},
    {"Cisco-IOS-XR-aaa-tacacs-cfg", ""},
    {"Cisco-IOS-XR-aaa-tacacs-oper", ""},
    {"Cisco-IOS-XR-alarmgr-server-oper", ""},
    {"Cisco-IOS-XR-asic-errors-oper", ""},
    {"Cisco-IOS-XR-asr9k-netflow-oper", ""},
    {"Cisco-IOS-XR-asr9k-policymgr-cfg", ""},
    {"Cisco-IOS-XR-asr9k-qos-cfg", ""},
    {"Cisco-IOS-XR-atm-common-datatypes", ""},
    {"Cisco-IOS-XR-atm-vcm-cfg", ""},
    {"Cisco-IOS-XR-atm-vcm-oper" , ""},
    {"Cisco-IOS-XR-bundlemgr-cfg", ""},
    {"Cisco-IOS-XR-bundlemgr-oper", ""},
    {"Cisco-IOS-XR-cdp-cfg", ""},
    {"Cisco-IOS-XR-cdp-oper", ""},
    {"Cisco-IOS-XR-clns-isis-cfg", ""},
    {"Cisco-IOS-XR-clns-isis-datatypes", ""},
    {"Cisco-IOS-XR-clns-isis-oper", ""},
    {"Cisco-IOS-XR-cmproxy-oper", ""},
    {"Cisco-IOS-XR-common-acl-datatypes", ""},
    {"Cisco-IOS-XR-config-mda-cfg", ""},
    {"Cisco-IOS-XR-config-mibs-cfg", ""},
    {"Cisco-IOS-XR-controller-optics-cfg", ""},
    {"Cisco-IOS-XR-controller-optics-oper", ""},
    {"Cisco-IOS-XR-controller-otu-oper", ""},
    {"Cisco-IOS-XR-crypto-macsec-mka-cfg", ""},
    {"Cisco-IOS-XR-crypto-macsec-mka-if-cfg", ""},
    {"Cisco-IOS-XR-crypto-macsec-mka-oper", ""},
    {"Cisco-IOS-XR-crypto-sam-cfg", ""},
    {"Cisco-IOS-XR-crypto-sam-oper", ""},
    {"Cisco-IOS-XR-crypto-ssh-cfg", ""},
    {"Cisco-IOS-XR-crypto-ssh-oper", ""},
    {"Cisco-IOS-XR-dnx-driver-oper", ""},
    {"Cisco-IOS-XR-dnx-netflow-oper", ""},
    {"Cisco-IOS-XR-drivers-media-eth-cfg", ""},
    {"Cisco-IOS-XR-drivers-media-eth-oper", ""},
    //{"Cisco-IOS-XR-es-ace-cfg", ""},
    {"Cisco-IOS-XR-es-acl-cfg", ""},
    {"Cisco-IOS-XR-es-acl-datatypes", ""},
    {"Cisco-IOS-XR-es-acl-oper", ""},
    {"Cisco-IOS-XR-ethernet-cfm-cfg", ""},
    {"Cisco-IOS-XR-ethernet-cfm-datatypes", ""},
    {"Cisco-IOS-XR-ethernet-cfm-oper", ""},
    {"Cisco-IOS-XR-ethernet-link-oam-cfg", ""},
    {"Cisco-IOS-XR-ethernet-link-oam-oper", ""},
    {"Cisco-IOS-XR-ethernet-lldp-cfg", ""},
    {"Cisco-IOS-XR-ethernet-lldp-oper", ""},
    {"Cisco-IOS-XR-fib-common-oper", ""},
    {"Cisco-IOS-XR-flashmib-cfg", ""},
    {"Cisco-IOS-XR-fretta-bcm-dpa-oper", ""},
    {"Cisco-IOS-XR-ha-eem-cfg", ""},
    {"Cisco-IOS-XR-ha-eem-oper", ""},
    {"Cisco-IOS-XR-icpe-infra-cfg", ""},
    {"Cisco-IOS-XR-icpe-infra-oper", ""},
    {"Cisco-IOS-XR-ifmgr-cfg", "" },
    {"Cisco-IOS-XR-ifmgr-oper", "" },
    {"Cisco-IOS-XR-infra-alarm-logger-cfg", "" },
    {"Cisco-IOS-XR-infra-alarm-logger-datatypes", "" },
    {"Cisco-IOS-XR-infra-alarm-logger-oper", "" },
    {"Cisco-IOS-XR-infra-ceredundancymib-cfg", ""},
    {"Cisco-IOS-XR-infra-dumper-cfg", "" },
    {"Cisco-IOS-XR-infra-infra-clock-cfg", "" },
    {"Cisco-IOS-XR-infra-infra-clock-linux-cfg", "" },
    {"Cisco-IOS-XR-infra-infra-locale-cfg", "" },
    {"Cisco-IOS-XR-infra-objmgr-cfg", "" },
    {"Cisco-IOS-XR-infra-objmgr-oper", "" },
    {"Cisco-IOS-XR-infra-policymgr-oper", "" },
    {"Cisco-IOS-XR-infra-rcmd-cfg", "" },
    {"Cisco-IOS-XR-infra-rcmd-oper", "" },
    {"Cisco-IOS-XR-infra-rmf-oper", "" },
    {"Cisco-IOS-XR-infra-rsi-cfg", "" },
    {"Cisco-IOS-XR-infra-rsi-oper", "" },
    {"Cisco-IOS-XR-infra-rsi-subscriber-cfg", "" },
    {"Cisco-IOS-XR-infra-sla-cfg", "" },
    {"Cisco-IOS-XR-infra-sla-datatypes", "" },
    {"Cisco-IOS-XR-infra-sla-oper", "" },
    {"Cisco-IOS-XR-infra-sla-datatypes", ""},
    {"Cisco-IOS-XR-infra-sla-oper", ""},
    {"Cisco-IOS-XR-infra-statsd-cfg", ""},
    {"Cisco-IOS-XR-infra-statsd-oper", ""},
    {"Cisco-IOS-XR-infra-syslog-cfg", ""},
    {"Cisco-IOS-XR-infra-syslog-oper", ""},
    {"Cisco-IOS-XR-infra-systemmib-cfg", ""},
    {"Cisco-IOS-XR-invmgr-oper", ""},
    {"Cisco-IOS-XR-ip-bfd-cfg", ""},
    {"Cisco-IOS-XR-ip-domain-oper", ""},
    {"Cisco-IOS-XR-ip-iarm-datatypes", ""},
    {"Cisco-IOS-XR-ip-icmp-cfg", ""},
    {"Cisco-IOS-XR-ip-iep-cfg", ""},
    {"Cisco-IOS-XR-ip-iep-oper", ""},
    {"Cisco-IOS-XR-ip-mobileip-cfg", ""},
    {"Cisco-IOS-XR-ip-ntp-admin-oper", ""},
    {"Cisco-IOS-XR-ip-ntp-cfg", ""},
    {"Cisco-IOS-XR-ip-ntp-oper", ""},
    {"Cisco-IOS-XR-ip-pfilter-cfg", ""},
    {"Cisco-IOS-XR-ip-pfilter-oper", ""},
    {"Cisco-IOS-XR-ip-pfilter-subscriber-cfg", ""},
    {"Cisco-IOS-XR-ip-rib-cfg", ""},
    {"Cisco-IOS-XR-ip-rsvp-cfg", ""},
    {"Cisco-IOS-XR-ip-rsvp-oper", ""},
    {"Cisco-IOS-XR-ip-static-cfg", ""},
    {"Cisco-IOS-XR-ip-tcp-cfg", ""},
    {"Cisco-IOS-XR-ip-tcp-oper", ""},
    {"Cisco-IOS-XR-ip-udp-cfg", ""},
    //{"Cisco-IOS-XR-ipv4-ace-cfg", ""},
    {"Cisco-IOS-XR-ipv4-acl-cfg", ""},
    {"Cisco-IOS-XR-ipv4-acl-datatypes", ""},
    {"Cisco-IOS-XR-ipv4-acl-oper", ""},
    {"Cisco-IOS-XR-ipv4-arp-cfg", ""},
    {"Cisco-IOS-XR-ipv4-arp-oper", ""},
    {"Cisco-IOS-XR-ipv4-bgp-cfg", ""},
    {"Cisco-IOS-XR-ipv4-bgp-datatypes", ""},
    {"Cisco-IOS-XR-ipv4-bgp-oper", ""},
    {"Cisco-IOS-XR-ipv4-hsrp-cfg", ""},
    {"Cisco-IOS-XR-ipv4-hsrp-oper", ""},
    {"Cisco-IOS-XR-ipv4-io-cfg", ""},
    {"Cisco-IOS-XR-ipv4-io-oper", ""},
    {"Cisco-IOS-XR-ipv4-ma-cfg", ""},
    {"Cisco-IOS-XR-ipv4-ma-oper", ""},
    {"Cisco-IOS-XR-ipv6-ma-subscriber-cfg", ""},
    {"Cisco-IOS-XR-ipv4-ospf-cfg", ""},
    {"Cisco-IOS-XR-ipv4-ospf-oper", ""},
    {"Cisco-IOS-XR-ipv4-telnet-cfg", ""},
    {"Cisco-IOS-XR-ipv4-telnet-mgmt-cfg", ""},
    {"Cisco-IOS-XR-ipv4-vrrp-cfg", ""},
    {"Cisco-IOS-XR-ipv4-vrrp-oper", ""},
    //{"Cisco-IOS-XR-ipv6-ace-cfg", ""},
    {"Cisco-IOS-XR-ipv6-acl-cfg", ""},
    {"Cisco-IOS-XR-ipv6-acl-datatypes", ""},
    {"Cisco-IOS-XR-ipv6-acl-oper", ""},
    {"Cisco-IOS-XR-ipv6-io-cfg", ""},
    {"Cisco-IOS-XR-ipv6-io-oper", ""},
    {"Cisco-IOS-XR-ipv6-ma-cfg", ""},
    {"Cisco-IOS-XR-ipv6-ma-oper", ""},
    {"Cisco-IOS-XR-ipv6-ma-subscriber-cfg", ""},
    {"Cisco-IOS-XR-ipv6-nd-cfg", ""},
    {"Cisco-IOS-XR-ipv6-nd-oper", ""},
    {"Cisco-IOS-XR-ipv6-nd-subscriber-cfg", ""},
    {"Cisco-IOS-XR-ipv6-nd-cfg", ""},
    {"Cisco-IOS-XR-ipv6-nd-oper", ""},
    {"Cisco-IOS-XR-ipv6-nd-subscriber-cfg", ""},
    {"Cisco-IOS-XR-ipv6-new-dhcpv6d-cfg", ""},
    {"Cisco-IOS-XR-ipv6-new-dhcpv6d-oper", ""},
    {"Cisco-IOS-XR-ipv6-ospfv3-cfg", ""},
    {"Cisco-IOS-XR-ipv6-ospfv3-oper", ""},
    {"Cisco-IOS-XR-l2-eth-infra-cfg", ""},
    {"Cisco-IOS-XR-l2-eth-infra-datatypes", ""},
    {"Cisco-IOS-XR-l2-eth-infra-oper", ""},
    {"Cisco-IOS-XR-l2vpn-cfg", ""},
    {"Cisco-IOS-XR-l2vpn-oper", ""},
    {"Cisco-IOS-XR-lib-keychain-cfg", ""},
    {"Cisco-IOS-XR-lib-keychain-macsec-cfg", ""},
    //{"Cisco-IOS-XR-lib-keychain-oper-sub1", ""},
    {"Cisco-IOS-XR-lib-mpp-cfg", ""},
    {"Cisco-IOS-XR-lib-mpp-oper", ""},
    {"Cisco-IOS-XR-linux-os-reboot-history-oper", ""},
    {"Cisco-IOS-XR-lpts-lib-cfg", ""},
    {"Cisco-IOS-XR-lpts-pre-ifib-cfg", ""},
    {"Cisco-IOS-XR-lpts-pre-ifib-oper", ""},
    {"Cisco-IOS-XR-man-ems-cfg", ""},
    {"Cisco-IOS-XR-man-ems-oper", ""},
    {"Cisco-IOS-XR-man-netconf-cfg",""},
    {"Cisco-IOS-XR-man-xml-ttyagent-cfg", ""},
    {"Cisco-IOS-XR-man-xml-ttyagent-oper", ""},
    {"Cisco-IOS-XR-mdrv-lib-cfg", ""},
    {"Cisco-IOS-XR-mpls-ldp-cfg-datatypes", ""},
    {"Cisco-IOS-XR-mpls-ldp-cfg", ""},
    {"Cisco-IOS-XR-mpls-ldp-oper-datatypes", ""},
    {"Cisco-IOS-XR-mpls-ldp-oper", ""},
    {"Cisco-IOS-XR-mpls-lsd-cfg", ""},
    {"Cisco-IOS-XR-mpls-lsd-oper", ""},
    {"Cisco-IOS-XR-mpls-oam-cfg", ""},
    {"Cisco-IOS-XR-mpls-static-cfg", ""},
    {"Cisco-IOS-XR-mpls-static-oper", ""},
    {"Cisco-IOS-XR-mpls-te-cfg", ""},
    {"Cisco-IOS-XR-mpls-te-datatypes", ""},
    {"Cisco-IOS-XR-mpls-te-oper", ""},
    {"Cisco-IOS-XR-mpls-vpn-oper", ""},
    {"Cisco-IOS-XR-ncs5500-policymgr-cfg", ""},
    {"Cisco-IOS-XR-ncs5500-qos-cfg", ""},
    {"Cisco-IOS-XR-ncs5500-qos-oper", ""},
    {"Cisco-IOS-XR-nto-misc-oper", ""},
    {"Cisco-IOS-XR-nto-misc-shprocmem-oper", ""},
    {"Cisco-IOS-XR-optics-driver-cfg", ""},
    {"Cisco-IOS-XR-parser-cfg", ""},
    {"Cisco-IOS-XR-pbr-cfg", ""},
    {"Cisco-IOS-XR-pbr-oper", ""},
    {"Cisco-IOS-XR-pfi-im-cmd-oper", ""},
    {"Cisco-IOS-XR-plat-chas-invmgr-oper", ""},
    {"Cisco-IOS-XR-platform-pifib-oper", ""},
    {"Cisco-IOS-XR-pmengine-cfg", ""},
    {"Cisco-IOS-XR-pmengine-oper", ""},
    {"Cisco-IOS-XR-policy-repository-cfg", ""},
    {"Cisco-IOS-XR-policy-repository-oper", ""},
    {"Cisco-IOS-XR-prm-server-oper", ""},
    {"Cisco-IOS-XR-procmem-oper", ""},
    {"Cisco-IOS-XR-qos-ma-cfg", ""},
    {"Cisco-IOS-XR-qos-ma-oper", ""},
    {"Cisco-IOS-XR-rgmgr-oper", ""},
    {"Cisco-IOS-XR-sdr-invmgr-diag-oper", ""},
    {"Cisco-IOS-XR-sdr-invmgr-oper", ""},
    {"Cisco-IOS-XR-segment-routing-ms-cfg", ""},
    {"Cisco-IOS-XR-segment-routing-ms-oper", ""},
    {"Cisco-IOS-XR-shellutil-cfg", ""},
    {"Cisco-IOS-XR-shellutil-filesystem-oper",""},
    {"Cisco-IOS-XR-shellutil-oper", ""},
    {"Cisco-IOS-XR-show-fpd-loc-ng-oper", ""},
    {"Cisco-IOS-XR-skp-policymgr-cfg", ""},
    {"Cisco-IOS-XR-skp-qos-cfg", ""},
    {"Cisco-IOS-XR-skp-qos-oper", ""},
    {"Cisco-IOS-XR-snmp-agent-cfg", ""},
    {"Cisco-IOS-XR-snmp-agent-oper", ""},
    {"Cisco-IOS-XR-snmp-entitymib-cfg", ""},
    {"Cisco-IOS-XR-snmp-entstatemib-cfg", ""},
    {"Cisco-IOS-XR-snmp-frucontrolmib-cfg", ""},
    {"Cisco-IOS-XR-snmp-ifmib-cfg", ""},
    {"Cisco-IOS-XR-snmp-ifmib-oper", ""},
    {"Cisco-IOS-XR-snmp-syslogmib-cfg", ""},
    {"Cisco-IOS-XR-spirit-corehelper-cfg", ""},
    {"Cisco-IOS-XR-spirit-install-instmgr-oper", ""},
    {"Cisco-IOS-XR-subscriber-infra-tmplmgr-cfg", ""},
    {"Cisco-IOS-XR-traffmon-netflow-cfg", ""},
    {"Cisco-IOS-XR-tty-management-cfg", ""},
    {"Cisco-IOS-XR-tty-management-cmd-oper", ""},
    {"Cisco-IOS-XR-tty-management-datatypes", ""},
    {"Cisco-IOS-XR-tty-management-oper", ""},
    {"Cisco-IOS-XR-tty-server-cfg", ""},
    {"Cisco-IOS-XR-tty-server-oper", ""},
    {"Cisco-IOS-XR-tty-vty-cfg", ""},
    {"Cisco-IOS-XR-types", "" },
    {"Cisco-IOS-XR-wanphy-ui-cfg", ""},
    {"Cisco-IOS-XR-wanphy-ui-oper", ""},
    {"Cisco-IOS-XR-watchd-cfg", ""},
    {"Cisco-IOS-XR-wd-cfg", ""},
    {"Cisco-IOS-XR-wdsysmon-fd-oper", ""},
    {"ietf-interfaces", ""},
    {"cisco-ietf-interfaces-devs", ""},
    //{"cisco-openconfig-if-aggregate-devs", ""},
    //{"cisco-openconfig-if-ethernet-devs", ""},
    //{"cisco-openconfig-interfaces-devs", ""},
    //{"cisco-openconfig-mpls-devs", ""},
    //{"cisco-xr-bgp-deviations", ""},
    //{"cisco-xr-bgp-policy-deviations", ""},
    //{"cisco-xr-netconf-monitoring-deviations", ""},
    //{"cisco-xr-routing-policy-deviations", ""},
    {"ydk", ""}
    
};

BOOST_AUTO_TEST_CASE( xr601 )
{
    std::string searchdir{TEST_HOME};
    searchdir+="/xr601";
    std::cout << searchdir << std::endl;
    mock::MockServiceProvider sp{searchdir, test_xr601};
    
    std::unique_ptr<ydk::core::RootSchemaNode> schema{sp.get_root_schema()};
   
    BOOST_REQUIRE(schema.get() != nullptr);
        
  }


