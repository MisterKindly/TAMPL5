if(BUILD_TESTS)
    enable_testing()
    
    add_subdirectory(${CMAKE_CURRENT_SOURCE_DIR}/../third-party/gtest)
    
    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}/../third-party/gtest/googletest/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../third-party/gtest/googlemock/include
        ${CMAKE_CURRENT_SOURCE_DIR}/../banking 
    )
    
    file(GLOB TEST_SOURCES *.cpp)
    
    add_executable(check ${TEST_SOURCES})
    
    target_link_libraries(check
        banking 
        gtest_main
    )
    
    add_test(NAME AllTests COMMAND check)
endif()
