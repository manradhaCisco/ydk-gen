cmake_minimum_required(VERSION 3.6)
cmake_policy(SET CMP0048 NEW)

set(api_tests bgptest)

include_directories(SYSTEM ${BOOST_INCLUDE_DIR})

find_package(Boost REQUIRED COMPONENTS unit_test_framework)

set(BOOST_INCLUDE_DIRS $boost_installation_prefix/include)

set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0")


foreach(test_name IN LISTS api_tests)
    add_executable(${test_name} api/${test_name}.cpp)
    set_property(TARGET ${test_name} PROPERTY CXX_STANDARD 11)
    set_property(TARGET ${test_name} PROPERTY CXX_STANDARD_REQUIRED ON)
    target_include_directories(${test_name} PRIVATE ${BOOST_INCLUDE_DIRS})
    target_compile_definitions(${test_name} PRIVATE "BOOST_TEST_DYN_LINK=1")
    target_link_libraries(${test_name} ydk ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})
    add_test(NAME ${test_name} COMMAND $<TARGET_FILE:${test_name}>)
endforeach(test_name)

configure_file("${PROJECT_SOURCE_DIR}/tests/config.hpp.in" "${PROJECT_SOURCE_DIR}/tests/config.hpp" ESCAPE_QUOTES @ONLY)
