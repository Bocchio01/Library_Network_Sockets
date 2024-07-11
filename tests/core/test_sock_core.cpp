#include <thread>
#include <catch2/catch_test_macros.hpp>
#include "core/sock_core.hpp"
#include "core/sock_exceptions.hpp"

TEST_CASE("SockCore Creation Test", "[SockCore]")
{
    SockCore core;

    REQUIRE_NOTHROW(core.Create(AF_INET, SOCK_STREAM, IPPROTO_TCP));
}

TEST_CASE("SockCore Send/Receive Test", "[SockCore]")
{
    SockCore core;

    REQUIRE_NOTHROW(core.Create(AF_INET, SOCK_STREAM, IPPROTO_TCP));

    SOCKET server_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    SOCKADDR_IN server_addr = SockCore::createAddress(AF_INET, "127.0.0.1", 8080);

    REQUIRE(bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) != SOCKET_ERROR);
    REQUIRE(listen(server_sock, 1) != SOCKET_ERROR);

    std::thread server_thread([&]()
                              {
        SOCKET client_sock = accept(server_sock, NULL, NULL);
        REQUIRE(client_sock != INVALID_SOCKET);

        char message[1024];
        int bytes_received = core.Receive(client_sock, message, sizeof(message));
        REQUIRE(bytes_received > 0);

        REQUIRE(core.Send(client_sock, "Hello, Client!") != SOCKET_ERROR);

        core.Disconnect(client_sock); });

    std::this_thread::sleep_for(std::chrono::milliseconds(100));

    SOCKADDR_IN client_addr = SockCore::createAddress(AF_INET, "127.0.0.1", 8080);
    REQUIRE(connect(core.sock, (struct sockaddr *)&client_addr, sizeof(client_addr)) != SOCKET_ERROR);

    REQUIRE(core.Send(core.sock, "Hello, Server!") != SOCKET_ERROR);

    char response[1024];
    int bytes_received = core.Receive(core.sock, response, sizeof(response));
    REQUIRE(bytes_received > 0);
    REQUIRE(std::string(response, bytes_received) == "Hello, Client!");

    server_thread.join();
    core.Disconnect(core.sock);
    core.Disconnect(server_sock);
}

TEST_CASE("SockCore Exception Test", "[SockCore]")
{
    SockCore core;

    REQUIRE_THROWS_AS(core.Create(AF_INET, SOCK_RAW, IPPROTO_TCP), TypedException<SockExceptionCode>);
}
