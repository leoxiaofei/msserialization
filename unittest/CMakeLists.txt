CMAKE_MINIMUM_REQUIRED(VERSION 3.14.0)

enable_testing()

find_package(GTest REQUIRED)

#FILE(GLOB MSTEST_INCLUDE_FILES "*.h")
#FILE(GLOB MSTEST_SOURCE_FILES "*.cpp")

find_package(RapidJSON CONFIG)

if (RapidJSON_FOUND)

list(APPEND MSTEST_SOURCE_FILES "rajson_test.cpp")

endif()

add_executable(msttest ${MSTEST_INCLUDE_FILES} ${MSTEST_SOURCE_FILES})

target_link_libraries(msttest PRIVATE msserialization ${GTEST_BOTH_LIBRARIES} ${CMAKE_THREAD_LIBS_INIT})

if (RapidJSON_FOUND)
target_link_libraries(msttest PRIVATE rapidjson)
endif()

include(GoogleTest)
gtest_add_tests(TARGET msttest)
