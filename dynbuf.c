/*
  Copyright (c) 2026 MagniPaulus

  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:

  The above copyright notice and this permission notice shall be included in
  all copies or substantial portions of the Software.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
  THE SOFTWARE.
*/

#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "dynbuf.h"

#define MAX_DYNBUF_SIZE 4 * 1024 * 1024 // 4MiB

bool dynbuf_prealloc(dynbuf_t *buffer, size_t capacity) {
    if(capacity > MAX_DYNBUF_SIZE) return false;
    char *content = calloc(1, capacity + 1);
    if(!content) {
        return false;
    }

    buffer->content = content;
    buffer->capacity = capacity;
    buffer->size = 0;
    return true;
}

bool dynbuf_new(dynbuf_t *buffer, const char *bytes) {
    return dynbuf_nnew(buffer, bytes, strlen(bytes));
}

bool dynbuf_nnew(dynbuf_t *buffer, const char *bytes, size_t size) {
    if(!dynbuf_prealloc(buffer, size)) {
        return false;
    }

    memcpy(buffer->content, bytes, size);
    buffer->capacity = size;
    buffer->size = size;

    return true;
}

void dynbuf_free(dynbuf_t *buffer) {
    if(buffer->content) free(buffer->content);
    buffer->content = 0;
    buffer->capacity = 0;
    buffer->content = nullptr;
}


/*
 * CODICE VECCHIO
 */
bool dynbuf_format(dynbuf_t *buffer, const char *restrict fmt, ...) {
    if(!buffer) return false;

    va_list o, c;
    va_start(o, fmt);
    va_copy(c, o);

    ssize_t need = vsnprintf(nullptr, 0, fmt, c);
    va_end(c);

    if(need < 0) {
        va_end(o);
        return false;
    }

    char *content = calloc(1, need + 1);
    if(!content) {
        va_end(o);
        return false;
    }

    vsnprintf(content, need + 1, fmt, o);
    va_end(o);

    buffer->content = content;
    buffer->size = need;

    return true;
}


/*
 * Manipulation
 */
bool dynbuf_resize(dynbuf_t *buffer, size_t size) {
    char *content = realloc(buffer->content, size + 1);
    if(!content) {
        return false;
    }
    buffer->content = content;

    buffer->capacity = size;
    if(buffer->size > size) {
        buffer->size = size;
    }

    return true;
}

bool dynbuf_append(dynbuf_t *buffer, const char *string) {
    return dynbuf_nappend(buffer, string, strlen(string));
}

bool dynbuf_nappend(dynbuf_t *buffer, const char *string, size_t size) {
    if(size > SIZE_MAX - buffer->size) {
        return false;
    }

    size_t need = buffer->size + size;
    if(!buffer->content) {
        if(!dynbuf_prealloc(buffer, need)) {
            return false;
        }
    } else if(need > buffer->capacity && !dynbuf_resize(buffer, need)) {
        return false;
    }

    memcpy(buffer->content + buffer->size, string, size);
    buffer->size += size;

    return true;
}

bool dynbuf_join(dynbuf_t *dest, dynbuf_t *buffer_a, dynbuf_t *buffer_b) {
    dynbuf_t buff = {};
    if(!dynbuf_prealloc(&buff, buffer_a->size + buffer_b->size)) {
        return false;
    }

    memcpy(buff.content, buffer_a->content, buffer_a->size);
    memcpy(buff.content + buffer_a->size, buffer_b->content, buffer_b->size);
    *dest = buff;
    return true;
}
