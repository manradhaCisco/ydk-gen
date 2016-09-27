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

#define BOOST_TEST_MODULE LevelsTests
#include <boost/test/unit_test.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <string.h>
#include <iostream>

#include "ydk/netconf_provider.hpp"
#include "ydk/crud_service.hpp"
#include "ydk_ydktest/ydktest_sanity.hpp"

using namespace ydk;
using namespace std;

#define MODELS_DIR "/Users/abhirame/Cisco/003/ydk-gen/sdk/cpp/ydk/tests/models"

struct YdkTest
{
	YdkTest()
    {
    	BOOST_TEST_MESSAGE( "Setup test" );
    }

    ~YdkTest()
    {
    	BOOST_TEST_MESSAGE( "Teardown test" );
    }
};

BOOST_FIXTURE_TEST_SUITE(sanity_levels, YdkTest )

BOOST_AUTO_TEST_CASE(one_level_pos_set)
{
	boost::log::core::get()->set_filter(
	        boost::log::trivial::severity > boost::log::trivial::debug
	    );
    ydk::core::Repository repo{MODELS_DIR};
    NetconfServiceProvider provider{&repo, "127.0.0.1", "admin", "admin", 12022};
    CrudService crud{};

    auto r_1 = make_unique<ydktest_sanity::Runner>();

    r_1->one->number = 1;
    r_1->one->name = "runner:one:name";
    bool reply = crud.create(provider, *r_1);
    BOOST_REQUIRE(reply);
}

BOOST_AUTO_TEST_CASE(one_level_pos)
{
    ydk::core::Repository repo{MODELS_DIR};
    NetconfServiceProvider provider{&repo, "127.0.0.1", "admin", "admin", 12022};
    CrudService crud{};

    // READ
    auto r_1 = make_unique<ydktest_sanity::Runner>();
    bool reply = crud.delete_(provider, *r_1);
    BOOST_REQUIRE(reply);

    r_1->one->number = 1;
    r_1->one->name = "runner:one:name";
    reply = crud.create(provider, *r_1);
    BOOST_REQUIRE(reply);

    auto filter = make_unique<ydktest_sanity::Runner>();
    auto r_2 = crud.read(provider, *filter);
    BOOST_REQUIRE(r_2!=nullptr);
    ydktest_sanity::Runner * r_2_ptr = dynamic_cast<ydktest_sanity::Runner*>(r_2.get());
    BOOST_REQUIRE(r_2_ptr!=nullptr);

    BOOST_REQUIRE(r_1->one->number == r_2_ptr->one->number);
    BOOST_REQUIRE(r_1->one->name == r_2_ptr->one->name);

    //UPDATE
    r_1 = make_unique<ydktest_sanity::Runner>();
    r_1->one->number = 10;
    r_1->one->name = "runner/one/name";

    filter = make_unique<ydktest_sanity::Runner>();
    reply = crud.update(provider, *r_1);
    r_2 = crud.read(provider, *filter);
    BOOST_REQUIRE(r_2!=nullptr);
    r_2_ptr = dynamic_cast<ydktest_sanity::Runner*>(r_2.get());
    BOOST_REQUIRE(r_2_ptr!=nullptr);

    BOOST_REQUIRE(r_1->one->number == r_2_ptr->one->number);
    BOOST_REQUIRE(r_1->one->name == r_2_ptr->one->name);

    // DELETE
    filter = make_unique<ydktest_sanity::Runner>();
    reply = crud.delete_(provider, *filter);
    BOOST_REQUIRE(reply);
}

