#ifndef SOCK_CORE_HPP
#define SOCK_CORE_HPP

#include <stdlib.h>
#include <iostream>
#include <stdio.h>
#include <string>

#include "../sock_includes_api.hpp"
#include "../sock_defines.hpp"

class SockCore
{
protected:
    int port;
    std::string ip;

public:
    SOCKET sock;

    SockCore();
    ~SockCore();

    bool createSocket(int address_family = AF_INET, int type = SOCK_STREAM, int protocol = IPPROTO_TCP);

    bool sendMessage(SOCKET target_sock, const char *message);

    bool sendMessageTo(SOCKET target_sock, const char *message, const char *ip, int port);

    int receiveMessage(SOCKET target_sock, char *message, int length);

    int receiveMessageFrom(SOCKET target_sock, char *message, int length, char *ip, int *port);

    void closeSocket(SOCKET target_sock);

    // static std::string getIpFromAddress(SOCKADDR_IN address);

    static SOCKADDR_IN createAddress(int address_family = AF_INET, std::string ip = "", int port = DEFAULT_PORT);

    static char *getLastError() noexcept;
};

#endif