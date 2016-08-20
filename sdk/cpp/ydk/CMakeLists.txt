cmake_minimum_required(VERSION 3.6)
cmake_policy(SET CMP0048 NEW)
project(ydk VERSION 0.5.0)


set(YDK_DESCRIPTION "YANG Development Kit Library. The library for  YDK API.")

# set version
set(YDK_MAJOR_VERSION 0)
set(YDK_MINOR_VERSION 5)
set(YDK_SERVICE_VERSION 0)
set(YDK_VERSION ${YDK_MAJOR_VERSION}.${YDK_MINOR_VERSION}.${YDK_SERVICE_VERSION})
set(YDK_CORE_SOVERSION ${YDK_MAJOR_VERSION}.${YDK_MINOR_VERSION})

# include custom Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

if(NOT LIB_INSTALL_DIR)
	set(LIB_INSTALL_DIR lib)
endif()

if(NOT INCLUDE_INSTALL_DIR)
	set(INCLUDE_INSTALL_DIR include/ydk)
endif()

# set default build type if not specified by user
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE debug)
endif()

set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0")

if(CMAKE_BUILD_TYPE STREQUAL debug)
	option(ENABLE_BUILD_TESTS "Build tests" ON)
	option(ENABLE_VALGRIND_TESTS "Build tests with valgrind" ON)
else()
	option(ENABLE_BUILD_TESTS "Build tests" OFF)
	option(ENABLE_VALGRIND_TESTS "Build tests with valgrind" OFF)
endif()

set(libydk_src src/netconf_provider.cpp src/netconf_client.cpp src/value.cpp src/crud_service.cpp src/core.cpp models/openconfig_bgp.cpp src/entity_walker.cpp )
set(libydk_headers src/netconf_provider.hpp src/netconf_client.hpp src/core.hpp src/value.hpp src/entity.hpp src/crud_service.hpp models/openconfig_bgp.h src/entity.hpp )

# Find dependent libraries
find_library(netconf_location netconf)
find_library(xml2_location xml2)
find_library(curl_location curl)
find_library(ssh_location ssh)
find_library(ssh_threads_location ssh_threads)
find_library(xslt_location xslt)
find_package(LibYANG REQUIRED)
find_package(BOOST REQUIRED)

include_directories(
	${Boost_INCLUDE_DIRS}
        ${LIBYANG_INCLUDE_DIRS}
)

add_library(ydk STATIC ${libydk_src})

target_link_libraries(ydk 
	${Boost_LIBRARIES}
	${netconf_location}
	${xml2_location}
	${curl_location}
	${ssh_location}
	${ssh_threads_location}
	${xslt_location}
        ${LIBYANG_LIBRARIES}
)



#set_target_properties(ydk PROPERTIES VERSION ${YDK_VERSION} SOVERSION ${YDK_SOVERSION})

set_property(TARGET ydk PROPERTY CXX_STANDARD 11)
set_property(TARGET ydk PROPERTY CXX_STANDARD_REQUIRED ON)

# find YANG library

install(TARGETS ydk DESTINATION ${LIB_INSTALL_DIR})
install(FILES ${libydk_headers} DESTINATION ${INCLUDE_INSTALL_DIR})


# generate doxygen documentation for ydk_core API
find_package(Doxygen)
if(DOXYGEN_FOUND)
	set(DOXYGEN_SKIP_DOT TRUE)
	add_custom_target(doc
			COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
	configure_file(Doxyfile.in Doxyfile)
endif()

add_subdirectory(samples)

# clean cmake cache
add_custom_target(cclean
		COMMAND make clean
		COMMAND find . -iname '*cmake*' -not -name CMakeLists.txt -not -path './CMakeModules*' -exec rm -rf {} +
		COMMAND rm -rf Makefile Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})

if(ENABLE_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif(ENABLE_BUILD_TESTS)