BOOST_AUTO_TEST_CASE(two_level_pos)
{
    ydk::core::Repository repo{MODELS_DIR};
    NetconfServiceProvider provider{&repo, "127.0.0.1", "admin", "admin", 12022};
    CrudService crud{};

    // READ
    auto r_1 = make_unique<ydktest_sanity::Runner>();
    r_1->two->number = 2;
    r_1->two->name = "runner:two:name";
    r_1->two->sub1->number = 21;

    bool reply = crud.delete_(provider, *r_1);
    BOOST_REQUIRE(reply);
    reply = crud.create(provider, *r_1);
    BOOST_REQUIRE(reply);

    auto filter = make_unique<ydktest_sanity::Runner>();
    auto r_2 = crud.read(provider, *filter);
    BOOST_REQUIRE(r_2!=nullptr);
    ydktest_sanity::Runner * r_2_ptr = dynamic_cast<ydktest_sanity::Runner*>(r_2.get());
    BOOST_REQUIRE(r_2_ptr!=nullptr);

    BOOST_REQUIRE(r_1->two->number == r_2_ptr->two->number);
    BOOST_REQUIRE(r_1->two->name == r_2_ptr->two->name);
    BOOST_REQUIRE(r_1->two->sub1->number == r_2_ptr->two->sub1->number);

    // UPDATE
    r_1 = make_unique<ydktest_sanity::Runner>();
    r_1->two->number = 20;
    r_1->two->name = "runner/two/name";
    r_1->two->sub1->number = 210;

    filter = make_unique<ydktest_sanity::Runner>();
    reply = crud.update(provider, *r_1);
    r_2 = crud.read(provider, *filter);
    BOOST_REQUIRE(r_2!=nullptr);
    r_2_ptr = dynamic_cast<ydktest_sanity::Runner*>(r_2.get());
    BOOST_REQUIRE(r_2_ptr!=nullptr);

    BOOST_REQUIRE(r_1->two->number == r_2_ptr->two->number);
    BOOST_REQUIRE(r_1->two->name == r_2_ptr->two->name);
    BOOST_REQUIRE(r_1->two->sub1->number == r_1->two->sub1->number);

    // DELETE
    filter = make_unique<ydktest_sanity::Runner>();
    reply = crud.delete_(provider, *filter);
    BOOST_REQUIRE(reply);
}

BOOST_AUTO_TEST_CASE(three_level_pos)
{
    ydk::core::Repository repo{MODELS_DIR};
    NetconfServiceProvider provider{&repo, "127.0.0.1", "admin", "admin", 12022};
    CrudService crud{};

    // READ
    auto r_1 = make_unique<ydktest_sanity::Runner>();
    //bool reply = crud.delete_(provider, *r_1);
    //BOOST_REQUIRE(reply);

    r_1->three->number = 3;
    r_1->three->name = "runner:three:name";
    r_1->three->sub1->number = 31;
    r_1->three->sub1->sub2->number = 311;

    bool reply = crud.create(provider, *r_1);
    BOOST_REQUIRE(reply);

    auto filter = make_unique<ydktest_sanity::Runner>();
    auto r_2 = crud.read(provider, *filter);
    BOOST_REQUIRE(r_2!=nullptr);
    ydktest_sanity::Runner * r_2_ptr = dynamic_cast<ydktest_sanity::Runner*>(r_2.get());
    BOOST_REQUIRE(r_2_ptr!=nullptr);

    BOOST_REQUIRE(r_1->three->number == r_2_ptr->three->number);
    BOOST_REQUIRE(r_1->three->name == r_2_ptr->three->name);
    BOOST_REQUIRE(r_1->three->sub1->number == r_2_ptr->three->sub1->number);
    BOOST_REQUIRE(r_1->three->sub1->sub2->number == r_2_ptr->three->sub1->sub2->number);

    // UPDATE
    r_1 = make_unique<ydktest_sanity::Runner>();
    r_1->three->number = 30;
    r_1->three->name = "runner/three/name";
    r_1->three->sub1->number = 310;
    r_1->three->sub1->sub2->number = 3110;

    filter = make_unique<ydktest_sanity::Runner>();
    reply = crud.update(provider, *r_1);
    r_2 = crud.read(provider, *filter);
    BOOST_REQUIRE(r_2!=nullptr);
    r_2_ptr = dynamic_cast<ydktest_sanity::Runner*>(r_2.get());
    BOOST_REQUIRE(r_2_ptr!=nullptr);

    BOOST_REQUIRE(r_1->three->number == r_2_ptr->three->number);
    BOOST_REQUIRE(r_1->three->name == r_2_ptr->three->name);
    BOOST_REQUIRE(r_1->three->sub1->number == r_2_ptr->three->sub1->number);
    BOOST_REQUIRE(r_1->three->sub1->sub2->number == r_2_ptr->three->sub1->sub2->number);

    // DELETE
    filter = make_unique<ydktest_sanity::Runner>();
    reply = crud.delete_(provider, *filter);
    BOOST_REQUIRE(reply);
}

