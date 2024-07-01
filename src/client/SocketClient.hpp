#ifndef CSOCKETCLIENT_HPP
#define CSOCKETCLIENT_HPP

#include "../SocketCommon.hpp"

#include "../SocketCore.hpp"
#include "../SocketError.hpp"

#pragma comment(lib, "Ws2_32.lib")

class SocketClient : public SocketCore
{

public:
    SOCKADDR_IN server;

    SocketClient();

    bool Connect(const char *address, int port);
};

#endif