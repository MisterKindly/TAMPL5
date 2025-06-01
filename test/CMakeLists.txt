include(FetchContent)
include(GoogleTestx)

FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
FetchContent_MakeAvailable(googletest)

if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    option(ENABLE_COVERAGE "Enable coverage reporting" ON)
    if(ENABLE_COVERAGE)
        target_compile_options(banking PRIVATE --coverage -fprofile-arcs -ftest-coverage)
        target_link_libraries(banking --coverage)
    endif()
endif()

add_executable(account_test AccountTest.cpp)
target_link_libraries(account_test
    PRIVATE
        banking
        gtest_main
        gmock
)

add_executable(transaction_test TransactionTest.cpp)
target_link_libraries(transaction_test
    PRIVATE
        banking
        gtest_main
        gmock
)

gtest_discover_tests(account_test)
gtest_discover_tests(transaction_test)
