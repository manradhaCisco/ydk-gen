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
#define BOOST_TEST_MODULE NetconfClientTest
#include <boost/test/unit_test.hpp>
#include <string.h>
#include "../ydk/src/netconf_client.hpp"
#include "../ydk/src/errors.hpp"
#include <iostream>
#include <sys/time.h>  

using namespace ydk;
using namespace std;
#define NC_VERB_VERBOSE 2

BOOST_AUTO_TEST_CASE(xr)
{
	NetconfClient client{ "admin", "admin", "localhost", 1220, 0};
	int OK = 0;

	int result = client.connect();
	BOOST_REQUIRE(result == OK);

    struct timeval t1, t2;
    double elapsedTime;

    // start timer
    gettimeofday(&t1, NULL);

	string reply = client.execute_payload(
	 "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
	 "<get-config>"
	 "<source><candidate/></source>"
	 "</get-config>"
	 "</rpc>");

    // stop timer
    gettimeofday(&t2, NULL);

    // compute and print the elapsed time in millisec
    elapsedTime = (t2.tv_sec - t1.tv_sec) * 1000.0;      // sec to ms
    elapsedTime += (t2.tv_usec - t1.tv_usec) / 1000.0;   // us to ms
    cout << elapsedTime << " ms.\n";



	result = client.close();
	BOOST_REQUIRE(result == OK);
}


BOOST_AUTO_TEST_CASE(Create)
{
	NetconfClient client{ "admin", "admin", "127.0.0.1", 12022, 0};
	int OK = 0;

	int result = client.connect();
	BOOST_REQUIRE(result == OK);

	result = client.close();
	BOOST_REQUIRE(result == OK);
}

BOOST_AUTO_TEST_CASE(EditgetConfig)
{
	NetconfClient client{ "admin", "admin", "127.0.0.1", 12022, 0};
	int OK = 0;

	int result = client.connect();
	BOOST_REQUIRE(result == OK);

	string reply = client.execute_payload(
	 "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
	 "<edit-config>"
	 "<target><candidate/></target>"
	 "<config>"
	 "<runner xmlns=\"http://cisco.com/ns/yang/ydktest-sanity\"><ytypes><built-in-t><number8>12</number8></built-in-t></ytypes></runner>"
	 "</config>"
	 "</edit-config>"
	 "</rpc>");
	BOOST_REQUIRE(NULL != strstr(reply.c_str(), "<ok/>"));

	reply = client.execute_payload(
	 "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
	 "<get-config>"
	 "<source><candidate/></source>"
	 "<filter>"
	 "<runner xmlns=\"http://cisco.com/ns/yang/ydktest-sanity\"/>"
	 "</filter>"
	 "</get-config>"
	 "</rpc>");
	BOOST_REQUIRE(NULL != strstr(reply.c_str(), "<number8>12</number8>"));

	reply = client.execute_payload(
	 "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
	 "<edit-config>"
	 "<target><candidate/></target>"
	 "<config>"
	 "<runner xmlns=\"http://cisco.com/ns/yang/ydktest-sanity\" operation=\"delete\"/>"
	 "</config>"
	 "</edit-config>"
	 "</rpc>");
	BOOST_REQUIRE(NULL != strstr(reply.c_str(), "<ok/>"));

	result = client.close();
	BOOST_REQUIRE(result == OK);
}

BOOST_AUTO_TEST_CASE(Validate)
{
	NetconfClient client{ "admin", "admin", "127.0.0.1", 12022, 0};
	int OK = 0;

	int result = client.connect();
	BOOST_REQUIRE(result == OK);

	string reply = client.execute_payload(
	 "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
	 "<validate>"
	 "<source><candidate/></source>"
	 "</validate>"
	 "</rpc>");

	BOOST_REQUIRE(NULL != strstr(reply.c_str(), "<ok/>"));

	result = client.close();
	BOOST_REQUIRE(result == OK);
}

