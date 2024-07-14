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

static void bindPort(int sock, unsigned port)
{
    struct sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(sock, (struct sockaddr *)&addr, sizeof(addr)) < 0)
        die("bind");
}

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

void serve(unsigned port)
{
    serverSock = openPort(port);
    poll = poll_init(TCP_BACKLOG);
    bufferCreate(TCP_BACKLOG);

    poll_add(poll, serverSock);

    while (1)
        loop();
}
