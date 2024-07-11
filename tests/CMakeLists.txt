add_subdirectory(${CMAKE_SOURCE_DIR}/libs/Catch ${CMAKE_BINARY_DIR}/libs/Catch)

set(WILDCARD_SOURCE *.cpp)
file(GLOB_RECURSE TEST_SOURCES ${WILDCARD_SOURCE})

add_executable(Test_exe ${TEST_SOURCES})
target_link_libraries(Test_exe PRIVATE Catch2::Catch2WithMain libs core)
target_include_directories(Test_exe PRIVATE ${CMAKE_SOURCE_DIR}/src ${CMAKE_SOURCE_DIR}/tests)

include(Catch)
catch_discover_tests(Test_exe)
