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

static void bufferClear(int sock)
{
    free(buffer[sock].data);
    buffer[sock].data = NULL;
    buffer[sock].size = 0;
}

void bufferCreate(size_t size)
{
    buffer = calloc(size, sizeof(buffer_t));
    buffer_size = size;
}
void bufferAppend(int sock, const char *data, size_t size)
{
    if (sock >= buffer_size)
        return;

    buffer[sock].data = realloc(buffer[sock].data, buffer[sock].size + size);
    memcpy(buffer[sock].data + buffer[sock].size, data, size);
    buffer[sock].size += size;
}

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
