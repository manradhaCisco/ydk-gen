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

#define BOOST_TEST_MODULE OCBgpTest
#include <boost/test/unit_test.hpp>
#include <iostream>
#include "../../src/core.hpp"
#include "../config.hpp"
#include "../../src/netconf_provider.hpp"
#include "../../src/entity_data_node_walker.hpp"
#include "../models_gen/ydktest_sanity.h"
#include "../../src/types.hpp"

//test_sanity_types begin
using namespace ydk::ydktest_sanity;

BOOST_AUTO_TEST_CASE( test_sanity_types_int8 )
{
     std::string searchdir{TEST_HOME};
    ydk::core::Repository repo{searchdir};
    
    ydk::NetconfServiceProvider sp{&repo,"127.0.0.1", "admin", "admin",  12022};
 
    ydk::ValidationService validation_service{};

    Runner::Ytypes::BuiltInT builtInT{};
    builtInT.number8 = (int8_t)0;
   
    auto diagnostic = validation_service.validate(sp, builtInT, ydk::ValidationService::Option::EDIT_CONFIG);

    
    BOOST_REQUIRE( !diagnostic.has_errors() );
    
    
}

BOOST_AUTO_TEST_CASE( test_sanity_types_int16 )
{
    std::string searchdir{TEST_HOME};
    ydk::core::Repository repo{searchdir};
    
    ydk::NetconfServiceProvider sp{&repo,"127.0.0.1", "admin", "admin",  12022};
    
    ydk::ValidationService validation_service{};
    
    Runner::Ytypes::BuiltInT builtInT{};
    builtInT.number16 = (int16_t)126;
    
    auto diagnostic = validation_service.validate(sp, builtInT, ydk::ValidationService::Option::EDIT_CONFIG);
    
    
    BOOST_REQUIRE( !diagnostic.has_errors() );
    
}

BOOST_AUTO_TEST_CASE( test_sanity_types_int32 )
{
    std::string searchdir{TEST_HOME};
    ydk::core::Repository repo{searchdir};
    
    ydk::NetconfServiceProvider sp{&repo,"127.0.0.1", "admin", "admin",  12022};
    
    ydk::ValidationService validation_service{};
    
    Runner::Ytypes::BuiltInT builtInT{};
    builtInT.number32 = 200000;
    
    auto diagnostic = validation_service.validate(sp, builtInT, ydk::ValidationService::Option::EDIT_CONFIG);
    
    
    BOOST_REQUIRE( !diagnostic.has_errors() );
    
}

BOOST_AUTO_TEST_CASE( test_sanity_types_int64 )
{

    std::string searchdir{TEST_HOME};
    ydk::core::Repository repo{searchdir};
    
    ydk::NetconfServiceProvider sp{&repo,"127.0.0.1", "admin", "admin",  12022};
    
    ydk::ValidationService validation_service{};
    
    Runner::Ytypes::BuiltInT builtInT{};
    builtInT.number64 = (int64_t)-9223372036854775808ll;
    
    auto diagnostic = validation_service.validate(sp, builtInT, ydk::ValidationService::Option::EDIT_CONFIG);
    
    
    BOOST_REQUIRE( !diagnostic.has_errors() );
    
}

BOOST_AUTO_TEST_CASE( test_sanity_types_uint8 )
{
    std::string searchdir{TEST_HOME};
    ydk::core::Repository repo{searchdir};
    
    ydk::NetconfServiceProvider sp{&repo,"127.0.0.1", "admin", "admin",  12022};
    
    ydk::ValidationService validation_service{};
    
    Runner::Ytypes::BuiltInT builtInT{};
    builtInT.u_number8 = (uint8_t)0;
    
    auto diagnostic = validation_service.validate(sp, builtInT, ydk::ValidationService::Option::EDIT_CONFIG);
    
    
    BOOST_REQUIRE( !diagnostic.has_errors() );}

