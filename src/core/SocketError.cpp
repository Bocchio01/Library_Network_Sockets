#include "SocketError.hpp"
#include "SocketCore.hpp"

extern "C"
{
#include "libs/log.c/src/log.h"
}

#include <string.h>
#include <winbase.h>

#include <stdlib.h>
#include <stdio.h>
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

char *get_error_text()
{

#if defined(_WIN32)

    static char message[256] = {0};
    FormatMessageW(
        FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
        0,
        WSAGetLastError(),
        0,
        reinterpret_cast<LPWSTR>(message),
        256,
        0);

    char *nl = strrchr(message, '\n');

    if (nl)
        *nl = 0;

    return message;

#else

    return strerror(errno);

#endif
}

void printLastError()
{
    wchar_t *error_message = NULL;

    FormatMessageW(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
            FORMAT_MESSAGE_FROM_SYSTEM |
            FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        WSAGetLastError(),
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPWSTR)&error_message, 0, NULL);

    fprintf(stderr, "%S\n", error_message);

    LocalFree(error_message);
}

bool SocketCore::checkResult(int expected, int actual, const char *message)
{
    if (expected != actual)
    {
        log_error("%s: %s\n", message, get_error_text());
        return false;
    }

    return true;
}