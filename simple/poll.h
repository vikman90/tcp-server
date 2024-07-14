#pragma once

typedef struct poll_t
{
    int fd;
    int size;
    void * events;
} poll_t;

poll_t * poll_init(int size);
void poll_destroy(poll_t * poll);
void poll_add(poll_t * poll, int fd);
int poll_wait(poll_t * poll, int timeout);
int poll_get(const poll_t * poll, int index);
