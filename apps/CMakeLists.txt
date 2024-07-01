add_executable(Client client.cpp)
target_link_libraries(Client PUBLIC
    libs
    src
)

add_executable(Server server.cpp)
target_link_libraries(Server PUBLIC
    libs
    src
)