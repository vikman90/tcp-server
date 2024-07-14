/**
 * @file server.cpp
 * @brief This file contains the implementation of the Server class.
 *
 * The Server class is responsible for managing the TCP server and handling client connections.
 * It provides methods for opening, binding, and running the server, as well as handling client connections asynchronously using coroutines.
 *
 * @author Vikman Fernandez-Castro
 * @date July 13, 2024
 */

#include <stdexcept>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "server.hpp"
#include "poll.hpp"

using namespace std;

// Destroys the Server object and frees the allocated memory.

Server::~Server()
{
    if (serverSock != -1)
        close(serverSock);
}

// Runs the server, opening the port, binding it, and accepting client connections.

/**
 * @brief Runs the server, opening the port, binding it, and accepting client connections.
 *
 * This function initializes the server by opening the specified port, binding it to the server socket,
 * and then entering a loop to accept client connections. Once a client connection is accepted,
 * the server will handle the client asynchronously using coroutines.
 *
 * @return void
 */
void Server::run()
{
    openPort();
    acceptClients();
    loop();
}

/**
 * @brief Opens the server port and initializes the server socket.
 *
 * This function creates a TCP socket, sets its address family to AF_INET,
 * and sets the socket type to SOCK_STREAM. It then attempts to open the socket,
 * binds it to the specified port, and listens for incoming connections.
 * If any of these operations fail, a runtime_error is thrown with an appropriate error message.
 *
 * @return void
 *
 * @throws runtime_error If an error occurs while opening, binding, or listening to the socket.
 */
void Server::openPort()
{
    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSock == -1)
        throw runtime_error("Error opening socket");

    bindPort();

    if (listen(serverSock, TCP_BACKLOG) == -1)
        throw runtime_error("Error listening on port");
}

/**
 * @brief Binds the server socket to the specified port.
 *
 * This function initializes a sockaddr_in structure with the server's address family (AF_INET),
 * port number (converted to network byte order using htons), and IP address (INADDR_ANY).
 * It then attempts to bind the server socket to the specified address and port.
 * If the binding operation fails, a runtime_error is thrown with an appropriate error message.
 *
 * @return void
 *
 * @throws runtime_error If an error occurs while binding the socket.
 */
void Server::bindPort()
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (::bind(serverSock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        throw runtime_error("Error binding socket");
}

/**
 * @brief Accepts incoming client connections and handles them asynchronously using coroutines.
 *
 * This function continuously listens for incoming client connections on the server socket.
 * When a client connection is accepted, the function creates a new socket for the client,
 * adds it to the poll for asynchronous I/O, and then calls the handleClient function to handle the client connection.
 *
 * @return A coroutine task that can be awaited.
 *
 * @throws runtime_error If an error occurs while accepting a client connection.
 */
Task Server::acceptClients()
{
    poll.add(serverSock);

    while (true)
    {
        co_await SocketAwaitable(*this, serverSock);

        int sock = accept(serverSock, NULL, NULL);

        if (sock == -1)
        {
            cerr << "Error accepting client" << endl;
            continue;
        }

        handleClient(sock);
    }
}

/**
 * @brief Handles an incoming client connection asynchronously using coroutines.
 *
 * This function accepts an incoming client connection on the specified socket,
 * adds the socket to the poll for asynchronous I/O, and then continuously receives data from the client.
 * When the client disconnects or an error occurs during data reception, the function closes the socket and stops handling the client.
 *
 * @param sock The socket descriptor for the client connection.
 *
 * @return A coroutine task that can be awaited.
 *
 * @throws runtime_error If an error occurs while accepting a client connection or receiving data.
 */
Task Server::handleClient(int sock)
{
    poll.add(sock);
    string buffer;

    for (auto active = true; active;)
    {
        co_await SocketAwaitable(*this, sock);

        char data[BUFFER_LENGTH];
        ssize_t bytesReceived = recv(sock, data, BUFFER_LENGTH - 1, 0);

        switch (bytesReceived)
        {
        case -1:
            cerr << "Error receiving data from client" << endl;
            break;

        case 0:
            close(sock);
            active = false;
            break;

        default:
            data[bytesReceived] = '\0';
            buffer.append(data);
        }
    }

    cout << "[" << sock << "]: " << buffer << endl;
}

/**
 * @brief Runs the server's main event loop, handling client connections asynchronously.
 *
 * This function continuously polls the server's poll object for active sockets.
 * When an active socket is detected, the function retrieves the corresponding coroutine handler,
 * removes the socket from the socketHandlers map, and resumes the coroutine to handle the client connection.
 *
 * @return void
 *
 * @note This function runs indefinitely until the server is stopped externally.
 */
void Server::loop()
{
    while (true)
    {
        for (auto i = 0; i < poll.wait(TIMEOUT_MILLIS); i++)
        {
            int j = poll[i];
            auto handler = socketHandlers[poll[i]];
            socketHandlers.erase(poll[i]);
            handler.resume();
        }
    }
}

/**
 * @brief Suspends the coroutine until the specified socket becomes ready for I/O.
 *
 * This function is a custom awaitable type used in conjunction with coroutines to handle asynchronous I/O operations.
 * When a coroutine awaits an instance of this class, it will be suspended until the specified socket becomes ready for I/O.
 * The server's poll object is used to monitor the socket for readiness, and when the socket is ready, the coroutine is resumed.
 *
 * @param h The coroutine handle representing the suspended coroutine.
 *
 * @return void
 *
 * @note This function is part of the Server class and is used internally to handle asynchronous I/O operations.
 */
void Server::SocketAwaitable::await_suspend(std::coroutine_handle<> h)
{
    server.socketHandlers[sock] = h;
}
