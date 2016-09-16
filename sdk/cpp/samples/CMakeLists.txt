cmake_minimum_required(VERSION 3.6)
cmake_policy(SET CMP0048 NEW)
project(bgp_samples)


set(SAMPLE_DESCRIPTION "BGP sample application using YANG Development Kit Library, the library for YDK API.")


# set default build type if not specified by user
set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0")

# Find dependent libraries
find_library(ydk_location ydk)
find_library(ydk_openconfig_location ydk_openconfig)
find_library(xml2_location xml2)
find_library(curl_location curl)
find_library(ssh_location ssh)
find_library(ssh_threads_location ssh_threads)
find_library(pcre_location pcre)
find_library(xslt_location xslt)

add_executable(bgp_create bgp_create.cpp args_parser.cpp)
add_executable(bgp_read bgp_read.cpp args_parser.cpp)
add_executable(bgp_update bgp_update.cpp args_parser.cpp)
add_executable(bgp_delete bgp_delete.cpp args_parser.cpp)

target_link_libraries(bgp_create 
	${ydk_location} 
	${ydk_ydktest_location} 
	${xml2_location}
	${curl_location}
	${ssh_location}
	${ssh_threads_location}
	${pcre_location}
	${xslt_location})

target_link_libraries(bgp_read
	${ydk_location} 
	${ydk_ydktest_location} 
	${xml2_location}
	${curl_location}
	${ssh_location}
	${ssh_threads_location}
	${pcre_location}
	${xslt_location})

target_link_libraries(bgp_update
	${ydk_location} 
	${ydk_ydktest_location} 
	${xml2_location}
	${curl_location}
	${ssh_location}
	${ssh_threads_location}
	${pcre_location}
	${xslt_location})

target_link_libraries(bgp_delete
	${ydk_location} 
	${ydk_ydktest_location} 
	${xml2_location}
	${curl_location}
	${ssh_location}
	${ssh_threads_location}
	${pcre_location}
	${xslt_location})

set_property(TARGET bgp_create PROPERTY CXX_STANDARD 14)
set_property(TARGET bgp_create PROPERTY CXX_STANDARD_REQUIRED ON)

set_property(TARGET bgp_read PROPERTY CXX_STANDARD 14)
set_property(TARGET bgp_read PROPERTY CXX_STANDARD_REQUIRED ON)

set_property(TARGET bgp_update PROPERTY CXX_STANDARD 14)
set_property(TARGET bgp_update PROPERTY CXX_STANDARD_REQUIRED ON)

set_property(TARGET bgp_delete PROPERTY CXX_STANDARD 14)
set_property(TARGET bgp_delete PROPERTY CXX_STANDARD_REQUIRED ON)
