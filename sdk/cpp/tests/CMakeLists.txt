cmake_minimum_required(VERSION 3.6)
cmake_policy(SET CMP0048 NEW)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")
set(netconf_tests bgpnctest;test_netconf_client;testsanitynctest)

include_directories(SYSTEM ${BOOST_INCLUDE_DIR})

find_library(xml2_location xml2)
find_library(curl_location curl)
find_library(ssh_location ssh)
find_library(ssh_threads_location ssh_threads)
find_library(pcre_location pcre)
find_library(xslt_location xslt)
find_package(Boost REQUIRED COMPONENTS unit_test_framework)
find_package(LibYDK REQUIRED)

set(BOOST_INCLUDE_DIRS $boost_installation_prefix/include)

set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0")

foreach(test_name IN LISTS netconf_tests)
    add_executable(${test_name} ${test_name}.cpp)
    set_property(TARGET ${test_name} PROPERTY CXX_STANDARD 14)
    set_property(TARGET ${test_name} PROPERTY CXX_STANDARD_REQUIRED ON)
    
    target_include_directories(${test_name} PRIVATE 
        ${LIBYDK_INCLUDE_DIRS} 
        ${BOOST_INCLUDE_DIRS}
    )
    
    target_compile_definitions(${test_name} PRIVATE 
        "BOOST_TEST_DYN_LINK=1"
    )
    
    target_link_libraries(${test_name} 
        ${xml2_location}
        ${curl_location}
        ${ssh_location}
        ${ssh_threads_location}
        ${pcre_location}
        ${xslt_location}
        ${LIBYDK_LIBRARIES} 
        ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY}
    )

    add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}>)
endforeach(test_name)


configure_file("${PROJECT_SOURCE_DIR}/../ydk/tests/config.hpp.in" "${PROJECT_SOURCE_DIR}/../ydk/tests/config.hpp" ESCAPE_QUOTES @ONLY)
