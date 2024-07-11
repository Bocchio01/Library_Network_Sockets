project(client
    VERSION 1.0
    DESCRIPTION "Client submodules"
    LANGUAGES CXX
)

add_library(${PROJECT_NAME}
    SocketClient.hpp
    SocketClient.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(${PROJECT_NAME} PUBLIC core libs)