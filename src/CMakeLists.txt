project(src
    VERSION 1.0
    DESCRIPTION "Core for the Sockets library"

    # LANGUAGES CPP
)

add_subdirectory(client)
add_subdirectory(server)

add_library(${PROJECT_NAME}
    SocketCommon.hpp
    SocketCore.hpp
    SocketCore.cpp
    SocketError.hpp
    SocketError.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC
    libs
    client
    server
)