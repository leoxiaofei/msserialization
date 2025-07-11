CMAKE_MINIMUM_REQUIRED(VERSION 3.24)

PROJECT(msserialization)

if(${CMAKE_VERSION} VERSION_LESS "3.19.0") 
	add_library(${PROJECT_NAME} INTERFACE)
else()
	FILE(GLOB_RECURSE MST_HEAD_FILES "include/*")
	add_library(${PROJECT_NAME} INTERFACE ${MST_HEAD_FILES})
endif()

target_include_directories(${PROJECT_NAME} INTERFACE "include")

option(TEST_OPT "option for test" OFF)

if (TEST_OPT)
	include(CTest)
	add_subdirectory(unittest)
endif()