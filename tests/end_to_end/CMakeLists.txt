add_executable(test_generator ./src/generator.cpp)
target_include_directories(test_generator
                           PRIVATE ${INCLUDE_DIR})

add_executable(gauss ./src/driver.cpp)
target_include_directories(gauss
                           PRIVATE ${INCLUDE_DIR})

add_executable(bareiss ./src/driver.cpp)
target_include_directories(bareiss
                           PRIVATE ${INCLUDE_DIR})
target_compile_options(bareiss
                       PRIVATE -DINTEGER)

install(TARGETS test_generator gauss bareiss
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
