# TCP Server Implementations

**Procedural C vs. Coroutine-based C++**

This repository contains two implementations of a TCP server: one using the procedural programming model in C and the other using coroutine-based asynchronous programming in C++. The server listens on a port and asynchronously receives data from clients, accumulating the data in memory and printing it to stdout upon client disconnection. The project aims to compare and understand the differences between procedural programming and coroutines in C++.

## Project Structure

- `server-simple` (C): Implementation using the procedural programming model.
- `server-cr` (C++): Implementation using coroutines for asynchronous programming.

## Functionality

- Listens on a TCP port.
- Asynchronously receives data from each client.
- Accumulates data in memory.
- Prints data to stdout upon client disconnection.

## Objective

The goal of this project is to compare and understand the differences between the procedural programming model in C and the use of coroutines in C++ for asynchronous programming.

## Supported Platforms

- Linux
- macOS

## Building and Running

### Prerequisites

- CMake 3.14 or higher
- A C/C++ compiler (gcc, clang, etc.)

```bash
cmake -B build
cmake --build build
```

### server-simple

Usage:

```
build/simple/server-simple <port>
```

### server-cr

Usage:

```
build/coroutine/server-cr <port>
```

### Example Client

```
echo "Hello World" | nc -w0 <addr> <port>
```
