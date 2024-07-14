/**
 * @file server.h
 * @brief This file contains the declaration for the serve() function.
 *
 * The server.h file provides the declaration for the serve() function, which starts a server listening on the specified port.
 *
 * @author Vikman Fernandez-Castro
 * @date July 7, 2024
 */

#pragma once

/**
 * @brief Starts a server listening on the specified port.
 *
 * This function initializes the server, sets up the poll set, and starts listening for incoming connections.
 * It continuously handles incoming connections and data using the poll() system call.
 *
 * @param port The port number on which the server should listen for incoming connections.
 *
 * @return This function does not return a value.
 */
void serve(unsigned port);
