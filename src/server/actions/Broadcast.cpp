#include "../sock_server.hpp"

#include "Broadcast.hpp"

#include "libs/cJSON/cJSON.h"

void SocketServer::ActionBroadcast(cJSON *json)
{
    for (SOCKET_extended_t client : this->clients)
    {
        this->Send(client.sock, cJSON_Print(json));
    }
}

cJSON *ActionBroadcastHelp()
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "action", "broadcast");
    cJSON_AddStringToObject(json, "help", "Broadcasts a message to all connected clients");
    cJSON_AddStringToObject(json, "message", "The message to broadcast");
    cJSON_AddStringToObject(json, "example", "{\"action\":\"broadcast\",\"message\":\"Hello, world!\"}");

    return json;
}