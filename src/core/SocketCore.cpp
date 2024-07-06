#include <assert.h>
#include <stdio.h>
#include <stdlib.h>

extern "C"
{
#include "libs/log.c/src/log.h"
}

#include "SocketCommon.hpp"
#include "SocketError.hpp"
#include "SocketCore.hpp"

SocketCore::SocketCore()
{
    this->port = 0;
    this->sock = INVALID_SOCKET;

#ifdef _WIN32
    int result = 0;
    WORD wVersionRequested = MAKEWORD(2, 2);
    LPWSADATA lpWSAData = new WSADATA();

    result = WSAStartup(wVersionRequested, lpWSAData);

    if (result != 0)
    {
        // Exception
        log_fatal("WSAStartup() failed with error: %d", result);
        exit(1);
    }
#endif
}

bool SocketCore::Create(int address_family, int type, int protocol)
{
    this->sock = socket(address_family, type, protocol);

    if (this->sock == INVALID_SOCKET)
    {
        // Exception
        log_fatal("socket() failed with error: %s", get_error_text());
        return false;
    }

    return true;
}

bool SocketCore::Send(SOCKET target_sock, const char *message)
{
    int result = send(target_sock, message, strlen(message), 0);

    if (result == SOCKET_ERROR)
    {
        // Exception
        log_fatal("send() failed with error: %s", get_error_text());
        return false;
    }

    return true;
}

bool SocketCore::SendTo(SOCKET target_sock, const char *message, const char *ip, int port)
{
    SOCKADDR_IN target;

    target.sin_family = AF_INET;
    target.sin_port = htons(port);
    target.sin_addr.s_addr = inet_addr(ip);

    int result = sendto(target_sock, message, strlen(message), 0, (struct sockaddr *)&target, sizeof(target));

    if (result == SOCKET_ERROR)
    {
        // Exception
        log_fatal("sendto() failed with error: %s", get_error_text());
        return false;
    }

    return true;
}

int SocketCore::Receive(SOCKET target_sock, char *message, int length)
{
    int result = recv(target_sock, message, length, 0);

    if (result == SOCKET_ERROR)
    {
        // Exception
        log_fatal("recv() failed with error: %s", get_error_text());
        return false;
    }

    return true;
}

int SocketCore::ReceiveFrom(SOCKET target_sock, char *message, int length, char *ip, int *port)
{
    SOCKADDR_IN target;
    int target_length = sizeof(target);

    int result = recvfrom(target_sock, message, length, 0, (struct sockaddr *)&target, &target_length);

    if (result == SOCKET_ERROR)
    {
        // Exception
        log_fatal("recvfrom() failed with error: %s", get_error_text());
        return false;
    }

    *port = ntohs(target.sin_port);
    strcpy(ip, inet_ntoa(target.sin_addr));

    return true;
}

void SocketCore::Disconnect(SOCKET target_sock)
{
#ifdef _WIN32
    closesocket(target_sock);
    WSACleanup();
#else
    close(target_sock);
#endif

    // shutdown(target_sock, SD_SEND);
}
