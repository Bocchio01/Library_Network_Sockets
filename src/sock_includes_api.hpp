#ifndef SOCK_INCLUDES_API_HPP
#define SOCK_INCLUDES_API_HPP

#ifdef _WIN32

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#ifndef _WINSOCK_DEPRECATED_NO_WARNINGS
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#endif

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>

// #include <iphlpapi.h>

#pragma comment(lib, "Ws2_32.lib")
// #pragma comment(lib, "Mswsock.lib")
// #pragma comment(lib, "AdvApi32.lib")

#else

#define INVALID_SOCKET -1

#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netdb.h>
#include <unistd.h>

#endif

#endif
