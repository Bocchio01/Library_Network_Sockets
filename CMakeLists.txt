cmake_minimum_required(VERSION 3.28)

project(Library_Network_Socket
    VERSION 0.1
    DESCRIPTION "A simple socket networking library for C"
    LANGUAGES CXX C
)

# Set the C/C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_C_STANDARD 99)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include_directories(
    ${CMAKE_SOURCE_DIR}
)

# Versioning control
# configure_file(Config.h.in src/Config.h)
add_subdirectory(libs)
add_subdirectory(src)
add_subdirectory(apps)
