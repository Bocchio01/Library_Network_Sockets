#include "../SocketServer.hpp"
#include "Custom.hpp"

#include "libs/cJSON/cJSON.h"

void SocketServer::ActionCustom(cJSON *json)
{
    log_error("Custom action not implemented");
    this->ActionBroadcast(json);
}