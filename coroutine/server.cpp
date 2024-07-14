#include <stdexcept>
#include <string>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "server.hpp"
#include "poll.hpp"

using namespace std;

Server::~Server()
{
    if (serverSock != -1)
        close(serverSock);
}

void Server::run()
{
    openPort();
    acceptClients();
    loop();
}

void Server::openPort()
{
    serverSock = socket(AF_INET, SOCK_STREAM, 0);

    if (serverSock == -1)
        throw runtime_error("Error opening socket");

    bindPort();

    if (listen(serverSock, TCP_BACKLOG) == -1)
        throw runtime_error("Error listening on port");
}

void Server::bindPort()
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (::bind(serverSock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        throw runtime_error("Error binding socket");
}

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

void Server::SocketAwaitable::await_suspend(std::coroutine_handle<> h)
{
    server.socketHandlers[sock] = h;
}
