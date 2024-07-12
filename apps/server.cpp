#include <iostream>
#include <thread>
#include <atomic>
#include <string>

#include "src/sock_includes_api.hpp"
#include "src/sock_defines.hpp"
#include "src/sock_exceptions.hpp"
#include "src/core/sock_core.hpp"
#include "src/server/sock_server.hpp"

#include "libs/spdlog/include/spdlog/spdlog.h"

int main(int argc, char *argv[])
{
    FILE *fid = fopen("log.txt", "w");
    spdlog::set_level(spdlog::level::info);
    // spdlog::add_fp(fid, spdlog::TRACE);

    try
    {
        SockServer server;

        server.createSocket();
        server.bindTo("127.0.0.1", 5500);
        server.startListening();

        std::thread server_thread([&server]()
                                  { server.start(); });

        std::string input;
        while (true)
        {
            std::cout << "Type 'exit' to stop the server: ";
            std::cin >> input;
            if (input == "exit")
            {
                server.stop();
                break;
            }
        }

        if (server_thread.joinable())
        {
            server_thread.join();
        }

        server.closeSocket(server.sock);
    }
    catch (const TypedException<SockExceptionCode> &e)
    {
        // fprintf(fid, "%s", cout << e);
        std::cerr << e;
        return EXIT_FAILURE;
    }

    fclose(fid);

    return EXIT_SUCCESS;
}
