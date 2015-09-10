/**
 * @author caofuxiang
 *         2015-09-08 16:57:57.
 */

#ifndef EVO_STACK_H
#define EVO_STACK_H

#include <stddef.h>
#include <stdbool.h>
#include "memory.h"

#define DECLARE_STACK(NAME, TYPE) \
typedef struct { \
    size_t capacity; \
    size_t size; \
    TYPE * entries; \
} NAME##_t;

#define DEFINE_STACK(NAME, TYPE) \
void NAME##_init(NAME##_t * stack, size_t capacity) { \
    stack->capacity = capacity; \
    stack->size = 0; \
    stack->entries = new_array(TYPE, capacity); \
} \
 \
void NAME##_destroy(NAME##_t * stack) { \
    free(stack->entries); \
} \
 \
bool NAME##_push(NAME##_t * stack, TYPE value) { \
    if (stack->size >= stack->capacity) { \
        return false; \
    } \
    stack->entries[stack->size++] = value; \
    return true; \
} \
 \
bool NAME##_pop(NAME##_t * stack, TYPE* value) { \
    if (stack->size == 0) { \
        return false; \
    } \
    *value = stack->entries[--stack->size]; \
    return true; \
}

#endif //EVO_STACK_H
