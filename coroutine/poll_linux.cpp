
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
