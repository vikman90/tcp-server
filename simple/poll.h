/**
 * @file poll.h
 * @brief This file contains the declaration and documentation for the poll_t data structure and related functions.
 *
 * The poll_t data structure is used to manage a set of file descriptors for polling I/O events.
 * The provided functions allow for initializing, destroying, adding file descriptors to the poll set,
 * waiting for events, and retrieving the file descriptors that have triggered events.
 *
 * @author Vikman Fernandez-Castro
 * @date July 7, 2024
 */

#pragma once

typedef struct poll_t
{
    int fd;
    int size;
    void * events;
} poll_t;

/**
 * @brief Initializes a poll set with the specified size.
 *
 * This function allocates memory for the poll set and initializes its internal data structures.
 *
 * @param size The maximum number of file descriptors that can be added to the poll set.
 *
 * @return The function returns a pointer to the initialized poll set.
 */
poll_t * poll_init(int size);

/**
 * @brief Destroys the poll set and frees the allocated memory.
 *
 * This function frees the memory allocated for the poll set and its internal data structures.
 *
 * @param poll The poll set to be destroyed.
 *
 * @return This function does not return a value.
 */
void poll_destroy(poll_t * poll);

/**
 * @brief Adds the specified file descriptor to the poll set.
 *
 * This function adds the specified file descriptor to the poll set for monitoring I/O events.
 *
 * @param poll The poll set to which the file descriptor should be added.
 * @param fd The file descriptor to be added to the poll set.
 *
 * @return This function does not return a value.
 */
void poll_add(poll_t * poll, int fd);

/**
 * @brief Waits for events on the poll set with the specified timeout.
 *
 * This function waits for events on the poll set for the specified timeout period.
 * If an event occurs on any of the file descriptors in the poll set, the function returns the number of events.
 *
 * @param poll The poll set to wait for events.
 * @param timeout The maximum time to wait for events, in milliseconds. If timeout is negative, the function will block indefinitely.
 *
 * @return The function returns the number of events that occurred on the poll set.
 */
int poll_wait(poll_t * poll, int timeout);

/**
 * @brief Retrieves the file descriptor associated with the specified event index.
 *
 * This function retrieves the file descriptor associated with the specified event index from the poll set.
 *
 * @param poll The poll set from which the file descriptor should be retrieved.
 * @param index The index of the event for which the file descriptor should be retrieved.
 *
 * @return The function returns the file descriptor associated with the specified event index.
 */
int poll_get(const poll_t * poll, int index);
