#include <iostream>
#include <thread>
#include <atomic>
#include <string>

#include "SocketCommon.hpp"
#include "SocketCore.hpp"
#include "server/SocketServer.hpp"

extern "C"
{
#include "libs/cLOG/cLOG.h"
}

int main(int argc, char *argv[])
{
    log_set_level(LOG_INFO);

    SocketServer server;

    server.Create();
    server.Bind("127.0.0.1", 5500);
    server.Listen();

    while (server.ShouldRun)
    {
        std::thread handleNewClient([&server]()
                                    { server.HandleNewClient(); });
        handleNewClient.join();
    }

    server.Disconnect();

    return EXIT_SUCCESS;
}