BOOST_AUTO_TEST_CASE(onelist_neg_dupkey)
{
    ydk::core::Repository repo{MODELS_DIR};
    NetconfServiceProvider provider{&repo, "127.0.0.1", "admin", "admin", 12022};
    CrudService crud{};

    auto r_1 = make_unique<ydktest_sanity::Runner>();
    bool reply = crud.delete_(provider, *r_1);
    BOOST_REQUIRE(reply);

    auto r_2 = make_unique<ydktest_sanity::Runner>();
    auto e_1 = make_unique<ydktest_sanity::Runner::OneList::Ldata>();
    auto e_2 = make_unique<ydktest_sanity::Runner::OneList::Ldata>();
    e_1->number = 1;
    e_2->name = "foo";
    e_2->number = 2;
    e_2->name = "bar";

    e_1->parent = r_1->one_list.get();
	r_1->one_list->ldata.push_back(move(e_1));

    e_2->parent = r_1->one_list.get();
	r_1->one_list->ldata.push_back(move(e_2));

	reply = crud.create(provider, *r_1);
	BOOST_REQUIRE(reply);
	auto r_read = crud.read(provider, *r_2);
	BOOST_REQUIRE(r_read != nullptr);
}

BOOST_AUTO_TEST_CASE(test_onelist_neg_update_key_nonexist)
{
    ydk::core::Repository repo{MODELS_DIR};
    NetconfServiceProvider provider{&repo, "127.0.0.1", "admin", "admin", 12022};
    CrudService crud{};

    //DELETE
    auto r_1 = make_unique<ydktest_sanity::Runner>();
    bool reply = crud.delete_(provider, *r_1);
    BOOST_REQUIRE(reply);

    //CREATE
    auto r_2 = make_unique<ydktest_sanity::Runner>();
    auto e_1 = make_unique<ydktest_sanity::Runner::OneList::Ldata>();
    e_1->parent = r_1->one_list.get();
    e_1->number = 1;
    e_1->name = "foo";
	r_1->one_list->ldata.push_back(move(e_1));
	reply = crud.create(provider, *r_1);
	BOOST_REQUIRE(reply);

	//READ
	auto r_read = crud.read(provider, *r_2);
	BOOST_REQUIRE(r_read != nullptr);

	//UPDATE
	r_1->one_list->ldata[0]->name = "2";
	r_1->one_list->ldata[0]->number = 2;
	reply = crud.update(provider, *r_1);
	BOOST_REQUIRE(reply);
	r_read = crud.read(provider, *r_2);
	BOOST_REQUIRE(r_read != nullptr);
}

BOOST_AUTO_TEST_CASE(test_twolist_pos)
{
    ydk::core::Repository repo{MODELS_DIR};
    NetconfServiceProvider provider{&repo, "127.0.0.1", "admin", "admin", 12022};
    CrudService crud{};

    //DELETE
    auto r_1 = make_unique<ydktest_sanity::Runner>();
    bool reply = crud.delete_(provider, *r_1);
    BOOST_REQUIRE(reply);

    //CREATE
    auto r_2 = make_unique<ydktest_sanity::Runner>();
    auto l_1 = make_unique<ydktest_sanity::Runner::TwoList::Ldata>();
    auto l_2 = make_unique<ydktest_sanity::Runner::TwoList::Ldata>();
    auto s_1 = make_unique<ydktest_sanity::Runner::TwoList::Ldata::Subl1>();
    auto s_2 = make_unique<ydktest_sanity::Runner::TwoList::Ldata::Subl1>();

    l_1->number = 21;
    l_1->name = "21name";
    l_1->parent = r_1->two_list.get();
    l_2->number = 22;
    l_2->name = "22name";
    l_2->parent = r_1->two_list.get();
    s_1->number = 221;
    s_1->name = "221name";
    s_1->parent = l_1.get();
    s_2->number = 222;
    s_2->name = "222name";
    s_2->parent = l_2.get();

    l_1->subl1.push_back(move(s_1));
    l_2->subl1.push_back(move(s_2));
    r_1->two_list->ldata.push_back(move(l_1));
    r_1->two_list->ldata.push_back(move(l_2));

    reply = crud.create(provider, *r_1);
    BOOST_REQUIRE(reply);

    //READ
    auto r_read = crud.read(provider, *r_2);
    BOOST_REQUIRE(r_read != nullptr);

    //UPDATE
    r_1->two_list->ldata[0]->number = 23;
    r_1->two_list->ldata[0]->name = "23name";
    r_1->two_list->ldata[0]->subl1[0]->number = 223;
    r_1->two_list->ldata[0]->subl1[0]->name = "223name";
    r_1->two_list->ldata[1]->number = 24;
    r_1->two_list->ldata[1]->name = "24name";
    r_1->two_list->ldata[1]->subl1[0]->number = 224;
    r_1->two_list->ldata[1]->subl1[0]->name = "224name";
    reply = crud.update(provider, *r_1);
	BOOST_REQUIRE(reply);
	r_read = crud.read(provider, *r_2);
	BOOST_REQUIRE(r_read != nullptr);
}

