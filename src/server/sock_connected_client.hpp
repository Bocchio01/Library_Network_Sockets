#ifndef SOCK_CONNECTED_CLIENT_HPP
#define SOCK_CONNECTED_CLIENT_HPP

#include "../sock_includes_api.hpp"
#include <unordered_map>
#include <functional>
#include <vector>
#include <chrono>

class SockConnectedClient
{
public:
    SOCKET sock;
    SOCKADDR_IN address;
    std::chrono::time_point<std::chrono::system_clock> connection_time;

    SockConnectedClient(SOCKET sock, const SOCKADDR_IN &address, const std::chrono::time_point<std::chrono::system_clock> &connection_time = std::chrono::system_clock::now())
        : sock(sock),
          address(address),
          connection_time(connection_time) {}

    bool operator==(const SockConnectedClient &other) const;
};

#endif