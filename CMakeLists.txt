CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

PROJECT(msserialization)

FILE(GLOB_RECURSE MST_HEAD_FILES "include/*")

add_library(msserialization INTERFACE)

#target_sources(msserialization INTERFACE ${MST_HEAD_FILES})
target_include_directories(msserialization INTERFACE "include")

#add_subdirectory(test)