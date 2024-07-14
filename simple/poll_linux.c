/**
 * @file poll_linux.c
 * @brief This file contains the implementation of the poll_t data structure and related functions using the epoll system call.
 *
 * This implementation uses the epoll system call, which provides more efficient event notification compared to the poll() system call.
 *
 * @author Vikman Fernandez-Castro
 * @date July 7, 2024
 */

#include "poll.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/epoll.h>

#define die(msg)     \
    {                \
        perror(msg); \
        abort();     \
    }

poll_t * poll_init(int size)
{
    int poll_fd = epoll_create(size);

    if (poll_fd < 0)
        die("epoll_create");

    poll_t * poll = calloc(1, sizeof(poll_t));
    poll->fd = poll_fd;
    poll->size = size;
    poll->events = calloc(size, sizeof(struct epoll_event));

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
    struct epoll_event request = {.events = EPOLLIN, .data = {.fd = fd}};

    if (epoll_ctl(poll->fd, EPOLL_CTL_ADD, fd, &request) == -1)
        die("epoll_ctl: add");
}

int poll_wait(poll_t * poll, int timeout)
{
    return epoll_wait(poll->fd, poll->events, poll->size, timeout);
}

int poll_get(const poll_t * poll, int index)
{
    struct epoll_event * events = poll->events;
    return events[index].data.fd;
}
