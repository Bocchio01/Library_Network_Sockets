#include <iostream>
#include <thread>
#include <atomic>
#include <string>

#include "src/sock_includes_api.hpp"
#include "src/sock_defines.hpp"
#include "src/sock_exceptions.hpp"
#include "src/core/sock_core.hpp"
#include "src/client/sock_client.hpp"

#include "libs/spdlog/include/spdlog/spdlog.h"
#include "libs/rapidjson/include/rapidjson/rapidjson.h"
#include "libs/rapidjson/include/rapidjson/document.h"
#include <windows.h>

int main(int argc, char *argv[])
{
    spdlog::set_level(spdlog::level::info);

    // const char *json = "{"action":"broadcast","message":"Hello from client 1"}";

    try
    {
        SocketClient client;

        client.createSocket();
        client.connectTo("127.0.0.1", 5500);

        std::thread sending_thread([&client]()
                                   {
            std::string inputbuffer = "";
            while (true)
            {
                printf("> ");
                std::getline(std::cin, inputbuffer);
                client.sendMessage(client.sock, inputbuffer.c_str());
            } });

        std::thread receiving_thread([&client]()
                                     {
            char buffer[1024];
            while (true)
            {
                client.receiveMessage(client.sock, buffer, sizeof(buffer));
                printf("Server > %s\n", buffer);
            } });

        sending_thread.join();
        receiving_thread.join();

        // client.start();

        client.closeSocket(client.sock);
    }
    catch (const TypedException<SockExceptionCode> &e)
    {
        // fprintf(fid, "%s", cout << e);
        std::cerr << e;
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
