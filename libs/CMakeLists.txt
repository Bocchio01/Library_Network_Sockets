# RapidJSON
add_subdirectory(rapidjson)

# getopt-for-windows
add_library(getopt STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/getopt-for-windows/getopt.c
)
target_include_directories(getopt PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/getopt-for-windows
)

# spdlog
add_subdirectory(spdlog)

# Interface library
add_library(libs INTERFACE)
target_link_libraries(libs INTERFACE
    RapidJSON
    getopt
    spdlog
    ws2_32
)