BOOST_AUTO_TEST_CASE(LockUnlock)
{
	NetconfClient client{ "admin", "admin", "127.0.0.1", 12022, 0};
	int OK = 0;

	int result = client.connect();
	BOOST_REQUIRE(result == OK);

    string reply = client.execute_payload(
                                          "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
                                          "<discard-changes/>"
                                          "</rpc>");

    BOOST_REQUIRE(NULL != strstr(reply.c_str(), "<ok/>"));


	reply = client.execute_payload(
		 "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
		 "<lock>"
		 "<target><candidate/></target>"
		 "</lock>"
		 "</rpc>");

	BOOST_REQUIRE(NULL != strstr(reply.c_str(), "<ok/>"));

	reply = client.execute_payload(
		 "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
		 "<unlock>"
		 "<target><candidate/></target>"
		 "</unlock>"
		 "</rpc>");

	BOOST_REQUIRE(NULL != strstr(reply.c_str(), "<ok/>"));

	reply = client.execute_payload(
		 "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
		 "<unlock>"
		 "<target><candidate/></target>"
		 "</unlock>"
		 "</rpc>");

	BOOST_REQUIRE(NULL != strstr(reply.c_str(), "<rpc-error>"));

	result = client.close();
	BOOST_REQUIRE(result == OK);
}

BOOST_AUTO_TEST_CASE(RpcError)
{
	NetconfClient client{ "admin", "admin", "127.0.0.1", 12022, 0};
	int OK = 0;

	int result = client.connect();
	BOOST_REQUIRE(result == OK);
	string reply = client.execute_payload(
	 "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
	 "<edit-config>"
	 "<target><candidate/></target>"
	 "<config>"
	 "<runner xmlns=\"http://cisco.com/ns/yang/ydktest-sanity\"><ytypes><built-in-t><number8>aaa</number8></built-in-t></ytypes></runner>"
	 "</config>"
	 "</edit-config>"
	 "</rpc>");
	BOOST_REQUIRE(NULL != strstr(reply.c_str(), "<rpc-error>"));

	result = client.close();
	BOOST_REQUIRE(result == OK);
}

BOOST_AUTO_TEST_CASE(DeviceNotConnectedClose)
{
	NetconfClient client{ "admin", "admin", "127.0.0.1", 12022, 0};
	int result = 1;
	try
	{
		result = client.close();
	}
	catch (YDKException & e)
	{
		BOOST_REQUIRE(e.err_msg=="Could not close session. Not connected to 127.0.0.1");
	}


}

BOOST_AUTO_TEST_CASE(DeviceNotConnectedExecute)
{
	NetconfClient client{ "admin", "admin", "127.0.0.1", 12022, 0};
	try
	{
		string s = client.execute_payload(
				 "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
				 "<edit-config>"
				 "<target><candidate/></target>"
				 "<config>"
				 "<runner xmlns=\"http://cisco.com/ns/yang/ydktest-sanity\"><ytypes><built-in-t><number8>aaa</number8></built-in-t></ytypes></runner>"
				 "</config>"
				 "</edit-config>"
				 "</rpc>");
		BOOST_REQUIRE(s== "");
	}
	catch (YDKException & e)
	{
		BOOST_REQUIRE(e.err_msg=="Could not execute payload. Not connected to 127.0.0.1");
	}

}


BOOST_AUTO_TEST_CASE(RpcInvalid)
{
	NetconfClient client{ "admin", "admin", "127.0.0.1", 12022, 0};
	int OK = 0;

	int result = client.connect();
	BOOST_REQUIRE(result == OK);

	try
	{
		string reply = client.execute_payload(
			 "<rpc xmlns=\"urn:ietf:params:xml:ns:netconf:base:1.0\">"
			 "<lock>"
			 "<source><candidate/></source>"
			 "</lock>"
			 "</rpc>");

		BOOST_REQUIRE(NULL != strstr(reply.c_str(), ""));

	}
	catch (YDKException & e)
	{
		BOOST_REQUIRE(e.err_msg=="Could not build payload");
	}

	result = client.close();
	BOOST_REQUIRE(result == OK);
}

