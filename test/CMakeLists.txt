cmake_minimum_required(VERSION 2.8)

PROJECT(MSSERIALIZE CXX)

set(TESTS
    qjson
    rajson)

include_directories("../include/")


foreach (example ${TESTS})
    add_executable(${example} ${example}/${example}.cpp)
endforeach()


add_custom_target(tests ALL DEPENDS ${TESTS})