BOOST_AUTO_TEST_CASE( test_sanity_types_uint16 )
{
    std::string searchdir{TEST_HOME};
    ydk::core::Repository repo{searchdir};
    
    ydk::NetconfServiceProvider sp{&repo,"127.0.0.1", "admin", "admin",  12022};
    
    ydk::ValidationService validation_service{};
    
    Runner::Ytypes::BuiltInT builtInT{};
    builtInT.u_number16 = (uint16_t)65535;
    
    auto diagnostic = validation_service.validate(sp, builtInT, ydk::ValidationService::Option::EDIT_CONFIG);
    
    
    BOOST_REQUIRE( !diagnostic.has_errors() );
    
}

BOOST_AUTO_TEST_CASE( test_sanity_types_uint32 )
{
    std::string searchdir{TEST_HOME};
    ydk::core::Repository repo{searchdir};
    
    ydk::NetconfServiceProvider sp{&repo,"127.0.0.1", "admin", "admin",  12022};
    
    ydk::ValidationService validation_service{};
    
    Runner::Ytypes::BuiltInT builtInT{};
    builtInT.u_number32 = (uint32_t)5927;
    
    auto diagnostic = validation_service.validate(sp, builtInT, ydk::ValidationService::Option::EDIT_CONFIG);
    
    
    BOOST_REQUIRE( !diagnostic.has_errors() );
}

BOOST_AUTO_TEST_CASE( test_sanity_types_uint64 )
{
    
    std::string searchdir{TEST_HOME};
    ydk::core::Repository repo{searchdir};
    
    ydk::NetconfServiceProvider sp{&repo,"127.0.0.1", "admin", "admin",  12022};
    
    ydk::ValidationService validation_service{};
    
    Runner::Ytypes::BuiltInT builtInT{};
    builtInT.u_number64 = (uint64_t)18446744073709551615ull;
    
    auto diagnostic = validation_service.validate(sp, builtInT, ydk::ValidationService::Option::EDIT_CONFIG);
    
    
    BOOST_REQUIRE( !diagnostic.has_errors() );}


BOOST_AUTO_TEST_CASE( test_sanity_types_bits )
{
    //TODO currently there is no way to set Bits in Value
    
    
}

BOOST_AUTO_TEST_CASE( test_sanity_types_decimal64 )
{
  //TODO current no way to set this
    
}

BOOST_AUTO_TEST_CASE( test_sanity_types_string)
{
    std::string searchdir{TEST_HOME};
    ydk::core::Repository repo{searchdir};
    
    ydk::NetconfServiceProvider sp{&repo,"127.0.0.1", "admin", "admin",  12022};
    
    ydk::ValidationService validation_service{};
    
    Runner::Ytypes::BuiltInT builtInT{};
    builtInT.name = "name_str";
    
    auto diagnostic = validation_service.validate(sp, builtInT, ydk::ValidationService::Option::EDIT_CONFIG);
    
    
    BOOST_REQUIRE( !diagnostic.has_errors() );


}

BOOST_AUTO_TEST_CASE( test_sanity_types_empty)
{
    ydk::Empty empty{};
    
    std::string searchdir{TEST_HOME};
    ydk::core::Repository repo{searchdir};
    
    ydk::NetconfServiceProvider sp{&repo,"127.0.0.1", "admin", "admin",  12022};
    
    ydk::ValidationService validation_service{};
    
    Runner::Ytypes::BuiltInT builtInT{};
    builtInT.emptee = empty;
    
    auto diagnostic = validation_service.validate(sp, builtInT, ydk::ValidationService::Option::EDIT_CONFIG);
    
    
    BOOST_REQUIRE( !diagnostic.has_errors() );
    
}

BOOST_AUTO_TEST_CASE( test_sanity_types_boolean)
{
    //TODO current no way to set this
}


BOOST_AUTO_TEST_CASE( test_sanity_types_embedded_enum)
{
    //TODO currently no way to set this
}

BOOST_AUTO_TEST_CASE( test_sanity_types_enum)
{
    //TODO currently no way to set this

    
}

BOOST_AUTO_TEST_CASE( test_sanity_types_union)
{
    //TODO currently no way to set this
    
    
}

BOOST_AUTO_TEST_CASE( test_sanity_types_union_enum)
{
 //TODO currently no way to set this
}

