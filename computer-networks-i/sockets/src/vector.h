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
