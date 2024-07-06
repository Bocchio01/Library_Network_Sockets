project(server
    VERSION 1.0
    DESCRIPTION "Server submodules"
    LANGUAGES CXX
)

add_library(${PROJECT_NAME}
    SocketServer.hpp
    SocketServer.cpp
    actions/Broadcast.hpp
    actions/Broadcast.cpp
    actions/Custom.hpp
    actions/Custom.cpp
    actions/help.hpp
    actions/help.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC core libs)