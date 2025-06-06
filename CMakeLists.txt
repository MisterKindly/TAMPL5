cmake_minimum_required(VERSION 3.10)
project(finalwork)

option(BUILD_TESTS "Build tests" ON)
option(ENABLE_COVERAGE "Enable coverage reporting" OFF)

if(ENABLE_COVERAGE)
    add_compile_options(--coverage -fprofile-arcs -ftest-coverage)
    add_link_options(--coverage)
endif()

add_subdirectory(banking)

if(BUILD_TESTS)
    enable_testing()
    
    add_subdirectory(third-party/gtest)
    
    file(GLOB TEST_SOURCES "test/*.cpp")
    
    add_executable(tests ${TEST_SOURCES})  # Исправлено имя цели
    
    target_link_libraries(tests  # Исправлено имя цели
        banking 
        gtest_main
    )
    
    add_test(NAME AllTests COMMAND tests)  # Исправлено имя цели
endif()
