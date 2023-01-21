aux_source_directory(./src SRC_LIST)

add_executable(hwmx_tests ${SRC_LIST})

target_link_libraries(hwmx_tests
                      PRIVATE ${GTEST_LIBRARIES})

target_include_directories(hwmx_tests
                           PRIVATE ${INCLUDE_DIR})

gtest_discover_tests(hwmx_tests)
