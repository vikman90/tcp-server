/**
 * @file main.cpp
 * @brief This file contains the entry point of the TCP server application.
 *
 * The main function parses the command-line arguments, validates the port number,
 * creates an instance of the Server class, and runs the server.
 *
 * @author Vikman Fernandez-Castro
 * @date July 13, 2024
 */

#include <iostream>
#include <cstdlib>
#include "server.hpp"

using namespace std;

/**
 * @brief Retrieves the port number from the command-line arguments.
 *
 * This function checks if the correct number of arguments is provided,
 * converts the port number string to an unsigned integer, and validates the port number.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 *
 * @return The function returns the validated port number.
 */
static unsigned getPort(int argc, char *argv[])
{
    if (argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <port>\n";
        exit(1);
    }

    unsigned port = strtoul(argv[1], NULL, 10);

    if (port < 1 || port > 65535)
    {
        cerr << "Invalid port number. Please enter a value between 1 and 65535.\n";
        exit(1);
    }

    return port;
}

/**
 * @brief The entry point of the TCP server application.
 *
 * The main function parses the command-line arguments, retrieves the port number,
 * creates an instance of the Server class, and runs the server.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 *
 * @return The function returns 0 if the server runs successfully, or 1 if an error occurs.
 */
int main(int argc, char **argv)
{
    unsigned port = getPort(argc, argv);
    Server server(port);
    server.run();
}
