#include "../sock_includes_api.hpp"
#include "sock_connected_client.hpp"

bool SockConnectedClient::operator==(const SockConnectedClient &other) const
{
    return sock == other.sock && inet_ntoa(address.sin_addr) == inet_ntoa(other.address.sin_addr);
}