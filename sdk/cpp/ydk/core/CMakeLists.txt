cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(ydk_core CXX)

set(YDKCORE_DESCRIPTION "YANG Development Kit Core Engine Library. The project provides the ydkcore library to be used in applications that use YDK API.")

# set version
set(YDK_CORE_MAJOR_VERSION 0)
set(YDK_CORE_MINOR_VERSION 5)
set(YDK_CORE_SERVICE_VERSION 0)
set(YDK_CORE_VERSION ${YDK_CORE_MAJOR_VERSION}.${YDK_CORE_MINOR_VERSION}.${YDK_CORE_SERVICE_VERSION})
set(YDK_CORE_SOVERSION ${YDK_CORE_MAJOR_VERSION}.${YDK_CORE_MINOR_VERSION})

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
#set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_CXX_FLAGS_DEBUG   "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE   "-g -O0")

if(CMAKE_BUILD_TYPE STREQUAL debug)
	option(ENABLE_BUILD_TESTS "Build tests" ON)
	option(ENABLE_VALGRIND_TESTS "Build tests with valgrind" ON)
else()
	option(ENABLE_BUILD_TESTS "Build tests" OFF)
	option(ENABLE_VALGRIND_TESTS "Build tests with valgrind" OFF)
endif()

set(lib_ydk_core_src
	src/ydk.cpp)


set(headers
        src/ydk.hpp)


add_library(ydk_core SHARED ${lib_ydk_core_src})
set_target_properties(ydk_core PROPERTIES VERSION ${YDK_CORE_VERSION} SOVERSION ${YDK_CORE_SOVERSION})

set_property(TARGET ydk_core PROPERTY CXX_STANDARD 11)
set_property(TARGET ydk_core PROPERTY CXX_STANDARD_REQUIRED ON)

# find YANG library
find_package(LibYANG REQUIRED)
include_directories(${LIBYANG_INCLUDE_DIRS})
target_link_libraries(ydk_core ${LIBYANG_LIBRARIES})

install(TARGETS ydk_core DESTINATION ${LIB_INSTALL_DIR})
install(FILES ${headers} DESTINATION ${INCLUDE_INSTALL_DIR})


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

