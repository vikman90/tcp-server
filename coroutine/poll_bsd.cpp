/**
 * @file poll.cpp
 * @brief This file contains the implementation of the Poll class, which provides an interface for managing file descriptor events using the kqueue mechanism.
 *
 * The Poll class is responsible for creating a kqueue instance, adding file descriptors to the kqueue, waiting for events, and retrieving the file descriptors that have triggered events.
 * It provides methods for adding file descriptors, waiting for events, and accessing the file descriptors that have triggered events.
 *
 * @author Vikman Fernandez-Castro
 * @date July 14, 2024
 */

#include "poll.hpp"
#include <unistd.h>
#include <stdexcept>
#include <sys/types.h>
#include <sys/event.h>

using namespace std;

Poll::Poll(int size) : size(size)
{
    polld = kqueue();

    if (polld == 0)
        throw runtime_error("Failed to create epoll instance");

    events = new struct kevent[size];
}

Poll::~Poll()
{
    close(polld);
    delete[] (struct kevent *)events;
}

void Poll::add(int fd)
{
    struct kevent request;
    EV_SET(&request, fd, EVFILT_READ, EV_ADD, 0, 0, 0);

    if (kevent(polld, &request, 1, NULL, 0, NULL) < 0)
        throw runtime_error("Failed to add file descriptor to epoll");
}

int Poll::wait(int timeout)
{
    struct timespec ts = { timeout / 1000, (timeout % 1000) * 1000000 };
    return kevent(polld, NULL, 0, (struct kevent *)events, size, timeout >= 0 ? &ts : NULL);
}

int Poll::operator[](int i)
{
    return ((struct kevent *)events)[i].ident;
}