BOOST_AUTO_TEST_CASE(test_threelist_pos)
{
    ydk::core::Repository repo{MODELS_DIR};
    NetconfServiceProvider provider{&repo, "127.0.0.1", "admin", "admin", 12022};
    CrudService crud{};

    //DELETE
    auto r_1 = make_unique<ydktest_sanity::Runner>();
    bool reply = crud.delete_(provider, *r_1);
    BOOST_REQUIRE(reply);

    //CREATE
    auto r_2 = make_unique<ydktest_sanity::Runner>();
    auto l_1 = make_unique<ydktest_sanity::Runner::ThreeList::Ldata>();
    auto l_2 = make_unique<ydktest_sanity::Runner::ThreeList::Ldata>();
    auto s_1 = make_unique<ydktest_sanity::Runner::ThreeList::Ldata::Subl1>();
    auto s_2 = make_unique<ydktest_sanity::Runner::ThreeList::Ldata::Subl1>();
    auto ss_1 = make_unique<ydktest_sanity::Runner::ThreeList::Ldata::Subl1::SubSubl1>();
    auto ss_2 = make_unique<ydktest_sanity::Runner::ThreeList::Ldata::Subl1::SubSubl1>();

    l_1->number = 21;
    l_1->name = "21name";
    l_1->parent = r_1->three_list.get();
    l_2->number = 22;
    l_2->name = "22name";
    l_2->parent = r_1->three_list.get();
    s_1->number = 221;
    s_1->name = "221name";
    s_1->parent = l_1.get();
    s_2->number = 222;
    s_2->name = "222name";
    s_2->parent = l_2.get();
    ss_1->number = 2221;
    ss_1->parent = s_1.get();
	ss_1->name = "2221name";
	ss_2->number = 2222;
    ss_2->parent = s_2.get();
	ss_2->name = "2222name";

	s_1->sub_subl1.push_back(move(ss_1));
	s_2->sub_subl1.push_back(move(ss_2));
    l_1->subl1.push_back(move(s_1));
    l_2->subl1.push_back(move(s_2));
    r_1->three_list->ldata.push_back(move(l_1));
    r_1->three_list->ldata.push_back(move(l_2));

    reply = crud.create(provider, *r_1);
    BOOST_REQUIRE(reply);

    //READ
    auto r_read = crud.read(provider, *r_2);
    BOOST_REQUIRE(r_read != nullptr);

    //UPDATE
    r_1->three_list->ldata[0]->number = 23;
	r_1->three_list->ldata[0]->name = "23name";
	r_1->three_list->ldata[0]->subl1[0]->number = 223;
	r_1->three_list->ldata[0]->subl1[0]->name = "223name";
	r_1->three_list->ldata[0]->subl1[0]->sub_subl1[0]->number = 2223;
	r_1->three_list->ldata[0]->subl1[0]->sub_subl1[0]->name = "2223name";
	r_1->three_list->ldata[1]->number = 24;
	r_1->three_list->ldata[1]->name = "24name";
	r_1->three_list->ldata[1]->subl1[0]->number = 224;
	r_1->three_list->ldata[1]->subl1[0]->name = "224name";
	r_1->three_list->ldata[1]->subl1[0]->sub_subl1[0]->number = 2224;
	r_1->three_list->ldata[1]->subl1[0]->sub_subl1[0]->name = "2224name";
	reply = crud.update(provider, *r_1);
	BOOST_REQUIRE(reply);
	r_read = crud.read(provider, *r_2);
	BOOST_REQUIRE(r_read != nullptr);
}

