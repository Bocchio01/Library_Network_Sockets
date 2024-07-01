project(server
    VERSION 1.0
    DESCRIPTION "Server submodules"

    # LANGUAGES CPP
)

add_library(${PROJECT_NAME}
    SocketServer.hpp
    SocketServer.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC libs)