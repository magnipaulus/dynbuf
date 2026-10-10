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
void dynbuf_clear(dynbuf_t *buffer);

[[gnu::format(printf, 2, 3)]]
bool dynbuf_format(dynbuf_t *buffer, const char *restrict fmt, ...);

/*
 * Manipulation
 */
bool dynbuf_resize(dynbuf_t *buffer, size_t size);
bool dynbuf_grow(dynbuf_t *buffer, size_t growth);
bool dynbuf_shrink(dynbuf_t *buffer, size_t reduction);
bool dynbuf_minimize(dynbuf_t *buffer);
bool dynbuf_append(dynbuf_t *buffer, const char *string);
bool dynbuf_nappend(dynbuf_t *buffer, const char *string, size_t size);
bool dynbuf_join(dynbuf_t *dest, dynbuf_t *buffer_a, dynbuf_t *buffer_b);