BOOST_AUTO_TEST_CASE( test_sanity_types_union_int)
{
    std::string searchdir{TEST_HOME};
    ydk::core::Repository repo{searchdir};
    
    ydk::NetconfServiceProvider sp{&repo,"127.0.0.1", "admin", "admin",  12022};
    
    ydk::ValidationService validation_service{};
    
    Runner::Ytypes::BuiltInT builtInT{};
    builtInT.enum_int_value = 2;
    
    auto diagnostic = validation_service.validate(sp, builtInT, ydk::ValidationService::Option::EDIT_CONFIG);
    
    
    BOOST_REQUIRE( !diagnostic.has_errors() );
    
}


BOOST_AUTO_TEST_CASE( test_union_leaflist)
{
    std::string searchdir{TEST_HOME};
    ydk::core::Repository repo{searchdir};
    
    ydk::NetconfServiceProvider sp{&repo,"127.0.0.1", "admin", "admin",  12022};
    
    ydk::ValidationService validation_service{};
    
    Runner::Ytypes::BuiltInT builtInT{};
    ydk::Value value1{ydk::YType::int16, "llunion"};
    value1 = (ydk::int16)1;
    
    ydk::Value value2{ydk::YType::int16, "llunion"};
    value2 = (ydk::int16)2;
    
    builtInT.llunion.push_back(value1);
    builtInT.llunion.push_back(value2);
    
    auto diagnostic = validation_service.validate(sp, builtInT, ydk::ValidationService::Option::EDIT_CONFIG);
    
    
    BOOST_REQUIRE( !diagnostic.has_errors() );
    
}

BOOST_AUTO_TEST_CASE( test_enum_leaflist)
{
    //TODO currently no way to set this    
}

BOOST_AUTO_TEST_CASE( test_identity_leaflist)
{
    std::string searchdir{TEST_HOME};
    ydk::core::Repository repo{searchdir};
    
    ydk::NetconfServiceProvider sp{&repo,"127.0.0.1", "admin", "admin",  12022};
    
    ydk::ValidationService validation_service{};
    
    Runner::Ytypes::BuiltInT builtInT{};
    ydk::Value value1{ydk::YType::identityref, "identity-llist"};
    value1 = ydk::ydktest_sanity::ChildIdentityIdentity{};
    
    ydk::Value value2{ydk::YType::identityref, "identity-llist"};
    value2 = ydk::ydktest_sanity::ChildChildIdentityIdentity{};
    
    builtInT.identity_llist.push_back(value1);
    builtInT.identity_llist.push_back(value2);
    
    auto diagnostic = validation_service.validate(sp, builtInT, ydk::ValidationService::Option::EDIT_CONFIG);
    
    BOOST_REQUIRE( !diagnostic.has_errors() );
    
}


BOOST_AUTO_TEST_CASE( test_union_complex_list)
{
    
    std::string searchdir{TEST_HOME};
    ydk::core::Repository repo{searchdir};
    
    ydk::NetconfServiceProvider sp{&repo,"127.0.0.1", "admin", "admin",  12022};
    
    ydk::ValidationService validation_service{};
    
    Runner::Ytypes::BuiltInT builtInT{};
    ydk::Value value{ydk::YType::str, "younion-list"};
    value = "123:45";
    
    builtInT.younion_list.push_back(value);
    
    auto diagnostic = validation_service.validate(sp, builtInT, ydk::ValidationService::Option::EDIT_CONFIG);
    
    BOOST_REQUIRE( !diagnostic.has_errors() );
    
}


BOOST_AUTO_TEST_CASE( test_identityref)
{
    
    std::string searchdir{TEST_HOME};
    ydk::core::Repository repo{searchdir};
    
    ydk::NetconfServiceProvider sp{&repo,"127.0.0.1", "admin", "admin",  12022};
    
    ydk::ValidationService validation_service{};
    
    Runner::Ytypes::BuiltInT builtInT{};
    
    auto identity = ydk::ydktest_sanity::ChildChildIdentityIdentity{};
    builtInT.identity_ref_value = identity;
    
    auto diagnostic = validation_service.validate(sp, builtInT, ydk::ValidationService::Option::EDIT_CONFIG);
    
    BOOST_REQUIRE( !diagnostic.has_errors() );
    
    
}

