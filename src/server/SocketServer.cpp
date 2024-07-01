#include "../SocketCommon.hpp"

#include <string>

#include "SocketServer.hpp"

// SocketServer::SocketServer() {}

bool SocketServer::Bind(const char *address, int port)
{
    ADDRINFOA hints;
    PADDRINFOA hostInfo;

    this->port = port;
    this->sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    ZeroMemory(&hints, sizeof(ADDRINFOA));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, std::to_string(this->port).c_str(), &hints, &hostInfo);
    int result = bind(this->sock, hostInfo->ai_addr, (int)hostInfo->ai_addrlen);

    if (result == SOCKET_ERROR)
    {
        this->Disconnect();
        // Exception
        log_fatal("bind() failed with error: %s", get_error_text());
        return false;
    }

    return true;
}

bool SocketServer::Listen(int backlog)
{
    int result = listen(this->sock, backlog);

    if (result == SOCKET_ERROR)
    {
        this->Disconnect();
        // Exception
        log_fatal("listen() failed with error: %s", get_error_text());
        return false;
    }
    else
    {
        log_info("Listening on port %d", this->port);
    }

    return true;
}

SOCKET SocketServer::Accept()
{
    SOCKADDR_IN client;
    int clientSize = sizeof(client);

    SOCKET clientSocket = accept(this->sock, (SOCKADDR *)&client, &clientSize);

    if (clientSocket == INVALID_SOCKET)
    {
        // Exception
        log_fatal("accept() failed with error: %s", get_error_text());
        return INVALID_SOCKET;
    }

    return clientSocket;
}

void SocketServer::HandleNewClient()
{
    char buffer[4 * 1024];
    SOCKET clientSocket = this->Accept();

    if (clientSocket != INVALID_SOCKET)
    {
        log_info("New client connected");

        this->clients.push_back(clientSocket);

        this->Receive(clientSocket, buffer, sizeof(buffer));

        if (strstr(buffer, "exit") != NULL)
        {
            this->ShouldRun = false;
            return;
        }

        log_info("Received: %s", buffer);

        this->Send(clientSocket, "Hello from server");
        for (int i = 0; i < this->clients.size(); i++)
        {
            this->Send(this->clients[i], "New client connected");
        }
    }
}