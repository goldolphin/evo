/**
 * @author caofuxiang
 *         2015-09-08 16:57:57.
 */

#include "stack.h"
#include "memory.h"

void stack_init(stack_t * stack, size_t capacity) {
    stack->capacity = capacity;
    stack->size = 0;
    stack->entries = new_array(TYPE, capacity);
}

void stack_destroy(stack_t * stack) {
    free(stack->entries);
}

bool stack_push(stack_t * stack, TYPE value) {
    if (stack->size >= stack->capacity) {
        return false;
    }
    stack->entries[stack->size++] = value;
    return true;
}

bool stack_pop(stack_t * stack, TYPE* value) {
    if (stack->size == 0) {
        return false;
    }
    *value = stack->entries[--stack->size];
    return true;
}
