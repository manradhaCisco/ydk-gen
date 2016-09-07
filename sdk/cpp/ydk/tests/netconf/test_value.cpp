#define BOOST_TEST_MODULE ValueTest
#include <boost/test/unit_test.hpp>
#include "../../src/types.hpp"
#include <iostream>

using namespace ydk;

class TestIdentity : public Identity
{
  public:
	TestIdentity() : Identity("test-identity")
	{
	}
};

class TestEnum : public Enum
{
  public:
	TestEnum() : Enum("test-enum")
	{
	}
};

BOOST_AUTO_TEST_CASE(test_uint8)
{
	Value test_value{YType::uint8, "name"};
	test_value = 4;
	BOOST_REQUIRE(test_value.get()=="4");
}

BOOST_AUTO_TEST_CASE(test_uint16)
{
	Value test_value{YType::uint16, "name"};
	test_value = 4;
	BOOST_REQUIRE(test_value.get()=="4");
}


BOOST_AUTO_TEST_CASE(test_uint32)
{
	Value test_value{YType::uint32, "name"};
	test_value = 4;
	BOOST_REQUIRE(test_value.get()=="4");
}


BOOST_AUTO_TEST_CASE(test_uint64)
{
	Value test_value{YType::uint64, "name"};
	test_value = 4;
	BOOST_REQUIRE(test_value.get()=="4");
}


BOOST_AUTO_TEST_CASE(test_int8)
{
	Value test_value{YType::int8, "name"};
	test_value = 4;
	BOOST_REQUIRE(test_value.get()=="4");
}


BOOST_AUTO_TEST_CASE(test_int16)
{
	Value test_value{YType::int16, "name"};
	test_value = 4;
	BOOST_REQUIRE(test_value.get()=="4");
}


BOOST_AUTO_TEST_CASE(test_int32)
{
	Value test_value{YType::int32, "name"};
	test_value = 4;
	BOOST_REQUIRE(test_value.get()=="4");
}


BOOST_AUTO_TEST_CASE(test_int64)
{
	Value test_value{YType::int64, "name"};
	test_value = 4;
	BOOST_REQUIRE(test_value.get()=="4");
}


BOOST_AUTO_TEST_CASE(test_empty)
{
	Value test_value{YType::empty, "name"};
	test_value = Empty{};
	BOOST_REQUIRE(test_value.get()=="name");
}

BOOST_AUTO_TEST_CASE(test_identity)
{
	Value test_value{YType::identityref, "name"};
	test_value = TestIdentity{};
	BOOST_TEST_MESSAGE(test_value.get());
	BOOST_REQUIRE(test_value.get()=="test-identity");
}

//BOOST_AUTO_TEST_CASE(test_enum_)
//{
//	Value test_value{YType::int64, "name"};
//	test_value = TestEnum{};
//	BOOST_TEST_MESSAGE(test_value.get());
//	BOOST_REQUIRE(test_value.get()=="test");
//}

BOOST_AUTO_TEST_CASE(test_str)
{
	Value test_value{YType::str, "name"};
	test_value = "hello";
	BOOST_REQUIRE(test_value.get()=="hello");
}

BOOST_AUTO_TEST_CASE(test_bool)
{
	Value test_value{YType::boolean, "name"};
	test_value = true;
	BOOST_REQUIRE(test_value.get()=="true");

	test_value = false;
	BOOST_REQUIRE(test_value.get()=="false");
}

