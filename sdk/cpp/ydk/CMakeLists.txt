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

# Set LibYDK variables
set(libydk_src
    src/codec_provider.cpp
    src/codec_service.cpp
    src/crud_service.cpp
    src/entity_data_node_walker.cpp
    src/entity_lookup.cpp
    src/errors.cpp
    src/netconf_client.cpp
    src/netconf_model_provider.cpp
    src/netconf_provider.cpp
    src/validation_service.cpp
    src/value.cpp
    src/value_list.cpp
    src/ydk_yang.cpp
    src/core/annotation.cpp
    src/core/capability.cpp
    src/core/core.cpp
    src/core/data_node.cpp
    src/core/repository.cpp
    src/core/root_data_node.cpp
    src/core/root_schema_node.cpp
    src/core/rpc.cpp
    src/core/schema_node.cpp
    src/core/schema_value_type.cpp
    src/core/statement.cpp)

set(libydk_headers 
    src/netconf_provider.hpp
    src/netconf_client.hpp
    src/codec_service.hpp
    src/codec_provider.hpp
    src/entity_lookup.hpp
    src/core.hpp
    src/entity_data_node_walker.hpp
    src/types.hpp
    src/errors.hpp
    src/validation_service.hpp
    src/service.hpp)

set(libydk_install_headers 
    src/netconf_provider.hpp
    src/codec_service.hpp
    src/codec_provider.hpp
    src/entity_lookup.hpp
    src/core.hpp
    src/crud_service.hpp
    src/errors.hpp
    src/types.hpp
    src/service.hpp
    src/validation_service.hpp)

set(Boost_USE_STATIC_LIBS        OFF)
set(Boost_USE_MULTITHREADED      ON)
set(Boost_USE_STATIC_RUNTIME     OFF)
set(BOOST_ALL_DYN_LINK           ON)   # force dynamic linking for all libraries

# Find dependent libraries
#SET(Boost_USE_STATIC_LIBS ON)
find_library(xml2_location xml2)
find_library(curl_location curl)
find_library(ssh_threads_location ssh_threads)
find_library(xslt_location xslt)
find_library(pcre_location pcre)
find_package(BOOST REQUIRED)
find_package(Boost COMPONENTS log_setup log thread date_time system filesystem REQUIRED)
find_package(LibSSH REQUIRED)
find_package(LibYANG REQUIRED)
find_package(LibNetconf REQUIRED)

#MESSAGE("netconf library ${LIBNETCONF_LIBRARY}")
#MESSAGE("yang library ${LIBYANG_LIBRARY}")

include_directories(
	${Boost_INCLUDE_DIRS}
        ${LIBYANG_INCLUDE_DIRS}
        ${LIBNETCONF_INCLUDE_DIRS}
        ${LIBSSH_INCLUDE_DIRS}
)

add_custom_command(OUTPUT libnetconf_objs
    COMMAND mkdir libnetconf_objs)

list(APPEND libnetconf_inc_objs "")
#MESSAGE("LIBNETCONF_OBJS ${LIBNETCONF_OBJS}")
foreach(netconf_obj IN LISTS LIBNETCONF_OBJS)
    add_custom_command(OUTPUT libnetconf_objs/${netconf_obj}
                        COMMAND ar -x ${LIBNETCONF_LIBRARY} ${netconf_obj}
                        MAIN_DEPENDENCY libnetconf_objs
                        WORKING_DIRECTORY libnetconf_objs)
    list(APPEND libnetconf_inc_objs libnetconf_objs/${netconf_obj})
    #MESSAGE("libnetconf_inc_objs ${libnetconf_inc_objs}")
endforeach(netconf_obj)

#MESSAGE("libnetconf_inc_objs ${libnetconf_inc_objs}")
#MESSAGE("libyang_inc_objs ${libyang_inc_objs}")

add_custom_command(OUTPUT libyang_objs
    COMMAND mkdir libyang_objs)

list(APPEND libyang_inc_objs "")
foreach(libyang_obj IN LISTS LIBYANG_OBJS)
    add_custom_command(OUTPUT libyang_objs/${libyang_obj}
                       COMMAND ar -x ${LIBYANG_LIBRARY} ${libyang_obj}
                       MAIN_DEPENDENCY libyang_objs
                       WORKING_DIRECTORY libyang_objs)
     list(APPEND libyang_inc_objs libyang_objs/${libyang_obj})
endforeach(libyang_obj)

add_library(ydk_core OBJECT ${libydk_src} ${libydk_headers})
set_property(TARGET ydk_core PROPERTY CXX_STANDARD 14)
set_property(TARGET ydk_core PROPERTY CXX_STANDARD_REQUIRED ON)
target_compile_definitions(ydk_core PRIVATE "BOOST_ALL_DYN_LINK=1")
target_compile_definitions(ydk_core PRIVATE "BOOST_LOG_DYN_LINK=1")



add_library(ydk STATIC $<TARGET_OBJECTS:ydk_core> ${libnetconf_inc_objs} ${libyang_inc_objs})
target_compile_definitions(ydk PRIVATE "BOOST_ALL_DYN_LINK=1")
target_compile_definitions(ydk PRIVATE "BOOST_LOG_DYN_LINK=1")

target_link_libraries(ydk
        ${Boost_LOG_SETUP_LIBRARY}
        ${Boost_LOG_LIBRARY}
        ${BOOST_THREAD_LIBRARY}
        ${BOOST_DATE_TIME_LIBRARY}
        ${Boost_FILESYSTEM_LIBRARY}
        ${Boost_SYSTEM_LIBRARY}
	${xml2_location}
	${curl_location}
	${ssh_threads_location}
        ${pcre_location}
	${xslt_location}
        ${LIBSSH_LIBRARIES}
)


set_property(TARGET ydk PROPERTY CXX_STANDARD 14)
set_property(TARGET ydk PROPERTY CXX_STANDARD_REQUIRED ON)


install(TARGETS ydk DESTINATION ${LIB_INSTALL_DIR})
install(FILES ${libydk_install_headers} DESTINATION ${INCLUDE_INSTALL_DIR})


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

