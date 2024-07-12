#ifndef SOCK_CLIENT_HPP
#define SOCK_CLIENT_HPP

#include "../core/sock_core.hpp"

class SocketClient : public SockCore
{

public:
    SOCKADDR_IN server;

    // SocketClient();

    bool connectTo(const char *address, int port);
    void startCommunication();
};

#endif