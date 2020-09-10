cmake_minimum_required(VERSION 2.8)


add_executable(rajson_demo rajson.cpp)

#include_directories("../../include/" ${RAPIDJSON_PATH}/include)
target_include_directories(rajson_demo PRIVATE "../../include/" ${RAPIDJSON_PATH}/include "../../submodule/rapidjson/include")


