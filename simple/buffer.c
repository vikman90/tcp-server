/**
 * @file buffer.c
 * @brief This file contains functions for managing a buffer array.
 *
 * The buffer array is used to store and manipulate data associated with different sockets.
 * The functions provided in this file allow for creating, appending data to, and dumping the contents of the buffer.
 *
 * @author Vikman Fernandez-Castro
 * @date July 7, 2024
*/

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct buffer_t
{
    char *data;
    size_t size;
} buffer_t;

static buffer_t *buffer;
static size_t buffer_size;

/**
 * @brief Clears the buffer associated with the given socket.
 *
 * This function frees the memory allocated for the buffer data and resets the size to 0.
 * After calling this function, the buffer will be empty and ready for reuse.
 *
 * @param sock The socket associated with the buffer to be cleared.
 *
 * @return This function does not return a value.
 */
static void bufferClear(int sock)
{
    free(buffer[sock].data);
    buffer[sock].data = NULL;
    buffer[sock].size = 0;
}

// Creates a buffer array of the specified size.

void bufferCreate(size_t size)
{
    buffer = calloc(size, sizeof(buffer_t));
    buffer_size = size;
}

// Appends data to the buffer associated with the given socket.

void bufferAppend(int sock, const char *data, size_t size)
{
    if (sock >= buffer_size)
        return;

    buffer[sock].data = realloc(buffer[sock].data, buffer[sock].size + size);
    memcpy(buffer[sock].data + buffer[sock].size, data, size);
    buffer[sock].size += size;
}

// Prints the contents of the buffer associated with the given socket and clears the buffer.

void bufferDump(int sock)
{
    if (sock >= buffer_size)
        return;

    if (buffer[sock].data != NULL)
    {
        printf("[%d]: \"%.*s\"\n", sock, (int)buffer[sock].size, buffer[sock].data);
        bufferClear(sock);
    }
}
