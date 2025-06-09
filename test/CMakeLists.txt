set(TEST_SOURCES
    AccountTest.cpp
    TransactionTest.cpp
    mock/MockAccount.cpp
)

add_executable(tests ${TEST_SOURCES})
target_link_libraries(tests banking gtest gmock  gtest_main)

set_target_properties(tests PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/test
)
