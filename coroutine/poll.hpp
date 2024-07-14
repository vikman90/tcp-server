#pragma once

class Poll
{
public:
    Poll(int size);
    ~Poll();
    void add(int fd);
    int wait(int timeout);
    int operator[](int i);

private:
    int size;
    int polld;
    void * events;
};
