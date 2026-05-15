CMAKE_MINIMUM_REQUIRED(VERSION 3.14.0)

# Performance benchmark project
# Separate from unit tests for focused performance measurement

# Find required packages
find_package(RapidJSON CONFIG)
find_package(nlohmann_json CONFIG)
find_package(yaml-cpp CONFIG)
find_package(Qt5Core CONFIG)
find_package(GTest REQUIRED)

# Define benchmark source files based on available libraries
set(MSBENCH_SOURCE_FILES
    "stltypedef.hpp"
    "basetypedef.hpp"
)

if(RapidJSON_FOUND)
    list(APPEND MSBENCH_SOURCE_FILES "rajson_bench.cpp")
endif()

if(nlohmann_json_FOUND)
    list(APPEND MSBENCH_SOURCE_FILES "nljson_bench.cpp")
endif()

if(yaml-cpp_FOUND)
    list(APPEND MSBENCH_SOURCE_FILES "cppyaml_bench.cpp")
endif()

if(Qt5Core_FOUND)
    list(APPEND MSBENCH_SOURCE_FILES "qjson_bench.cpp")
endif()

# Cross-engine comparison benchmark (requires multiple libraries)
if(RapidJSON_FOUND AND nlohmann_json_FOUND AND yaml-cpp_FOUND)
    list(APPEND MSBENCH_SOURCE_FILES "convert_bench.cpp")
endif()

# Create benchmark executable
add_executable(msbench ${MSBENCH_SOURCE_FILES})

target_link_libraries(msbench PRIVATE msserialization ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

if(RapidJSON_FOUND)
    target_link_libraries(msbench PRIVATE rapidjson)
endif()

if(nlohmann_json_FOUND)
    target_link_libraries(msbench PRIVATE nlohmann_json::nlohmann_json)
endif()

if(yaml-cpp_FOUND)
    target_link_libraries(msbench PRIVATE yaml-cpp::yaml-cpp)
endif()

if(Qt5Core_FOUND)
    target_link_libraries(msbench PRIVATE Qt5::Core)
endif()

# Register tests
include(GoogleTest)
gtest_add_tests(TARGET msbench)