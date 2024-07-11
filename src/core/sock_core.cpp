#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>

extern "C"
{
#include "libs/log.c/src/log.h"
}

#include "../sock_includes_api.hpp"
#include "sock_core.hpp"
#include "sock_exceptions.hpp"

SockCore::SockCore()
{
    this->port = 0;
    this->ip = "";
    this->sock = INVALID_SOCKET;

#ifdef _WIN32
    WORD wVersionRequested = MAKEWORD(2, 2);
    LPWSADATA lpWSAData = new WSADATA();

    int result = WSAStartup(wVersionRequested, lpWSAData);

    if (result != 0)
    {
        throw TYPED_EXCEPTION("WSA Startup failed", SockExceptionCode::LIBRARY_NOT_FOUND);
    }
#endif

    return;
}

bool SockCore::Create(int address_family, int type, int protocol)
{
    this->sock = socket(address_family, type, protocol);

    if (this->sock == INVALID_SOCKET)
    {
        throw TYPED_EXCEPTION("Socket creation failed", SockExceptionCode::CREATION_FAILED);
    }

    return true;
}

bool SockCore::Send(SOCKET target_sock, const char *message)
{
    int result = send(target_sock, message, strlen(message), 0);

    return result != SOCKET_ERROR;
}

bool SockCore::SendTo(SOCKET target_sock, const char *message, const char *ip, int port)
{
    SOCKADDR_IN target = SockCore::createAddress(AF_INET, ip, port);

    int result = sendto(target_sock, message, strlen(message), 0, (struct sockaddr *)&target, sizeof(target));

    return result != SOCKET_ERROR;
}

int SockCore::Receive(SOCKET target_sock, char *message, int length)
{
    int result = recv(target_sock, message, length, 0);

    return result != SOCKET_ERROR ? result : 0;
}

int SockCore::ReceiveFrom(SOCKET target_sock, char *message, int length, char *ip, int *port)
{
    SOCKADDR_IN target;
    int target_length = sizeof(target);

    int result = recvfrom(target_sock, message, length, 0, (struct sockaddr *)&target, &target_length);

    return result != SOCKET_ERROR ? result : 0;
}

void SockCore::Disconnect(SOCKET target_sock)
{
#ifdef _WIN32
    closesocket(target_sock);
    WSACleanup();
#else
    close(target_sock);
#endif

    // shutdown(target_sock, SD_SEND);
}

SOCKADDR_IN SockCore::createAddress(int address_family, std::string ip, int port)
{
    SOCKADDR_IN address;

    address.sin_family = address_family;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = strlen(ip.c_str()) > 0 ? inet_addr(ip.c_str()) : INADDR_ANY;

    return address;
}

char *SockCore::getLastError() noexcept
{

#if defined(_WIN32)

    wchar_t *error_message = NULL;

    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&error_message, 0, NULL);

    return (char *)error_message;

#else

    return strerror(errno);

#endif
}