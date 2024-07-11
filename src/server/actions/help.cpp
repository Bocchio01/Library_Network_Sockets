#include "../sock_server.hpp"
#include "help.hpp"
#include "Broadcast.hpp"
// #include "server/actions/Custom.hpp"

void SocketServer::ActionHelp()
{
    cJSON *json = cJSON_CreateObject();
    // cJSON_AddStringToObject(json, "action", "help");
    // cJSON_AddStringToObject(json, "message", "Available actions: help, broadcast, sendto, restart, custom");
    cJSON_AddStringToObject(json, "help", "help - Show this message");

    cJSON *broadcast = ActionBroadcastHelp();
    cJSON_AddItemToObject(json, "broadcast", broadcast);

    // this->ActionSendTo(json);
    this->ActionBroadcast(json);
    cJSON_Delete(json);
}