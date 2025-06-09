cmake_minimum_required(VERSION 3.10)
project(finalwork)

option(BUILD_TESTS "Build tests" ON)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

if(ENABLE_COVERAGE)
    add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
    add_link_options(--coverage)
endif()

add_subdirectory(banking)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}/banking
    ${CMAKE_CURRENT_SOURCE_DIR}/test/mock
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/gtest/googlemock/include
    ${CMAKE_CURRENT_SOURCE_DIR}/third-party/gtest/googletest/include
)

if(BUILD_TESTS)
    enable_testing()

    add_subdirectory(third-party/gtest EXCLUDE_FROM_ALL)
    
    
    file(GLOB TEST_SOURCES "test/*.cpp" "test/mock/*.cpp")
    
    
    add_executable(tests ${TEST_SOURCES})
    
    target_include_directories(tests PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/banking
        ${CMAKE_CURRENT_SOURCE_DIR}/test/mock
        ${CMAKE_CURRENT_SOURCE_DIR}/third-party/gtest/googletest/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third-party/gtest/googlemock/include
    )

    target_link_libraries(tests banking gtest gmock gtest_main)
    
    
    add_test(NAME AllTests COMMAND tests)
endif()
