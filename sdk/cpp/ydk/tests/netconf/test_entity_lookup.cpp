#define BOOST_TEST_MODULE LookupTest
#include <typeinfo>
#include <boost/test/unit_test.hpp>
#include "../../src/entity.hpp"
#include "../../src/make_unique.hpp"

#include "../../src/top_entity_lookup.hpp"

using namespace ydk;

class Test: public Entity {
public:
    Test() {}
    ~Test() {}
    bool has_data() const {return true;}
    EntityPath get_entity_path() const
    {
        std::string path ("test");
        std::vector<std::pair<std::string, std::string>> value_paths;
        value_paths.push_back(std::make_pair(std::string{"test"}, std::string{"value"}));
        return EntityPath(path, value_paths);
    }
    Entity* set_child(std::string path) {return nullptr;}
    void set_value(std::string value_path, std::string value) {}
};

BOOST_AUTO_TEST_CASE(lookup_routing_policy)
{
    std::unique_ptr<TopEntityLookUp> entity_lookup = TopEntityLookUp::get_entity_lookup();
    std::string path ("/ydk-entity:test");
    entity_lookup->insert(path, std::make_unique<Test>());
    std::unique_ptr<Entity> test_lookup = entity_lookup->lookup(path);
    std::unique_ptr<Entity> test = std::make_unique<Test>();

    BOOST_REQUIRE(typeid(test).name() == typeid (test_lookup).name());
}
