#include <stdio.h>
#include <stdlib.h>
#include <stdexcept>

#include "libs/spdlog/include/spdlog/spdlog.h"
#include "../sock_includes_api.hpp"
#include "../sock_exceptions.hpp"
#include "sock_core.hpp"

SockCore::SockCore()
{
    spdlog::trace("Initializing SockCore class...");

    this->port = 0;
    this->ip = "";
    this->sock = INVALID_SOCKET;

#ifdef _WIN32
    WORD wVersionRequested = MAKEWORD(2, 2);
    LPWSADATA lpWSAData = new WSADATA();

    int result = WSAStartup(wVersionRequested, lpWSAData);

    if (result != 0)
    {
        spdlog::critical("Initailizing SockCore class failed.");
        throw TYPED_EXCEPTION("WSA Startup failed", SockExceptionCode::LIBRARY_NOT_FOUND);
    }
#endif

    spdlog::trace("SockCore class initialized.");

    return;
}

SockCore::~SockCore()
{
    spdlog::trace("Destroying SockCore class...");

    if (this->sock != INVALID_SOCKET)
    {
        this->closeSocket(this->sock);
    }

    spdlog::trace("SockCore class destroyed.");

    return;
}

bool SockCore::createSocket(int address_family, int type, int protocol)
{
    spdlog::trace("Creating socket...");

    this->sock = socket(address_family, type, protocol);

    if (this->sock == INVALID_SOCKET)
    {
        spdlog::critical("Socket creation failed: {}", SockCore::getLastError());
        throw TYPED_EXCEPTION("Socket creation failed", SockExceptionCode::CREATION_FAILED);
    }

    spdlog::trace("Socket created.");

    return true;
}

bool SockCore::sendMessage(SOCKET target_sock, const char *message)
{
    spdlog::trace("Sending message...");

    int result = send(target_sock, message, strlen(message), 0);

    if (result == SOCKET_ERROR)
    {
        spdlog::error("send() failed: {}", SockCore::getLastError());
        return false;
    }

    spdlog::trace("Sent message: {}", message);

    return true;
}

bool SockCore::sendMessageTo(SOCKET target_sock, const char *message, const char *ip, int port)
{
    spdlog::trace("Sending message to {}:{}...", ip, port);

    SOCKADDR_IN target = SockCore::createAddress(AF_INET, ip, port);

    int result = sendto(target_sock, message, strlen(message), 0, (struct sockaddr *)&target, sizeof(target));

    if (result == SOCKET_ERROR)
    {
        spdlog::error("sendto() failed: {}", SockCore::getLastError());
        return false;
    }

    spdlog::trace("Sent message: {}", message);

    return true;
}

int SockCore::receiveMessage(SOCKET target_sock, char *message, int length)
{
    spdlog::trace("Receiving message...");

    int result = recv(target_sock, message, length, 0);

    if (result == SOCKET_ERROR)
    {
        spdlog::error("recv() failed: {}", SockCore::getLastError());
        return 0;
    }

    message[result] = '\0';
    spdlog::trace("Received message: {}", message);

    return result;
}

int SockCore::receiveMessageFrom(SOCKET target_sock, char *message, int length, char *ip, int *port)
{
    spdlog::trace("Receiving message from...");

    SOCKADDR_IN target;
    int target_length = sizeof(target);

    int result = recvfrom(target_sock, message, length, 0, (struct sockaddr *)&target, &target_length);

    if (result == SOCKET_ERROR)
    {
        spdlog::error("recvfrom() failed: {}", SockCore::getLastError());
        return 0;
    }

    *port = ntohs(target.sin_port);
    strcpy(ip, inet_ntoa(target.sin_addr));

    message[result] = '\0';
    spdlog::trace("Received message: {}", message);

    return result;
}

void SockCore::closeSocket(SOCKET target_sock)
{
    spdlog::trace("Disconnecting socket...");

#ifdef _WIN32
    closesocket(target_sock);
    WSACleanup();
#else
    close(target_sock);
#endif

    spdlog::trace("Socket disconnected.");
}

SOCKADDR_IN SockCore::createAddress(int address_family, std::string ip, int port)
{
    spdlog::trace("Creating socket address from ip and port...");

    SOCKADDR_IN address;

    address.sin_family = address_family;
    address.sin_port = htons(port);
    address.sin_addr.s_addr = strlen(ip.c_str()) > 0 ? inet_addr(ip.c_str()) : INADDR_ANY;

    spdlog::trace("Socket address created.");

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