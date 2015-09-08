/**
 * @author caofuxiang
 *         2015-09-08 16:57:57.
 */

#ifndef EVO_STACK_H
#define EVO_STACK_H

#include <stddef.h>
#include <stdbool.h>

typedef int TYPE;

typedef struct {
    size_t capacity;
    size_t size;
    TYPE * entries;
} stack_t;
#endif //EVO_STACK_H
