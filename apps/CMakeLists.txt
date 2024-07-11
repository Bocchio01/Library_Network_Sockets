add_executable(Client_exe client.cpp)
target_link_libraries(Client_exe PRIVATE
    libs
    client
)

add_executable(Server_exe server.cpp)
target_link_directories(Server_exe PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
)
target_link_libraries(Server_exe PRIVATE
    libs
    server
)
