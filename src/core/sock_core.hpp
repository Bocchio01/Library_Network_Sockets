#ifndef SOCK_CORE_HPP
#define SOCK_CORE_HPP

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include "../sock_includes_api.hpp"

// typedef struct
// {
//     char raw[1024];
//     cJSON *json;
// } message_t;

class SockCore
{
protected:
    int port;
    std::string ip;

public:
    SOCKET sock;
    bool ShouldRun = true;

    /**
     * @brief Construct a new SockCore object
     *
     * If on Windows, initializes the WSA library.
     * If on Linux, does nothing.
     *
     * @throws SockExceptionLibraryNotFound If the WSA library could not be initialized (Windows only)
     */
    SockCore();

    /**
     * @brief Create a new socket
     *
     * @param address_family The address family of the socket
     * @param type The type of the socket
     * @param protocol The protocol of the socket
     *
     * @throws SockExceptionCreationFailed If the socket creation failed
     * @return true If the socket was created successfully
     */
    bool Create(int address_family = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);

    /**
     * @brief Bind the socket to a port
     *
     * @param port The port to bind to
     *
     * @throws SockExceptionBindFailed If the socket could not be bound to the port
     * @return true If the socket was bound successfully
     */
    bool Send(SOCKET target_sock, const char *message);

    /**
     * @brief Send a message to a target socket
     *
     * @param target_sock The target socket to send the message to
     * @param message The message to send
     * @param ip The IP address of the target socket
     * @param port The port of the target socket
     *
     * @throws SockExceptionSendFailed If the message could not be sent
     * @return true If the message was sent successfully
     */
    bool SendTo(SOCKET target_sock, const char *message, const char *ip, int port);

    /**
     * @brief Receive a message from a target socket
     *
     * @param target_sock The target socket to receive the message from
     * @param message The message to store the received message in
     * @param length The length of the message
     *
     * @throws SockExceptionReceiveFailed If the message could not be received
     * @return int The number of bytes received
     */
    int Receive(SOCKET target_sock, char *message, int length);

    /**
     * @brief Receive a message from a target socket
     *
     * @param target_sock The target socket to receive the message from
     * @param message The message to store the received message in
     * @param length The length of the message
     * @param ip The IP address of the target socket
     * @param port The port of the target socket
     *
     * @throws SockExceptionReceiveFailed If the message could not be received
     * @return int The number of bytes received
     */
    int ReceiveFrom(SOCKET target_sock, char *message, int length, char *ip, int *port);

    /**
     * @brief Close the socket
     */
    void Disconnect(SOCKET target_sock);

    static SOCKADDR_IN createAddress(int address_family = AF_INET, std::string ip = "", int port = 5500);

    /**
     * @brief Get the last error message
     *
     * @return char* The last error message
     */
    static char *getLastError() noexcept;
};

#endif