/**
 * @author caofuxiang
 *         2016-01-19 16:50:50.
 */

#include "vector.h"
#include "memory.h"

static void vector_realloc(vector_t *vector, size_t capacity) {
    vector_entry_t *new_entries = new_array(vector_entry_t, capacity);
    if (vector->size > 0) {
        memcpy(new_entries, vector->entries, vector->size * sizeof(vector_entry_t));
        free(vector->entries);
    }
    vector->entries = new_entries;
    vector->capacity = capacity;
}

void vector_init(vector_t * vector, size_t initial_capacity) {
    vector->capacity = 0;
    vector->size = 0;
    vector->entries = NULL;
    vector_realloc(vector, initial_capacity);
}

void vector_destroy(vector_t * vector) {
    free(vector->entries);
    vector->capacity = 0;
    vector->size = 0;
    vector->entries = NULL;
}

void vector_clear(vector_t * vector) {
    vector->size = 0;
}

void vector_push(vector_t * vector, void * value) {
    while (vector->size == vector->capacity) {
        vector_realloc(vector, 2*vector->capacity);
    }
    vector->entries[vector->size++].value = value;
}

bool vector_pop(vector_t * vector, void ** value) {
    if (vector->size == 0) return false;
    *value = vector->entries[--vector->size].value;
    return true;
}

void * vector_get(vector_t * vector, size_t index) {
    ensure(index >= 0 && index < vector->size);
    return vector->entries[index].value;
}
