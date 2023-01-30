add_executable(test_generator ./src/generator.cpp)
target_include_directories(test_generator
                           PRIVATE ${INCLUDE_DIR})

add_executable(gauss ./src/gauss.cpp)
target_include_directories(gauss
                           PRIVATE ${INCLUDE_DIR})

add_executable(bareiss ./src/bareiss.cpp)
target_include_directories(bareiss
                           PRIVATE ${INCLUDE_DIR})

install(TARGETS test_generator gauss bareiss
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})
