if(BUILD_TESTS)
    enable_testing()
    
    # Подключаем Google Test как subproject
    add_subdirectory(third-party/gtest)
    
    # Включаем директории с заголовками gtest
    include_directories(
        ${CMAKE_CURRENT_SOURCE_DIR}../third-party/gtest/googletest/include
        ${CMAKE_CURRENT_SOURCE_DIR}../third-party/gtest/googlemock/include
    )
    
    # Формируем список тестовых файлов
    file(GLOB ${PROJECT_NAME}_TEST_SOURCES tests/*.cpp)
    
    # Создаем тестовый исполняемый файл
    add_executable(check ${${PROJECT_NAME}_TEST_SOURCES})
    
    # Линкуем с основным проектом и gtest
    target_link_libraries(check
        ${PROJECT_NAME}  # Ваша основная библиотека
        gtest_main       # Основная библиотека gtest
    )
    
    # Регистрируем тест
    add_test(NAME check COMMAND check)
endif()
