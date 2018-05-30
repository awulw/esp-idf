/*
    Copyright (C) 2017 Tieto Poland Sp. z o. o.
*/

#ifndef HOMEKIT_HK_BYTE_STREAM_H
#define HOMEKIT_HK_BYTE_STREAM_H

#include "../device/hk_api.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

typedef uint8_t byte_t; /// Represents an 8-bit byte. Used to differentiate from
                        /// zero-terminated strings.

typedef struct
{
    byte_t *data;         /// Owning pointer to the first byte of the allocated bytes.
                          /// Call \ref hk_byte_stream_destroy to release its memory.
    size_t data_capacity; /// The maximal capacity of the currently allocated \ref
                          /// data array (in bytes)
    size_t data_size;     /// Current size of \ref data (in bytes)
} hk_byte_stream_t;

// TODO(DJ): doxygen docs
HK_NOT_NULL_ARGS(1)
void hk_byte_stream_construct(hk_byte_stream_t *stream);

void hk_byte_stream_destroy(hk_byte_stream_t *stream);

HK_WARN_UNUSED_RESULT
HK_NOT_NULL_ARGS(1)
bool hk_byte_stream_reserve(hk_byte_stream_t *stream, size_t new_capacity);

HK_WARN_UNUSED_RESULT
HK_NOT_NULL_ARGS(1, 2)
bool hk_byte_stream_prepend(hk_byte_stream_t *stream, const byte_t *buffer, size_t buffer_size);

HK_WARN_UNUSED_RESULT
HK_NOT_NULL_ARGS(1, 2)
bool hk_byte_stream_append(hk_byte_stream_t *stream, const byte_t *buffer, size_t buffer_size);

HK_WARN_UNUSED_RESULT
void *hk_realloc(void *old_ptr, size_t old_size, size_t new_size);

HK_WARN_UNUSED_RESULT
HK_NOT_NULL_ARGS(1)
char *hk_strdup(const char *string);

HK_WARN_UNUSED_RESULT
HK_NOT_NULL_ARGS(1)
char *hk_strndup(const char *string, size_t size);

HK_WARN_UNUSED_RESULT
HK_NOT_NULL_ARGS(1)
void *hk_memdup(const void *source, size_t size);

#endif // HOMEKIT_HK_BYTE_STREAM_H
