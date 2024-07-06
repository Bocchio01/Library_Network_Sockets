# cJSON
add_definitions(-DENABLE_CUSTOM_COMPILER_FLAGS=Off)
add_definitions(-DENABLE_CJSON_TEST=Off)
add_subdirectory(cJSON)

# getopt-for-windows
add_library(getopt STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/getopt-for-windows/getopt.c
)
target_include_directories(getopt PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/getopt-for-windows
)

# log.c
add_definitions(-DLOG_USE_COLOR)
add_library(logc STATIC
    ${CMAKE_CURRENT_SOURCE_DIR}/log.c/src/log.c
)
target_include_directories(logc PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/log.c/src
)

# Interface library
add_library(libs INTERFACE)
target_link_libraries(libs INTERFACE
    cjson
    getopt
    logc
    ws2_32
)