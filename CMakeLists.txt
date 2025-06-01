cmake_minimum_required(VERSION 3.10)
project(finalwork)

option(BUILD_TESTS "Build tests" ON)

add_subdirectory(banking)

if(BUILD_TESTS)
    enable_testing()
    
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/third-party/gtest)
    
    
        ${CMAKE_CURRENT_SOURCE_DIR}/third-party/gtest/googletest/include
        ${CMAKE_CURRENT_SOURCE_DIR}/third-party/gtest/googlemock/include
    )
    
    file(GLOB ${PROJECT_NAME}_TEST_SOURCES test/ *.cpp)
    
    add_executable(check \${\${PROJECT_NAME}_TEST_SOURCES})

    target_link_libraries(check
        banking 
        gtest_main
    )
    
    add_test(NAME AllTests COMMAND check)
endif()
