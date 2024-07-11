#ifndef CSOCKETCLIENT_HPP
#define CSOCKETCLIENT_HPP

#include "../core/SocketCommon.hpp"

#include "../core/SocketCore.hpp"
#include "../core/SocketError.hpp"

#pragma comment(lib, "Ws2_32.lib")

class SocketClient : public SockCore
{

public:
    SOCKADDR_IN server;

    // SocketClient();

    bool Connect(const char *address, int port);
};

#endif