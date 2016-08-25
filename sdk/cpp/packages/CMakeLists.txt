# To Run:
# cd <build_dir>
# cmake ..
# make install or cmake --build .

# Testing:
# cmake .. -DBUILD_TESTS=ON
# make install
# make test
# Individual Testing:
# ctest -R <test_name> -VV

# Use this for building samples of the bundles 
# libydk will have its own top level cmake file under ydk
cmake_minimum_required(VERSION 3.6)
cmake_policy(SET CMP0048 NEW)
project(ydk VERSION 0.4.2)
message ("Generating project ${PROJECT_NAME} ${PROJECT_VERSION}")

option(BUILD_TESTS "Build test programs" OFF)

#add_subdirectory(ydk)

if(BUILD_TESTS)
	enable_testing()
	add_subdirectory(tests)
endif(BUILD_TESTS)
