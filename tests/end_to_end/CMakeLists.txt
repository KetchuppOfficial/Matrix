add_executable(test_generator ./src/generator.cpp)
target_include_directories(test_generator
                           PRIVATE ${INCLUDE_DIR})

add_executable(test_driver ./src/driver.cpp)
target_include_directories(test_driver
                           PRIVATE ${INCLUDE_DIR})
