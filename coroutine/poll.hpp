/**
 * @file poll.hpp
 * @brief This file contains the declaration of the Poll class.
 *
 * The Poll class is used to manage a set of file descriptors for polling I/O events.
 * The class provides methods for adding file descriptors to the poll set, waiting for events,
 * and retrieving the file descriptors that have triggered events.
 *
 * @author Vikman Fernandez-Castro
 * @date July 14, 2024
 */

#pragma once

class Poll
{
public:
    /**
     * @brief Constructs a Poll object with the specified size.
     *
     * This constructor initializes the poll set with the specified size.
     *
     * @param size The maximum number of file descriptors that can be added to the poll set.
     */
    Poll(int size);

    /**
     * @brief Destroys the Poll object and frees the allocated memory.
     */
    ~Poll();

    /**
     * @brief Adds the specified file descriptor to the poll set.
     *
     * This function adds the specified file descriptor to the poll set for monitoring I/O events.
     *
     * @param fd The file descriptor to be added to the poll set.
     */
    void add(int fd);

    /**
     * @brief Waits for events on the poll set with the specified timeout.
     *
     * This function waits for events on the poll set for the specified timeout period.
     * If an event occurs on any of the file descriptors in the poll set, the function returns the number of events.
     *
     * @param timeout The maximum time to wait for events, in milliseconds. If timeout is negative, the function will block indefinitely.
     *
     * @return The function returns the number of events that occurred on the poll set.
     */
    int wait(int timeout);

    /**
     * @brief Retrieves the file descriptor associated with the specified event index.
     *
     * This function retrieves the file descriptor associated with the specified event index from the poll set.
     *
     * @param i The index of the event for which the file descriptor should be retrieved.
     *
     * @return The function returns the file descriptor associated with the specified event index.
     */
    int operator[](int i);

private:
    int size;
    int polld;
    void * events;
};
