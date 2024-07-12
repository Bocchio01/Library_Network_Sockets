#include "action_broadcast.hpp"

#include "libs/rapidjson/include/rapidjson/document.h"

void actionBroadcast(SockServer *server, const rapidjson::Document &json)
{
    std::lock_guard<std::mutex> lock(server->connected_clients_mutex);
    for (SockConnectedClient client : server->connected_clients)
    {
        server->sendMessage(client.sock, json["message"].GetString());
    }
}