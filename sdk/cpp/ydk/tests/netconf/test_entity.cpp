#define BOOST_TEST_MODULE EntityTest
#include <boost/test/unit_test.hpp>
#include "../../src/entity.hpp"
#include "../../src/value.hpp"

using namespace ydk;
using namespace std;

class TestEntity:public Entity
{
  public:
	TestEntity()
		: parent(nullptr), name{Type::str, "name"}
	{
	}

	~TestEntity()
	{
	}

  public:
	bool has_data() const
	{
		return name.is_set;
	}

	EntityPath get_entity_path() const
	{
		return {{"test"}, {name.get_name_value()}};
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
	}

  Entity* parent;

  private:
	Value name;
};

BOOST_AUTO_TEST_CASE(entity)
{
	TestEntity test{};
	string test_value = "value for test";
	EntityPath expected {"test", {{"name", test_value}}};

	BOOST_REQUIRE(test.get_entity_path().path == "test");
	BOOST_REQUIRE(test.has_data() == false);

	test.set_value("name", test_value);
	BOOST_REQUIRE(test.has_data() == true);

	BOOST_REQUIRE(test.get_entity_path() == expected);
}
