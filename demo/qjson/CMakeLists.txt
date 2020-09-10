cmake_minimum_required(VERSION 2.8)

set(CMAKE_AUTOMOC ON)

find_package(Qt5Core)
find_package(Qt5Widgets)

add_executable(qjson_demo qjson.cpp)

#include_directories("../../include/" ${RAPIDJSON_PATH}/include)
target_include_directories(qjson_demo PRIVATE "../../include/")

target_link_libraries(qjson_demo Qt5::Core Qt5::Widgets)

