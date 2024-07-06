#include <iostream>
#include <thread>
#include <atomic>
#include <string>

#include "SocketCommon.hpp"
#include "SocketCore.hpp"
#include "SocketServer.hpp"

extern "C"
{
#include "libs/log.c/src/log.h"
}

// void SocketServer::ActionCustom(cJSON *json) override
// {
//     log_error("Custom action implemented overridden!");
//     this->ActionBroadcast(json);
// }

int main(int argc, char *argv[])
{
    log_set_level(LOG_INFO);

    SocketServer server;

    server.Create();
    server.Bind("127.0.0.1", 5500);
    server.Listen();

    while (server.ShouldRun)
    {
        server.HandleNewClient();
    }

    server.Disconnect(server.sock);

    return EXIT_SUCCESS;
}
