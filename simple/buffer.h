#pragma once

#include <stddef.h>

void bufferCreate(size_t size);
void bufferAppend(int sock, const char *data, size_t size);
void bufferDump(int sock);
