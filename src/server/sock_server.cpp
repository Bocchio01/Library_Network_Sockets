#include <string>
#include <iostream>
#include <thread>
#include <atomic>

#include "libs/spdlog/include/spdlog/spdlog.h"
#include "libs/rapidjson/include/rapidjson/reader.h"
#include "libs/rapidjson/include/rapidjson/document.h"
#include "libs/rapidjson/include/rapidjson/filewritestream.h"
#include "libs/rapidjson/include/rapidjson/writer.h"
#include <cstdio>

#include "../sock_includes_api.hpp"
#include "sock_server.hpp"
#include <iostream>
#include <assert.h>
#include <mutex>

#include "actions/action_broadcast.hpp"

SockServer::SockServer() : should_run(true)
{
    spdlog::trace("Initializing SockServer class...");

    addAction("broadcast", actionBroadcast);
}

SockServer::~SockServer()
{
    spdlog::trace("Destroying SockServer class...");

    for (auto &client : this->connected_clients)
    {
        this->closeSocket(client.sock);
    }

    spdlog::trace("SockServer class destroyed.");

    return;
}

bool SockServer::bindTo(const std::string ip, const int port)
{
    spdlog::trace("Binding socket to {}:{}", ip.c_str(), port);

    this->ip = ip;
    this->port = port;
    SOCKADDR_IN address = this->createAddress(AF_INET, this->ip, this->port);

    int result = bind(this->sock, (struct sockaddr *)&address, sizeof(address));

    if (result == SOCKET_ERROR)
    {
        spdlog::critical("bind() failed: {}", SockCore::getLastError());
        throw TYPED_EXCEPTION("bind() failed", SockExceptionCode::BIND_FAILED);
        return false;
    }

    return true;
}

bool SockServer::startListening(int backlog)
{
    spdlog::trace("Launching listen()...");

    int result = listen(this->sock, backlog);

    if (result == SOCKET_ERROR)
    {
        spdlog::critical("listen() failed: {}", SockCore::getLastError());
        throw TYPED_EXCEPTION("listen() failed", SockExceptionCode::LISTEN_FAILED);
        return false;
    }
    else
    {
        spdlog::info("Listening at {}:{}", this->ip.c_str(), this->port);
    }

    return true;
}

SockConnectedClient SockServer::acceptNewClient()
{
    SockConnectedClient connected_client(INVALID_SOCKET, SOCKADDR_IN());
    int connectedClientAddressSize = sizeof(connected_client.address);

    connected_client.sock = accept(this->sock, (SOCKADDR *)&connected_client.address, &connectedClientAddressSize);

    if (connected_client.sock == INVALID_SOCKET)
    {
        spdlog::error("accept() failed: {}", SockCore::getLastError());
        return connected_client;
    }

    return connected_client;
}

void SockServer::handleNewClient()
{
    while (this->should_run)
    {
        SockConnectedClient connected_client = this->acceptNewClient();

        if (connected_client.sock != INVALID_SOCKET)
        {
            spdlog::info("New client connected");

            this->addConnectedClient(connected_client);

            this->connected_client_threads.emplace_back([this, connected_client]()
                                                        { this->handleConnectedClient(connected_client); });
        }
    }
}

void SockServer::handleConnectedClient(SockConnectedClient connected_client)
{
    spdlog::info("Handling connected client");

    rapidjson::Document json;
    char buffer[1024] = {0};

    while (this->should_run)
    {
        int nBytesReceived = this->receiveMessage(connected_client.sock, buffer, sizeof(buffer));

        if (nBytesReceived == 0)
        {
            this->removeConnectedClient(connected_client);
            spdlog::warn("Client disconnected.");
            return;
        }

        // buffer[nBytesReceived] = '\0';
        json.Parse(buffer);
        if (json.HasParseError())
        {
            spdlog::error("Error parsing JSON.");
            return;
        }

        assert(json.IsObject());
        assert(json.HasMember("action"));
        assert(json["action"].IsString());

        std::string action = json["action"].GetString();
        runAction(action.c_str(), json);
    }
}

void SockServer::addConnectedClient(SockConnectedClient connected_client)
{
    spdlog::info("Adding client with IP: {}", inet_ntoa(connected_client.address.sin_addr));

    std::lock_guard<std::mutex> lock(this->connected_clients_mutex);
    this->connected_clients.push_back(connected_client);

    spdlog::info("Client added.");
}

void SockServer::removeConnectedClient(SockConnectedClient connected_client)
{
    spdlog::info("Removing client with IP: {}", inet_ntoa(connected_client.address.sin_addr));

    std::lock_guard<std::mutex> lock(this->connected_clients_mutex);
    this->connected_clients.erase(std::remove(this->connected_clients.begin(), this->connected_clients.end(), connected_client), this->connected_clients.end());
    this->closeSocket(connected_client.sock);

    spdlog::info("Client removed.");
}

void SockServer::addAction(const std::string &name, std::function<void(SockServer *, const rapidjson::Document &)> func)
{
    this->actions[name] = func;
}

void SockServer::runAction(const std::string &action, const rapidjson::Document &json)
{
    spdlog::info("Executing action: {}", action);

    auto it = actions.find(action);
    if (it == actions.end())
    {
        spdlog::error("Action not found: {}", action);
        return;
    }

    it->second(this, json);
}

void SockServer::start()
{
    std::thread([this]()
                {
        while (this->should_run) {
            handleNewClient();
        } })
        .detach();
}

void SockServer::stop()
{
    spdlog::info("Stopping server...");

    should_run = false;

    shutdown_condition_variable.notify_all();

    closeSocket(sock);

    for (auto &thread : this->connected_client_threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }

    closeSocket(sock);

    spdlog::info("Server stopped.");
}