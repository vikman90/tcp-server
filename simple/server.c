/**
 * @file server.c
 * @brief This file contains the implementation of a simple TCP server using the poll() system call.
 *
 * The server listens for incoming connections on a specified port, accepts incoming connections, and handles client requests.
 * It uses a buffer array to store and manipulate data associated with different sockets.
 * The server uses the poll() system call to efficiently manage multiple connections and handle events.
 *
 * @author Vikman Fernandez-Castro
 * @date July 7, 2024
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include "poll.h"
#include "buffer.h"

#define TCP_BACKLOG 2048
#define BUFFER_LENGTH 4096
#define TIMEOUT_MILLIS -1
#define die(msg)     \
    {                \
        perror(msg); \
        abort();     \
    }

static int serverSock;
static poll_t *poll;
static char *data[TCP_BACKLOG];

/**
 * @brief Binds the specified socket to the given port.
 *
 * This function sets up the socket address structure and binds the socket to the specified port.
 *
 * @param sock The socket to be bound.
 * @param port The port number to bind the socket to.
 *
 * @return This function does not return a value.
 */
static void bindPort(int sock, unsigned port)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        die("bind");
}

/**
 * @brief Opens a listening socket on the specified port.
 *
 * This function creates a socket, binds it to the specified port, and sets it to listen for incoming connections.
 *
 * @param port The port number on which the socket should listen for incoming connections.
 *
 * @return The function returns the file descriptor of the listening socket.
 */
static int openPort(unsigned port)
{
    int sock = socket(AF_INET, SOCK_STREAM, 0);

    if (sock < 0)
        die("socket");

    bindPort(sock, port);

    if (listen(sock, TCP_BACKLOG) < 0)
        die("listen");

    return sock;
}
/**
 * @brief Prints the data associated with the given socket and frees the memory.
 *
 * This function prints the data associated with the given socket, frees the memory allocated for the data,
 * and sets the data pointer to NULL.
 *
 * @param sock The socket associated with the data to be printed and freed.
 *
 * @return This function does not return a value.
 */
static void printData(int sock)
{
    if (sock >= TCP_BACKLOG)
        return;

    if (data[sock] != NULL)
    {
        printf("[%d]: %s\n", sock, data[sock]);
        free(data[sock]);
        data[sock] = NULL;
    }
}

/**
 * @brief Handles incoming data on the specified socket.
 *
 * This function receives data from the specified socket, appends it to the buffer associated with the socket,
 * and prints the data if it is complete.
 *
 * @param sock The socket associated with the incoming data.
 *
 * @return This function does not return a value.
 */
static void handleConn(int sock)
{
    char data[BUFFER_LENGTH];
    ssize_t bytes_read = recv(sock, data, sizeof(data), 0);

    if (bytes_read > 0)
        bufferAppend(sock, data, bytes_read);
    else
    {
        bufferDump(sock);
        close(sock);
        return;
    }
}

/**
 * @brief Main loop for the server.
 *
 * This function continuously waits for events on the poll set, handles incoming connections and data,
 * and prints the data associated with each socket.
 *
 * @return This function does not return a value.
 */
static void loop()
{
    int nEvents = poll_wait(poll, TIMEOUT_MILLIS);

    for (int i = 0; i < nEvents; i++)
    {
        int sock = poll_get(poll, i);

        if (sock == serverSock)
        {
            int sock = accept(serverSock, NULL, NULL);
            poll_add(poll, sock);
        }
        else if (sock > 0)
            handleConn(sock);
    }
}

// Starts a server listening on the specified port.

void serve(unsigned port)
{
    serverSock = openPort(port);
    poll = poll_init(TCP_BACKLOG);
    bufferCreate(TCP_BACKLOG);

    poll_add(poll, serverSock);

    while (1)
        loop();
}
