/**
 * @file task.hpp
 * @brief This file contains the declaration of the Task struct.
 *
 * The Task struct is used to represent asynchronous tasks or coroutines.
 * It provides the necessary components for creating and managing coroutines,
 * allowing for non-blocking I/O operations and asynchronous programming.
 *
 * @author Vikman Fernandez-Castro
 * @date July 13, 2024
 */

#pragma once

#include <iostream>
#include <coroutine>

using namespace std;

struct Task
{
    struct promise_type
    {
        /**
         * @brief Returns a Task object representing the coroutine.
         *
         * @return The function returns a Task object.
         */
        Task get_return_object() { return Task{}; }

        /**
         * @brief Specifies the initial suspension point for the coroutine.
         *
         * @return The function returns std::suspend_never, indicating that the coroutine will not suspend initially.
         */
        std::suspend_never initial_suspend() { return {}; }

        /**
         * @brief Specifies the final suspension point for the coroutine.
         *
         * @return The function returns std::suspend_never, indicating that the coroutine will not suspend at the end.
         */
        std::suspend_never final_suspend() noexcept { return {}; }

        /**
         * @brief Specifies the behavior when the coroutine returns.
         *
         * In this case, the coroutine returns void, so the function does not have any specific behavior.
         */
        void return_void() {}

        /**
         * @brief Specifies the behavior when an unhandled exception occurs within the coroutine.
         *
         * In this case, the function terminates the program using std::terminate().
         */
        void unhandled_exception() { std::terminate(); }
    };
};
