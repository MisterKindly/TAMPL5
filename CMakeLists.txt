cmake_minimum_required(VERSION 3.10)
project(finalwork)

option(BUILD_TESTS "Build tests" ON)

add_subdirectory(banking)

if(BUILD_TESTS)
	add_subdirectory(test)
endif()
