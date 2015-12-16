/**
 * vector.h:
 *   Dynamic, type unsafe, heap-allocated array.
 *
 * Copyright (C) 2015 Emilio Cobos Álvarez <emiliocobos@usal.es>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef VECTOR_H
#define VECTOR_H

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

typedef struct vector {
    size_t capacity;
    size_t size;
    size_t element_size;
    void* buffer;
} vector_t;

#define VECTOR_DEFAULT_CAPACITY 8
#define VECTOR_INITIALIZER(elem_size)                                          \
    { 0, 0, elem_size, NULL }

#define vector_size(v) (v)->size
#define vector_capacity(v) (v)->capacity

bool vector_reserve(vector_t* v, size_t capacity);
bool vector_init(vector_t* v, size_t element_size, size_t initial_capacity);
bool vector_push(vector_t* v, const void* buff);
bool vector_set(vector_t* v, size_t index, const void* buff);
bool vector_get(vector_t*, size_t index, void* out_buff);
bool vector_delete(vector_t* v, size_t index);
void vector_destroy(vector_t* v);

#endif
