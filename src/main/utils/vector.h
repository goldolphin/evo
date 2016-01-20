/**
 * @author caofuxiang
 *         2016-01-19 16:50:50.
 */

#ifndef EVO_VECTOR_H
#define EVO_VECTOR_H

#include <stddef.h>
#include <stdbool.h>

typedef struct {
    void * value;
} vector_entry_t;

typedef struct {
    size_t capacity;
    size_t size;
    vector_entry_t * entries;
} vector_t;

void vector_init(vector_t * vector, size_t initial_capacity);

void vector_destroy(vector_t * vector);

static inline size_t vector_size(vector_t * vector) {
    return vector->size;
}

void vector_clear(vector_t * vector);

void vector_push(vector_t * vector, void * value);

bool vector_pop(vector_t * vector, void ** value);

void * vector_get(vector_t * vector, size_t index);

#endif //EVO_VECTOR_H
