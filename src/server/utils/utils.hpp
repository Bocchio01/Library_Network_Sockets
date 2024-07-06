#include "server/SocketServer.hpp"

static Actions_t actions[] = {
    {"broadcast", &SocketServer::ActionBroadcast},
    {"customs", &SocketServer::ActionCustoms},
    {"disconnect", &SocketServer::ActionDisconnect},
    {"echo", &SocketServer::ActionEcho},
    {"help", &SocketServer::ActionHelp},
    {"list", &SocketServer::ActionList},
    {"quit", &SocketServer::ActionQuit},
    {"shutdown", &SocketServer::ActionShutdown},
    {"unknown", &SocketServer::ActionUnknown},
    {NULL, NULL},
};
