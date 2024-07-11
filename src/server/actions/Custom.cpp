#include "../sock_server.hpp"

#include "Custom.hpp"

extern "C"
{
#include "log.h"
}
#include "libs/cJSON/cJSON.h"

void SocketServer::ActionCustom(cJSON *json)
{
    log_error("Custom action not implemented");
    this->ActionBroadcast(json);
}

cJSON *ActionCustomHelp()
{
    cJSON *json = cJSON_CreateObject();
    cJSON_AddStringToObject(json, "action", "custom");
    cJSON_AddStringToObject(json, "help", "Custom action");
    cJSON_AddStringToObject(json, "message", "This action is not implemented");
    cJSON_AddStringToObject(json, "example", "{\"action\":\"custom\"}");

    return json;
}