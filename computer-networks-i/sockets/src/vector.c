/**
 * vector.c:
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
#include "vector.h"

#define AS_CHAR_BUFF(v) ((char*)v)

bool vector_init(vector_t* v, size_t element_size, size_t initial_capacity) {
    v->buffer = NULL;
    v->element_size = element_size;
    v->size = 0;
    v->capacity = 0;

    if (initial_capacity)
        return vector_reserve(v, initial_capacity);

    return true;
}

bool vector_reserve(vector_t* v, size_t capacity) {
    if (v->capacity >= capacity)
        return true; // No action needed

    if (v->buffer == NULL) {
        assert(v->size == 0);
        v->buffer = malloc(v->element_size * capacity);
    } else {
        v->buffer = realloc(v->buffer, v->element_size * capacity);
    }

    v->capacity = capacity;

    return v->buffer != NULL;
}

bool vector_push(vector_t* v, const void* buff) {
    if (v->size == v->capacity)
        if (!vector_reserve(v, v->capacity ? v->capacity * 2
                                           : VECTOR_DEFAULT_CAPACITY))
            return false;

    return vector_set(v, v->size++, buff);
}

bool vector_set(vector_t* v, size_t index, const void* buff) {
    if (index >= v->size)
        return false;

    memcpy(AS_CHAR_BUFF(v->buffer) + index * v->element_size,
           AS_CHAR_BUFF(buff), v->element_size);
    return true;
}

bool vector_get(vector_t* v, size_t index, void* out_buff) {
    if (index >= v->size || out_buff == NULL)
        return false;

    memcpy(AS_CHAR_BUFF(out_buff),
           AS_CHAR_BUFF(v->buffer) + index * v->element_size, v->element_size);
    return true;
}

bool vector_delete(vector_t* v, size_t index) {
    if (index >= v->size)
        return false;

    if (index == v->size - 1) {
        v->size--;
        return true;
    }

    memmove(AS_CHAR_BUFF(v->buffer) + index * v->element_size,
            AS_CHAR_BUFF(v->buffer) + (index + 1) * v->element_size,
            (v->size - index - 1) * v->element_size);
    v->size--;
    return true;
}

void vector_destroy(vector_t* v) {
    if (v->buffer)
        free(v->buffer);

    v->buffer = NULL;
}
