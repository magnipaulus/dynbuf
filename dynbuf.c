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

#include <stddef.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/_types/_ssize_t.h>

#include "dynbuf.h"

bool dynbuf_prealloc(dynbuf_t *buffer, size_t capacity) {
    // buffers are always null-terminated so the maximum size
    // is (size_t maximum value) - 1. Since size_t is unsigned
    // the only possible invalid size is exactly SIZE_MAX.
    if(capacity == SIZE_MAX) return false;
    char *content = calloc(1, capacity + 1);
    if(!content) {
        return false;
    }

    buffer->size = 0;
    buffer->capacity = capacity;
    buffer->content = content;

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
    buffer->size = size;

    return true;
}

void dynbuf_clear(dynbuf_t *buffer) {
    if(buffer->content) free(buffer->content);
    buffer->content = 0;
    buffer->capacity = 0;
    buffer->content = nullptr;
}

bool dynbuf_format(dynbuf_t *buffer, const char *restrict fmt, ...) {
    va_list vaa, vab;
    va_start(vaa, fmt);
    va_copy(vab, vaa);

    ssize_t need = vsnprintf(nullptr, 0, fmt, vab);
    va_end(vab);

    if(need < 0) {
        va_end(vaa);
        return false;
    }

    if(!dynbuf_prealloc(buffer, need)) {
        va_end(vaa);
        return false;
    }

    vsnprintf(buffer->content, need, fmt, vaa);
    va_end(vaa);

    buffer->size = need;

    return true;
}


/*
 * Manipulation
 */
bool dynbuf_resize(dynbuf_t *buffer, size_t size) {
    if(size == SIZE_MAX) {
        return false;
    }

    char *content = realloc(buffer->content, size + 1);
    if(!content) {
        return false;
    }
    buffer->content = content;

    buffer->capacity = size;
    if(buffer->size > size) {
        buffer->size = size;
    }
    buffer->content[size] = '\0';

    return true;
}

bool dynbuf_grow(dynbuf_t *buffer, size_t growth) {
    if(SIZE_MAX - buffer->capacity < growth) {
        return false;
    }
    return dynbuf_resize(buffer, buffer->size + growth);
}

bool dynbuf_shrink(dynbuf_t *buffer, size_t reduction) {
    if(buffer->capacity < reduction) {
        return false;
    }
    return dynbuf_resize(buffer, buffer->size - reduction);
}

bool dynbuf_minimize(dynbuf_t *buffer) {
    return dynbuf_resize(buffer, buffer->size);
}

bool dynbuf_append(dynbuf_t *buffer, const char *string) {
    return dynbuf_nappend(buffer, string, strlen(string));
}

bool dynbuf_nappend(dynbuf_t *buffer, const char *string, size_t size) {
    if(SIZE_MAX - buffer->size < size) {
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

bool dynbuf_join(dynbuf_t *dest, dynbuf_t *a, dynbuf_t *b) {
    if(SIZE_MAX - a->size < b->size) {
        return false;
    }

    size_t dsize = a->size + b->size;
    if(!dynbuf_prealloc(dest, dsize)) {
        return false;
    }

    memcpy(dest->content, a->content, a->size);
    memcpy((dest->content + a->size), b->content, b->size);
    dest->size = dsize;

    return true;
}
