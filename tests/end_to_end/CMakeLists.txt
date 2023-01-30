add_executable(test_generator ./src/generator.cpp)
target_include_directories(test_generator
                           PRIVATE ${INCLUDE_DIR})

add_executable(test_driver ./src/driver.cpp)
target_include_directories(test_driver
                           PRIVATE ${INCLUDE_DIR})

install(TARGETS test_generator test_driver
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
