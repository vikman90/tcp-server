/**
 * @file buffer.h
 * @brief This file contains declarations for functions related to buffer management.
 *
 * The buffer.h file provides functions to create, append data to, and dump the contents of a buffer array.
 *
 * @author Vikman Fernandez-Castro
 * @date July 7, 2024
 */

#pragma once

#include <stddef.h>

/**
 * @brief Creates a buffer array of the specified size.
 *
 * This function initializes the buffer array with the given size. Each element in the array is a buffer_t structure,
 * which contains a pointer to the buffer data and its size. The memory for the buffer array is allocated using calloc,
 * ensuring that all elements are initialized to zero.
 *
 * @param size The size of the buffer array to be created. This value should be greater than zero.
 *
 * @return This function does not return a value. Upon successful creation of the buffer array, the global variables
 * 'buffer' and 'buffer_size' will be updated accordingly.
 */
void bufferCreate(size_t size);

/**
 * @brief Appends data to the buffer associated with the given socket.
 *
 * This function reallocates memory for the buffer data to accommodate the new data,
 * then copies the provided data into the buffer. The size of the buffer is updated accordingly.
 *
 * @param sock The socket associated with the buffer to which data will be appended.
 *             This value should be a valid index within the buffer array.
 * @param data A pointer to the data to be appended to the buffer.
 * @param size The size of the data to be appended to the buffer.
 *
 * @return This function does not return a value.
 *
 * @note If the provided socket index is out of bounds (greater than or equal to buffer_size),
 *       the function will return without performing any action.
 */
void bufferAppend(int sock, const char *data, size_t size);

/**
 * @brief Prints the contents of the buffer associated with the given socket and clears the buffer.
 *
 * This function checks if the provided socket index is within bounds. If it is, it then checks if the buffer
 * associated with the given socket contains any data. If data is present, it prints the data to the standard output
 * in the format "[sock]: \"data\"". After printing the data, the function clears the buffer by calling the
 * bufferClear() function.
 *
 * @param sock The socket associated with the buffer to be dumped. This value should be a valid index within the buffer array.
 *
 * @return This function does not return a value.
 *
 * @note If the provided socket index is out of bounds (greater than or equal to buffer_size),
 *       the function will return without performing any action.
 */
void bufferDump(int sock);
