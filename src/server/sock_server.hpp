#ifndef SOCK_SERVER_HPP
#define SOCK_SERVER_HPP

#define MAX_CLIENTS 10

#include "../sock_includes_api.hpp"
#include "../core/sock_core.hpp"
#include "../core/sock_exceptions.hpp"

#include "libs/cJSON/cJSON.h"

#include <vector>

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

class SocketServer : public SockCore
{
private:
    std::vector<SOCKET_extended_t> clients;

public:
    // SocketServer();

    bool Bind(const char *ip, int port);
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