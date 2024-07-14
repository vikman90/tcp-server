#include "poll.h"
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/event.h>

#define die(msg)     \
    {                \
        perror(msg); \
        abort();     \
    }

poll_t * poll_init(int size)
{
    int poll_fd = kqueue();

    if (poll_fd < 0)
        die("epoll_create");

    poll_t * poll = calloc(1, sizeof(poll_t));
    poll->fd = poll_fd;
    poll->size = size;
    poll->events = calloc(size, sizeof(struct kevent));

    return poll;
}

void poll_destroy(poll_t * poll)
{
    close(poll->fd);
    free(poll->events);
    free(poll);
}

void poll_add(poll_t * poll, int fd)
{
    struct kevent request;
    EV_SET(&request, fd, EVFILT_READ, EV_ADD, 0, 0, 0);

    if (kevent(poll->fd, &request, 1, NULL, 0, NULL) < 0)
        die("kevent: add");
}

int poll_wait(poll_t * poll, int timeout)
{
    struct timespec ts = { timeout / 1000, (timeout % 1000) * 1000000 };
    return kevent(poll->fd, NULL, 0, poll->events, poll->size, timeout >= 0 ? &ts : NULL);
}

int poll_get(const poll_t * poll, int index)
{
    struct kevent * events = poll->events;
    return events[index].ident;
}
