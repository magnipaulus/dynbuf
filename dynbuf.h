#pragma once

#include <stdlib.h>

typedef struct dynbuf {
    size_t size;
    size_t capacity;
    char *content;
} dynbuf_t;

/*
 * Creation and destruction
 */
bool dynbuf_prealloc(dynbuf_t *buffer, size_t capacity);
bool dynbuf_new(dynbuf_t *buffer, const char *bytes);
bool dynbuf_nnew(dynbuf_t *buffer, const char *bytes, size_t size);
void dynbuf_free(dynbuf_t *buffer);

[[gnu::format(printf, 2, 3)]]
bool dynbuf_format(dynbuf_t *buffer, const char *restrict fmt, ...);

/*
 * Manipulation
 */
bool dynbuf_resize(dynbuf_t *buffer, size_t size);
bool dynbuf_append(dynbuf_t *buffer, const char *string);
bool dynbuf_nappend(dynbuf_t *buffer, const char *string, size_t size);
bool dynbuf_join(dynbuf_t *dest, dynbuf_t *buffer_a, dynbuf_t *buffer_b);
