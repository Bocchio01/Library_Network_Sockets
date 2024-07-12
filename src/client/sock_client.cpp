#include "../sock_includes_api.hpp"
#include "../core/sock_core.hpp"
#include "sock_client.hpp"

#include "libs/spdlog/include/spdlog/spdlog.h"

// SocketClient::SocketClient() {}

bool SocketClient::connectTo(const char *address, int port)
{
    this->port = port;

    memset(&this->server, 0, sizeof(this->server));
    this->server.sin_family = AF_INET;
    this->server.sin_addr.s_addr = inet_addr(address);
    this->server.sin_port = htons(port);

    int result = connect(this->sock, (SOCKADDR *)(&this->server), sizeof(this->server));

    if (result == SOCKET_ERROR)
    {
        spdlog::critical("connect() failed with error: {}", SockCore::getLastError());
        return false;
    }
    else
    {
        spdlog::info("Connected to {}:{}", address, port);
    }

    return true;
}