#ifndef ACTION_BROADCAST_HPP
#define ACTION_BROADCAST_HPP

#include "sock_server.hpp"
#include "libs/rapidjson/include/rapidjson/document.h"

void actionBroadcast(SockServer *server, const rapidjson::Document &json);

// cJSON *ActionBroadcastHelp();

#endif