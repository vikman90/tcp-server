#pragma once

#include <map>

#include "poll.hpp"
#include "task.hpp"

#define TCP_BACKLOG 2048
#define BUFFER_LENGTH 4096
#define TIMEOUT_MILLIS -1

class Server
{
public:
    Server(unsigned int port) : port(port), serverSock(-1), poll(TCP_BACKLOG) {}
    ~Server();

    void run();

private:
    void openPort();
    void bindPort();
    Task acceptClients();
    Task handleClient(int sock);
    void loop();

    class SocketAwaitable
    {
    public:
        SocketAwaitable(Server &server, int sock) : server(server), sock(sock) {}
        bool await_ready() { return false; }
        void await_suspend(std::coroutine_handle<> h);
        void await_resume() {}

    private:
        Server &server;
        int sock;
    };

    unsigned int port;
    int serverSock;
    Poll poll;
    std::map<int, std::coroutine_handle<>> socketHandlers;
};
