cmake_minimum_required(VERSION 3.6)
cmake_policy(SET CMP0048 NEW)
project(ydk_oc VERSION 0.5.0)



set(YDK_OC_DESCRIPTION "openconfig ydk bundle")

# set version
set(YDK_OC_MAJOR_VERSION 0)
set(YDK_OC_MINOR_VERSION 5)
set(YDK_OC_SERVICE_VERSION 0)
set(YDK_OC_VERSION ${YDK_OC_MAJOR_VERSION}.${YDK_OC_MINOR_VERSION}.${YDK_OC_SERVICE_VERSION})
set(YDK_OC_CORE_SOVERSION ${YDK_OC_MAJOR_VERSION}.${YDK_OC_MINOR_VERSION})

# include custom Modules
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMakeModules/")

if(NOT LIB_INSTALL_DIR)
	set(LIB_INSTALL_DIR lib)
endif()

if(NOT INCLUDE_INSTALL_DIR)
	set(INCLUDE_INSTALL_DIR include/ydk_oc)
endif()

# set default build type if not specified by user
if(NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE debug)
endif()

set(CMAKE_CXX_FLAGS         "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0")


set(libydk_oc_src src/openconfig_bgp.cpp src/top_entity_lookup_gen.cpp)
set(libydk_oc_headers src/openconfig_bgp.h)

find_package(LibYDK REQUIRED)

include_directories(
	${Boost_INCLUDE_DIRS}
        ${LIBYDK_INCLUDE_DIRS}
)

add_library(ydk_oc STATIC ${libydk_oc_src})

target_link_libraries(ydk_oc 
    ${LIBYDK_LIBRARIES}
)

set_property(TARGET ydk_oc PROPERTY CXX_STANDARD 11)
set_property(TARGET ydk_oc PROPERTY CXX_STANDARD_REQUIRED ON)

# find YDK library

install(TARGETS ydk_oc DESTINATION ${LIB_INSTALL_DIR})
install(FILES ${libydk_oc__headers} DESTINATION ${INCLUDE_INSTALL_DIR})


# generate doxygen documentation for ydk_core API
find_package(Doxygen)
if(DOXYGEN_FOUND)
	set(DOXYGEN_SKIP_DOT TRUE)
	add_custom_target(doc
			COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
			WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})
	configure_file(Doxyfile.in Doxyfile)
endif()


# clean cmake cache
add_custom_target(cclean
		COMMAND make clean
		COMMAND find . -iname '*cmake*' -not -name CMakeLists.txt -not -path './CMakeModules*' -exec rm -rf {} +
		COMMAND rm -rf Makefile Doxyfile
		WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR})


