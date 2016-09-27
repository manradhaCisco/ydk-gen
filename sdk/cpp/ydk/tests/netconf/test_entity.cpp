#define BOOST_TEST_MODULE EntityTest
#include <boost/test/unit_test.hpp>
#include "../../src/types.hpp"

using namespace ydk;
using namespace std;

class TestEntity:public Entity
{
  public:
	TestEntity()
		: name{YType::str, "name"}, enabled{YType::boolean, "enabled"}, bits_field{YType::bits, "bits-field"}
	{
	}

	~TestEntity()
	{
	}

  public:
	bool has_data() const
	{
		return name.is_set || enabled.is_set;
	}

	std::string get_segment_path() const
	{
		return "test";
	}

	EntityPath get_entity_path(Entity* parent) const
	{
		return {{"test"}, {name.get_name_value(), enabled.get_name_value(), bits_field.get_name_value()}};
	}

	Entity* set_child(std::string path)
	{
		return nullptr;
	}

	void set_value(std::string value_path, std::string value)
	{
		if(value_path == "name")
		{
			name = value;
		}
		else if(value_path == "enabled")
		{
			enabled = value;
		}
		else if(value_path == "bits-field")
		{
			bits_field[value] = true;
		}
	}

	class Child:public Entity
	{

	};

  Value name;
  Value enabled;
  Value bits_field;
};

BOOST_AUTO_TEST_CASE(entity)
{
	TestEntity test{};
	string test_value = "value for test";
	EntityPath expected {"test", {{"name", test_value}, {"enabled", "true"}, {"bits-field", "bit1 bit2"}}};

	BOOST_REQUIRE(test.get_entity_path(nullptr).path == "test");
	BOOST_REQUIRE(test.has_data() == false);

	test.name = test_value;
	test.enabled = true;

	test.bits_field["bit1"] = true;
	test.bits_field["bit2"] = true;

	BOOST_REQUIRE(test.has_data() == true);
//	BOOST_REQUIRE(test.get_entity_path(nullptr) == expected); //TODO
}
