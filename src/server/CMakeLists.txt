project(server
    VERSION 1.0
    DESCRIPTION "Server submodules"
    LANGUAGES CXX
)

add_library(${PROJECT_NAME}
    sock_server.cpp
    sock_connected_client.cpp
    actions/action_broadcast.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC core libs)