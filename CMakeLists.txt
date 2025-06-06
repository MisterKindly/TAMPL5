cmake_minimum_required(VERSION 3.10)
project(finalwork)

option(BUILD_TESTS "Build tests" ON)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)


if(ENABLE_COVERAGE)
    add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
    add_link_options(--coverage)
endif()


include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/banking
    ${CMAKE_CURRENT_SOURCE_DIR}/test/mock
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/gtest/googlemock/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/gtest/googletest/include
)

add_subdirectory(banking)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(third-party/gtest)
    
    
    file(GLOB TEST_SOURCES "test/*.cpp")
    
    
    add_executable(tests ${TEST_SOURCES})
    
    # Связываем с библиотеками
    target_link_libraries(tests banking gtest_main)
    
    
    add_test(NAME AllTests COMMAND tests)
endif()
