cmake_minimum_required(VERSION 3.6)
cmake_policy(SET CMP0048 NEW)
project(bgp_sample)


set(SAMPLE_DESCRIPTION "BGP sample application using YANG Development Kit Library, the library for  YDK API.")


# set default build type if not specified by user
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0")

# Find dependent libraries
find_library(ydk_location ydk)
find_library(ydk_openconfig_location ydk_openconfig)
find_library(netconf_location netconf)
find_library(yang_location yang)

add_executable(bgp_sample bgp_sample.cpp)
target_link_libraries(bgp_sample 
	${ydk_location}
	${ydk_openconfig_location}
	${netconf_location}
	${yang_location}
)

set_property(TARGET bgp_sample PROPERTY CXX_STANDARD 14)
set_property(TARGET bgp_sample PROPERTY CXX_STANDARD_REQUIRED ON)
