/**
 * @file server.hpp
 * @brief This file contains the declaration of the Server class.
 *
 * The Server class is responsible for managing the TCP server and handling client connections.
 * It provides methods for opening, binding, and running the server, as well as handling client connections asynchronously using coroutines.
 *
 * @author Vikman Fernandez-Castro
 * @date July 7, 2024
 */

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
    /**
     * @brief Constructs a Server object with the specified port number.
     *
     * This constructor initializes the server with the specified port number.
     *
     * @param port The port number on which the server will listen for incoming connections.
     */
    Server(unsigned int port) : port(port), serverSock(-1), poll(TCP_BACKLOG) {}

    /**
     * @brief Destroys the Server object and frees the allocated memory.
     */
    ~Server();

    /**
     * @brief Runs the server, opening the port, binding it, and accepting client connections.
     *
     * This function runs the server by opening the port, binding it, and accepting client connections asynchronously using coroutines.
     */
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
