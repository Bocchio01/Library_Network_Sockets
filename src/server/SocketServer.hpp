#ifndef CSOCKETSERVER_HPP
#define CSOCKETSERVER_HPP

#define MAX_CLIENTS 10

#include "../SocketCommon.hpp"

#include "../SocketCore.hpp"
#include "../SocketError.hpp"

#include <vector>

#pragma comment(lib, "Ws2_32.lib")

class SocketServer : public SocketCore
{
private:
    std::vector<SOCKET> clients;

public:
    // SocketServer();

    bool Bind(const char *address, int port);
    bool Listen(int backlog = MAX_CLIENTS);
    SOCKET Accept();

    void HandleNewClient();
};

#endif