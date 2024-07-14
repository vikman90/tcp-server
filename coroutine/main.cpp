#include <iostream>
#include <cstdlib>
#include "server.hpp"

using namespace std;

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

int main(int argc, char **argv)
{
    unsigned port = getPort(argc, argv);
    Server server(port);
    server.run();
}
