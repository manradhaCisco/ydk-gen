#define BOOST_TEST_MODULE ValueTest
#include <boost/test/unit_test.hpp>
#include "../../src/value.hpp"
#include <iostream>

using namespace ydk;

class TestIdentity : public Identity {
	std::string get_tag() override
	{
		return "test";
	}
};

class TestEnum : public Enum {
	std::string  get_tag() override
	{
		return "test";
	}
};

BOOST_AUTO_TEST_CASE(test_uint8)
{
	Value test_value{Type::uint8, "name"};
	test_value = 4;
	BOOST_REQUIRE(test_value.get()=="4");
}

BOOST_AUTO_TEST_CASE(test_uint16)
{
	Value test_value{Type::uint16, "name"};
	test_value = 4;
	BOOST_REQUIRE(test_value.get()=="4");
}


BOOST_AUTO_TEST_CASE(test_uint32)
{
	Value test_value{Type::uint32, "name"};
	test_value = 4;
	BOOST_REQUIRE(test_value.get()=="4");
}


BOOST_AUTO_TEST_CASE(test_uint64)
{
	Value test_value{Type::uint64, "name"};
	test_value = 4;
	BOOST_REQUIRE(test_value.get()=="4");
}


BOOST_AUTO_TEST_CASE(test_int8)
{
	Value test_value{Type::int8, "name"};
	test_value = 4;
	BOOST_REQUIRE(test_value.get()=="4");
}


BOOST_AUTO_TEST_CASE(test_int16)
{
	Value test_value{Type::int16, "name"};
	test_value = 4;
	BOOST_REQUIRE(test_value.get()=="4");
}


BOOST_AUTO_TEST_CASE(test_int32)
{
	Value test_value{Type::int32, "name"};
	test_value = 4;
	BOOST_REQUIRE(test_value.get()=="4");
}


BOOST_AUTO_TEST_CASE(test_int64)
{
	Value test_value{Type::int64, "name"};
	test_value = 4;
	BOOST_REQUIRE(test_value.get()=="4");
}


BOOST_AUTO_TEST_CASE(test_empty)
{
	Value test_value{Type::empty, "name"};
	test_value = Empty{};
	BOOST_TEST_MESSAGE("gggg"+test_value.get());
	BOOST_REQUIRE(test_value.get()=="name");
}

//BOOST_AUTO_TEST_CASE(test_identity)
//{
//	Value test_value{Type::identityref, "name"};
//	test_value = TestIdentity{};
//	BOOST_TEST_MESSAGE(test_value.get());
//	BOOST_REQUIRE(test_value.get()=="test");
//}
//
//BOOST_AUTO_TEST_CASE(test_enum_)
//{
//	Value test_value{Type::int64, "name"};
//	test_value = TestEnum{};
//	BOOST_TEST_MESSAGE(test_value.get());
//	BOOST_REQUIRE(test_value.get()=="test");
//}

BOOST_AUTO_TEST_CASE(test_str)
{
	Value test_value{Type::str, "name"};
	test_value = "hello";
	BOOST_REQUIRE(test_value.get()=="hello");
}

//BOOST_AUTO_TEST_CASE(test_bool)
//{
//	Value test_value{Type::boolean, "name"};
//	test_value = true;
//	BOOST_REQUIRE(test_value.get()=="true");
//}

