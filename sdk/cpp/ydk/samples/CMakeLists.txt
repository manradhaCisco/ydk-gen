cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(ydk_core_samples CXX)

set(YDK_CORE_SAMPLES_DESCRIPTION "Samples")

set(samples bgp)

foreach(sample IN LISTS samples)
    add_executable(${sample} ${sample}.cpp)
    target_link_libraries(${sample} ydk)
    set_property(TARGET ${sample} PROPERTY CXX_STANDARD 11)
    set_property(TARGET ${sample} PROPERTY CXX_STANDARD_REQUIRED ON)
endforeach(sample)

