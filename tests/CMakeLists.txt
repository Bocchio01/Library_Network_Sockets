add_subdirectory(${CMAKE_SOURCE_DIR}/libs/Catch ${CMAKE_BINARY_DIR}/libs/Catch)

add_executable(tests first_test.cpp)
target_link_libraries(tests PRIVATE Catch2::Catch2WithMain dependencies)

include(Catch)
catch_discover_tests(tests)
