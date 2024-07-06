#ifndef CSOCKETSERVER_HPP
#define CSOCKETSERVER_HPP

#define MAX_CLIENTS 10

#include "../core/SocketCommon.hpp"

#include "../core/SocketCore.hpp"
#include "../core/SocketError.hpp"

#include "actions/Broadcast.hpp"
#include "actions/Custom.hpp"

#include "libs/cJSON/cJSON.h"

#include <vector>

#pragma comment(lib, "Ws2_32.lib")

typedef struct
{
    SOCKET sock;
    SOCKADDR_IN address;
} SOCKET_extended_t;

enum Actions_t
{
    BROADCAST,
    SENDTO,
    RESTART,
    CUSTOM
};

class SocketServer : public SocketCore
{
private:
    std::vector<SOCKET_extended_t> clients;

public:
    // SocketServer();

    bool Bind(const char *address, int port);
    bool Listen(int backlog = MAX_CLIENTS);
    SOCKET_extended_t Accept();

    void HandleNewClient();
    void HandleConnectedClient(SOCKET_extended_t clientExtended);

    void ActionHandler(char *action_string, cJSON *json);
    void ActionHelp();
    void ActionBroadcast(cJSON *json);
    void ActionCustom(cJSON *json);
};

#endif