BOOST_AUTO_TEST_CASE(test_InbtwList_pos)
{
    ydk::core::Repository repo{MODELS_DIR};
    NetconfServiceProvider provider{&repo, "127.0.0.1", "admin", "admin", 12022};
    CrudService crud{};

    //DELETE
    auto r_1 = make_unique<ydktest_sanity::Runner>();
    bool reply = crud.delete_(provider, *r_1);
    BOOST_REQUIRE(reply);

    //CREATE
    auto r_2 = make_unique<ydktest_sanity::Runner>();
	auto l_1 = make_unique<ydktest_sanity::Runner::InbtwList::Ldata>();
	auto l_2 = make_unique<ydktest_sanity::Runner::InbtwList::Ldata>();
	auto s_1 = make_unique<ydktest_sanity::Runner::InbtwList::Ldata::Subc::SubcSubl1>();
	auto s_2 = make_unique<ydktest_sanity::Runner::InbtwList::Ldata::Subc::SubcSubl1>();

    l_1->number = 21;
    l_1->name = "21name";
    l_1->parent = r_1->inbtw_list.get();
    l_2->number = 22;
    l_2->name = "22name";
    l_2->parent = r_1->inbtw_list.get();
    s_1->number = 221;
    s_1->name = "221name";
    s_1->parent = l_1->subc.get();
    s_2->number = 222;
    s_2->name = "222name";
    s_2->parent = l_2->subc.get();

    l_1->subc->subc_subl1.push_back(move(s_1));
    l_2->subc->subc_subl1.push_back(move(s_2));
    r_1->inbtw_list->ldata.push_back(move(l_1));
    r_1->inbtw_list->ldata.push_back(move(l_2));

    reply = crud.create(provider, *r_1);
    BOOST_REQUIRE(reply);

    //READ
    auto r_read = crud.read(provider, *r_2);
    BOOST_REQUIRE(r_read != nullptr);

    //UPDATE
    r_1->inbtw_list->ldata[0]->number = 221;
    r_1->inbtw_list->ldata[0]->name = "221name";
    r_1->inbtw_list->ldata[0]->subc->subc_subl1[0]->number = 2221;
    r_1->inbtw_list->ldata[0]->subc->subc_subl1[0]->name = "2221name";
    r_1->inbtw_list->ldata[1]->number = 222;
    r_1->inbtw_list->ldata[1]->name = "222name";
    r_1->inbtw_list->ldata[1]->subc->subc_subl1[0]->number = 2222;
    r_1->inbtw_list->ldata[1]->subc->subc_subl1[0]->name = "2222name";
    reply = crud.update(provider, *r_1);
	BOOST_REQUIRE(reply);
	r_read = crud.read(provider, *r_2);
	BOOST_REQUIRE(r_read != nullptr);
}

BOOST_AUTO_TEST_CASE(test_leafref_simple_pos)
{
    ydk::core::Repository repo{MODELS_DIR};
    NetconfServiceProvider provider{&repo, "127.0.0.1", "admin", "admin", 12022};
    CrudService crud{};

    //DELETE
    auto r_1 = make_unique<ydktest_sanity::Runner>();
    bool reply = crud.delete_(provider, *r_1);
    BOOST_REQUIRE(reply);

    //CREATE
    auto r_2 = make_unique<ydktest_sanity::Runner>();

	r_1->ytypes->built_in_t->number8 = 100;
	r_1->ytypes->built_in_t->leaf_ref = r_1->ytypes->built_in_t->number8;
	crud.create(provider, *r_1);

	//READ
	auto r_read = crud.read(provider, *r_2);
	BOOST_REQUIRE(r_read != nullptr);

	//UPDATE
	r_1->ytypes->built_in_t->number8 = 110;
	r_1->ytypes->built_in_t->leaf_ref = r_1->ytypes->built_in_t->number8;
	crud.update(provider, *r_1);
	r_read = crud.read(provider, *r_2);
	BOOST_REQUIRE(r_read != nullptr);
}

BOOST_AUTO_TEST_CASE(test_leafref_pos)
{
	ydk::core::Repository repo{MODELS_DIR};
    NetconfServiceProvider provider{&repo, "127.0.0.1", "admin", "admin", 12022};
    CrudService crud{};

    //DELETE
    auto r_1 = make_unique<ydktest_sanity::Runner>();
    bool reply = crud.delete_(provider, *r_1);
    BOOST_REQUIRE(reply);

    //CREATE
    auto r_2 = make_unique<ydktest_sanity::Runner>();
}

BOOST_AUTO_TEST_SUITE_END()

