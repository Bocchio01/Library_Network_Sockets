#ifndef SOCKET_CORE_HPP
#define SOCKET_CORE_HPP

#include "SocketCommon.hpp"

#include <stdlib.h>

typedef struct
{
    char raw[1024];
    cJSON *json;
} message_t;

class SocketCore
{
protected:
    int port;

    bool checkResult(int expected, int actual, const char *message);

public:
    SOCKET sock;
    bool ShouldRun = true;

    /**
     * @brief Construct a new Socket Core object
     *
     * If on Windows, initializes the WSA library.
     */
    SocketCore();

    /**
     * @brief Create a new socket object
     *
     * @param address_family The address family of the socket
     * @param type The type of the socket
     * @param protocol The protocol of the socket
     *
     * @return true If the socket was created successfully
     * @return false If the socket was not created successfully
     */
    bool Create(int address_family = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);

    bool Send(SOCKET target_sock, const char *message);
    bool SendTo(const char *message, const char *ip, int port);
    bool Receive(SOCKET target_sock, char *message, int length);
    bool ReceiveFrom(char *message, int length, char *ip, int *port);
    void Disconnect();

    // iResult = shutdown(ClientSocket, SD_SEND);
};

#endif