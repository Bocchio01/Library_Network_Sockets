#include <iostream>
#include <thread>
#include <atomic>
#include <string>

#include "src/sock_includes_api.hpp"
#include "src/core/sock_core.hpp"
#include "src/server/sock_server.hpp"

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

    try
    {

        SocketServer server;

        server.Create();
        server.Bind("127.0.0.1", 5500);
        server.Listen();

        while (server.ShouldRun)
        {
            server.HandleNewClient();
        }

        server.Disconnect(server.sock);
    }
    catch (const TypedException<SockExceptionCode> &e)
    {
        std::cerr << e;
    }

    return EXIT_SUCCESS;
}
