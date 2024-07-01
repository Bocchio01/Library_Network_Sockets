#include <iostream>
#include <thread>
#include <atomic>
#include <string>

#include "SocketCommon.hpp"

extern "C"
{
#include "libs/cLOG/cLOG.h"
}

#include "SocketCommon.hpp"
#include "SocketCore.hpp"
#include "client/SocketClient.hpp"

int main(int argc, char *argv[])
{
    log_set_level(LOG_INFO);

    char buffer[1024];
    SocketClient client;

    client.Create();
    client.Connect("127.0.0.1", 5500);

    client.Send(client.sock, "Sono il client!");

    while (true)
    {
        client.Receive(client.sock, buffer, sizeof(buffer));
        printf("Server says: %s\n", buffer);
    }

    client.Disconnect();

    return EXIT_SUCCESS;
}
