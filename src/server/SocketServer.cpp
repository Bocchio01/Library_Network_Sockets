#include "../core/SocketCommon.hpp"

#include <string>
#include <iostream>
#include <thread>
#include <atomic>

#include "SocketServer.hpp"

#include "actions/Broadcast.hpp"
#include "actions/Custom.hpp"
#include "actions/help.hpp"

#include "libs/cJSON/cJSON.h"

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
        this->Disconnect(this->sock);
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
        this->Disconnect(this->sock);
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

SOCKET_extended_t SocketServer::Accept()
{
    SOCKADDR_IN clientAddress;
    int clientAddressSize = sizeof(clientAddress);

    SOCKET clientSocket = accept(this->sock, (SOCKADDR *)&clientAddress, &clientAddressSize);

    if (clientSocket == INVALID_SOCKET)
    {
        // Exception
        log_fatal("accept() failed with error: %s", get_error_text());
        return {INVALID_SOCKET, clientAddress};
    }

    SOCKET_extended_t clientExtended;
    clientExtended.sock = clientSocket;
    clientExtended.address = clientAddress;

    return clientExtended;
}

void SocketServer::HandleNewClient()
{
    char buffer[4 * 1024];
    SOCKET_extended_t clientExtended = this->Accept();

    if (clientExtended.sock != INVALID_SOCKET)
    {
        log_info("New client connected");

        this->clients.push_back(clientExtended);

        std::thread handleConnectedClient([this, clientExtended]()
                                          { this->HandleConnectedClient(clientExtended); });
        handleConnectedClient.join();
    }
}

void SocketServer::HandleConnectedClient(SOCKET_extended_t clientExtended)
{
    log_info("Handling connected client");

    char buffer[MAX_LENGTH_MESSAGE] = {0};

    while (true)
    {
        int nBytesReceived = this->Receive(clientExtended.sock, buffer, sizeof(buffer));

        if (nBytesReceived == 0)
        {
            log_info("Client disconnected");
            this->Disconnect(clientExtended.sock);
            // this->clients.erase(
            //     std::remove(this->clients.begin(), this->clients.end(), clientExtended),
            //     this->clients.end());
            return;
        }

        buffer[nBytesReceived] = '\0';
        log_info("Received: %s", buffer);
        this->Send(clientExtended.sock, "Hello from server");

        cJSON *json = cJSON_Parse(buffer);
        if (json == NULL)
        {
            const char *error_ptr = cJSON_GetErrorPtr();
            if (error_ptr != NULL)
            {
                log_error("Error before: %s\n", error_ptr);
                cJSON_Delete(json);
                return;
            }
        }

        log_debug("Parsed: %s", cJSON_Print(json));
        const cJSON *action = cJSON_GetObjectItemCaseSensitive(json, "action");
        if (cJSON_IsString(action) && (action->valuestring != NULL))
            this->ActionHandler(action->valuestring, json);
        else
        {
            this->Send(clientExtended.sock, "Invalid JSON");
            log_error("Error parsing JSON");
        }
    }
}

void SocketServer::ActionHandler(char *action_string, cJSON *json)
{
    if (strcmp(action_string, "broadcast") == 0)
        this->ActionBroadcast(json);

    else if (strcmp(action_string, "help") == 0)
        this->ActionHelp();

    else if (strcmp(action_string, "custom") == 0)
        this->ActionCustom(json);
}