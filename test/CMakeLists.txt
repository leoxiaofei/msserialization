cmake_minimum_required(VERSION 2.8)

PROJECT(MSSERIALIZE CXX)

set(TESTS
    qjson
    rajson)

include_directories("../include/" ${RAPIDJSON_PATH}/include)


foreach (example ${TESTS})
    add_executable(${example} ${example}/${example}.cpp)
endforeach()