BOOST_AUTO_TEST_CASE(WrongXml)
{
	NetconfClient client{ "admin", "admin", "127.0.0.1", 12022, 0};
	int OK = 0;

	int result = client.connect();
	BOOST_REQUIRE(result == OK);

	try
	{
		string reply = client.execute_payload(
				"<testing>"
		 );
		BOOST_REQUIRE(reply== "");
	}
	catch (YDKException & e)
	{
		BOOST_REQUIRE(e.err_msg=="Could not build payload");
	}

	result = client.close();
	BOOST_REQUIRE(result == OK);
}

BOOST_AUTO_TEST_CASE(CorrectXmlWrongRpc)
{
	NetconfClient client{ "admin", "admin", "127.0.0.1", 12022, 0};
	int OK = 0;

	int result = client.connect();
	BOOST_REQUIRE(result == OK);

	try
	{
		string reply = client.execute_payload(
				"<testing/>"
		);
		BOOST_REQUIRE(reply== "");
	}
	catch (YDKException & e)
	{
		BOOST_REQUIRE(e.err_msg=="Could not build payload");
	}


	result = client.close();
	BOOST_REQUIRE(result == OK);
}

BOOST_AUTO_TEST_CASE(EmptyRpc)
{
	NetconfClient client{ "admin", "admin", "127.0.0.1", 12022, 0};
	int OK = 0;

	int result = client.connect();
	BOOST_REQUIRE(result == OK);

	try
	{
		string reply = client.execute_payload("");
		BOOST_REQUIRE(reply== "");
	}
	catch (YDKException & e)
	{
		BOOST_REQUIRE(e.err_msg=="Could not build payload");
	}

	result = client.close();
	BOOST_REQUIRE(result == OK);
}

BOOST_AUTO_TEST_CASE(MultipleClients)
{
	NetconfClient client1{ "admin", "admin", "127.0.0.1", 12022, 0};
	NetconfClient client2{ "admin", "admin", "127.0.0.1", 12022, 0};
	NetconfClient client3{ "admin", "admin", "127.0.0.1", 12022, 0};
	NetconfClient client4{ "admin", "admin", "127.0.0.1", 12022, 0};
	NetconfClient client5{ "admin", "admin", "127.0.0.1", 12022, 0};
	NetconfClient client6{ "admin", "admin", "127.0.0.1", 12022, 0};
	NetconfClient client7{ "admin", "admin", "127.0.0.1", 12022, 0};
	NetconfClient client8{ "admin", "admin", "127.0.0.1", 12022, 0};
	NetconfClient client9{ "admin", "admin", "127.0.0.1", 12022, 0};
	NetconfClient client10{ "admin", "admin", "127.0.0.1", 12022, 0};
	NetconfClient client11{ "admin", "admin", "127.0.0.1", 12022, 0};
	NetconfClient client12{ "admin", "admin", "127.0.0.1", 12022, 0};
	NetconfClient client13{ "admin", "admin", "127.0.0.1", 12022, 0};
	NetconfClient client14{ "admin", "admin", "127.0.0.1", 12022, 0};
	NetconfClient client15{ "admin", "admin", "127.0.0.1", 12022, 0};

	int result = client1.connect() && client2.connect() && client3.connect() && client4.connect() && client5.connect()
		 && client6.connect() && client7.connect() && client8.connect() && client9.connect() && client10.connect()
		 && client11.connect() && client12.connect() && client13.connect() && client14.connect() && client15.connect();
	BOOST_REQUIRE(result == 0);

	result = client1.close() && client2.close() && client3.close() && client4.close() && client5.close()
			 && client6.close() && client7.close() && client8.close() && client9.close() && client10.close()
			 && client11.close() && client12.close() && client13.close() && client14.close() && client15.close();
	BOOST_REQUIRE(result == 0);
}

