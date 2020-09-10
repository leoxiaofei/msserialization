cmake_minimum_required(VERSION 2.8)

PROJECT(MSSERIALIZE_TEST CXX)

set(TESTS
    qjson
    rajson)

#include_directories("../include/" ${RAPIDJSON_PATH}/include)


foreach (example ${TESTS})
    add_subdirectory(${example})
endforeach()


