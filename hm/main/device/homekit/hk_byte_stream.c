/*
    Copyright (C) 2017 Tieto Poland Sp. z o. o.
*/

#include "hk_byte_stream.h"
#include <assert.h>

#define MAX(a, b) ((a) > (b) ? a : b)
#define MIN(a, b) ((a) < (b) ? a : b)

void hk_byte_stream_construct(hk_byte_stream_t *stream)
{
    assert(stream != NULL);
    memset(stream, 0, sizeof(*stream));
}

void hk_byte_stream_destroy(hk_byte_stream_t *stream)
{
    if (stream == NULL)
        return;
    free(stream->data);
    memset(stream, 0, sizeof(*stream));
}

bool hk_byte_stream_reserve(hk_byte_stream_t *stream, size_t new_capacity)
{
    assert(stream != NULL);
    if (stream->data_capacity < new_capacity)
    {
        byte_t *const temp_data = (byte_t *)hk_realloc(stream->data, stream->data_capacity, new_capacity);
        if (temp_data == NULL)
        {
            return false;
        }

        stream->data_capacity = new_capacity;
        stream->data = temp_data;
    }

    return true;
}

bool hk_byte_stream_prepend(hk_byte_stream_t *stream, const byte_t *buffer, size_t buffer_size)
{
    assert(stream != NULL);
    assert(buffer != NULL);

    if (!hk_byte_stream_reserve(stream, stream->data_size + buffer_size))
    {
        return false;
    }

    memmove(&stream->data[buffer_size], stream->data, stream->data_size);
    memcpy(stream->data, buffer, buffer_size);
    stream->data_size += buffer_size;

    return true;
}

bool hk_byte_stream_append(hk_byte_stream_t *stream, const byte_t *buffer, size_t buffer_size)
{
    assert(stream != NULL);
    assert(buffer != NULL);

    if (!hk_byte_stream_reserve(stream, stream->data_size + buffer_size))
    {
        return false;
    }

    memcpy(&stream->data[stream->data_size], buffer, buffer_size);
    stream->data_size += buffer_size;

    return true;
}

void *hk_realloc(void *old_ptr, size_t old_size, size_t new_size)
{
    void *new_ptr = calloc(1, new_size);
    if (new_ptr == NULL)
    {
        return new_ptr;
    }

    if (old_ptr != NULL)
    {
        memcpy(new_ptr, old_ptr, MIN(old_size, new_size));
    }

    free(old_ptr);
    return new_ptr;
}

char *hk_strdup(const char *string)
{
    size_t len = strlen(string);
    return (char *)hk_memdup(string, len + 1);
}

char *hk_strndup(const char *string, size_t size)
{
    char *dup_mem = (char *)malloc(size + 1);
    if (dup_mem == NULL)
    {
        return dup_mem;
    }

    dup_mem[size] = 0;
    memcpy(dup_mem, string, size);
    return dup_mem;
}

void *hk_memdup(const void *source, size_t size)
{
    char *dup_mem = (char *)malloc(size);
    if (dup_mem == NULL)
    {
        return dup_mem;
    }

    memcpy(dup_mem, source, size);
    return dup_mem;
}
