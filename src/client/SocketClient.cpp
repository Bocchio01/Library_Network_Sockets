#include "
#include "core/SocketCore.hpp"
#include "core/SocketError.hpp"

#include "SocketClient.hpp"

// SocketClient::SocketClient() {}

bool SocketClient::Connect(const char *address, int port)
{
    this->port = port;

    memset(&this->server, 0, sizeof(this->server));
    this->server.sin_family = AF_INET;
    this->server.sin_addr.s_addr = inet_addr(address);
    this->server.sin_port = htons(port);

    int result = connect(this->sock, (SOCKADDR *)(&this->server), sizeof(this->server));

    if (result == SOCKET_ERROR)
    {
        log_fatal("connect() failed with error: %s", get_error_text());
        return false;
    }
    else
    {
        log_info("Connected to %s:%d", address, port);
    }

    return true;
}