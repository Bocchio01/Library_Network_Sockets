#ifndef SOCK_SERVER_HPP
#define SOCK_SERVER_HPP

#include "../sock_includes_api.hpp"
#include "../sock_defines.hpp"
#include "../sock_exceptions.hpp"
#include "../core/sock_core.hpp"
#include "sock_connected_client.hpp"

#include "libs/rapidjson/include/rapidjson/document.h"
#include "libs/spdlog/include/spdlog/spdlog.h"

#include <unordered_map>
#include <functional>
#include <vector>
#include <mutex>
#include <thread>
#include <condition_variable>

class SockServer : public SockCore
{
private:
    std::unordered_map<std::string, std::function<void(SockServer *, const rapidjson::Document &)>> actions;
    std::condition_variable shutdown_condition_variable;

public:
    bool should_run = true;
    std::mutex connected_clients_mutex;
    std::vector<std::thread> connected_client_threads;
    std::vector<SockConnectedClient> connected_clients;

    SockServer();
    ~SockServer();

    bool bindTo(const std::string ip, const int port);
    bool startListening(int backlog = MAX_CLIENTS);
    SockConnectedClient acceptNewClient();

    void handleNewClient();
    void handleConnectedClient(SockConnectedClient connected_client);
    void addConnectedClient(SockConnectedClient connected_client);
    void removeConnectedClient(SockConnectedClient connected_client);

    void addAction(const std::string &name, std::function<void(SockServer *, const rapidjson::Document &)> func);
    void runAction(const std::string &action, const rapidjson::Document &json);

    void start();
    void stop();
};

#endif