cmake_minimum_required(VERSION 3.28)

project(Socket_Networking
    VERSION 0.1
    DESCRIPTION "A simple socket networking library for C"

    # # LANGUAGES CPP
)

set(CMAKE_CPP_COMPILER g++)

# set(CMAKE_CPP_STANDARD 11)
# set(CMAKE_CPP_STANDARD_REQUIRED ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_definitions(-DLOG_USE_COLOR)

include_directories(
    ${CMAKE_SOURCE_DIR}
)

add_subdirectory(libs)
add_subdirectory(src)
add_subdirectory(apps)