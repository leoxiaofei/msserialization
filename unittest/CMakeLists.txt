CMAKE_MINIMUM_REQUIRED(VERSION 3.14.0)

enable_testing()

find_package(GTest REQUIRED)

find_package(RapidJSON CONFIG)

find_package(nlohmann_json CONFIG)

find_package(yaml-cpp CONFIG)

find_package(Qt5Core CONFIG)

if (RapidJSON_FOUND)
list(PREPEND  MSTEST_SOURCE_FILES "rajson_test.cpp")
endif()

if (nlohmann_json_FOUND)
list(PREPEND  MSTEST_SOURCE_FILES "nljson_test.cpp")
endif()

if(yaml-cpp_FOUND)
list(PREPEND  MSTEST_SOURCE_FILES "cppyaml_test.cpp")
endif(yaml-cpp_FOUND)

if(Qt5Core_FOUND)
list(PREPEND  MSTEST_SOURCE_FILES "qjson_test.cpp")
endif(Qt5Core_FOUND)

# 互转测试：需要至少两个库同时存在
if(RapidJSON_FOUND AND nlohmann_json_FOUND AND yaml-cpp_FOUND)
list(PREPEND  MSTEST_SOURCE_FILES "convert_test.cpp")
endif()


add_executable(msttest ${MSTEST_INCLUDE_FILES} ${MSTEST_SOURCE_FILES})

target_link_libraries(msttest PRIVATE msserialization ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

if (RapidJSON_FOUND)
target_link_libraries(msttest PRIVATE rapidjson)
endif()

if (nlohmann_json_FOUND)
target_link_libraries(msttest PRIVATE nlohmann_json::nlohmann_json)
endif()

if(yaml-cpp_FOUND)
target_link_libraries(msttest PRIVATE yaml-cpp::yaml-cpp)
endif(yaml-cpp_FOUND)

if(Qt5Core_FOUND)
target_link_libraries(msttest PRIVATE Qt5::Core)
endif(Qt5Core_FOUND)

include(GoogleTest)
gtest_add_tests(TARGET msttest)
