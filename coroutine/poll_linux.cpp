/**
 * @file poll.cpp
 * @brief This file contains the implementation of the Poll class, which provides an interface for managing file descriptor events using the epoll mechanism.
 *
 * The Poll class is responsible for creating an epoll instance, adding file descriptors to the epoll, waiting for events, and retrieving the file descriptors that have triggered events.
 * It provides methods for adding file descriptors, waiting for events, and accessing the file descriptors that have triggered events.
 *
 * @author Vikman Fernandez-Castro
 * @date July 14, 2024
 */

#include "poll.hpp"
#include <unistd.h>
#include <stdexcept>
#include <sys/epoll.h>

using namespace std;

Poll::Poll(int size) : size(size)
{
    polld = epoll_create(size);

    if (polld == 0)
        throw runtime_error("Failed to create epoll instance");

    events = new epoll_event[size];
}

Poll::~Poll()
{
    close(polld);
    delete[] (epoll_event *)events;
}

void Poll::add(int fd)
{
    struct epoll_event request = {.events = EPOLLIN, .data = {.fd = fd}};

    if (epoll_ctl(polld, EPOLL_CTL_ADD, fd, &request) == -1)
        throw runtime_error("Failed to add file descriptor to epoll");
}

int Poll::wait(int timeout)
{
    return epoll_wait(polld, (epoll_event *)events, size, timeout);
}

int Poll::operator[](int i)
{
    return ((epoll_event *)events)[i].data.fd;
}
