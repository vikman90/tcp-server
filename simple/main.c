/**
 * @file main.c
 * @brief This file contains the main function for the server application.
 *
 * The main.c file includes the necessary headers and defines the main function, which parses command-line arguments,
 * validates the port number, and starts the server using the serve() function from the server.h file.
 *
 * @author Vikman Fernandez-Castro
 * @date July 7, 2024
 */

#include <stdio.h>
#include <stdlib.h>
#include "server.h"

static unsigned getPort(int argc, char *argv[])
{
    if (argc != 2)
    {
        fprintf(stderr, "Usage: %s <port>\n", argv[0]);
        exit(1);
    }

    unsigned port = strtoul(argv[1], NULL, 10);

    if (port < 1 || port > 65535)
    {
        fprintf(stderr, "Invalid port number. Please enter a value between 1 and 65535.\n");
        exit(1);
    }

    return port;
}

int main(int argc, char *argv[])
{
    unsigned port = getPort(argc, argv);
    serve(port);
    return 0;
}
