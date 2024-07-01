project(client
    VERSION 1.0
    DESCRIPTION "Client submodules"

    # LANGUAGES CPP
)

add_library(${PROJECT_NAME}
    SocketClient.hpp
    SocketClient.cpp
)

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(${PROJECT_NAME} PUBLIC libs)