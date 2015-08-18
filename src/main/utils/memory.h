/**
 * 
 * @author goldolphin
 *         2015-05-02 23:16
 */
#ifndef EVO_MEMORY_H
#define EVO_MEMORY_H

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

/**
 * Utils.
 */
#define ensure(expr) do {if(!(expr)) abort();} while(0)

#define new_data(type) ((type *) malloc(sizeof(type)))

#define new_array(type, size) ((type *) malloc(sizeof(type)*(size)))

#define zero_data(a) memset(a, 0, sizeof *a)

#define zero_array(a, size) memset(a, 0, (sizeof *a)*(size))

#define array_size(a) ((sizeof a)/(sizeof a[0]))

#define int2ptr(n, type) ((type *)(intptr_t) (n))

#define uint2ptr(n, type) ((type *)(uintptr_t) (n))

#define ptr2int(p, type) ((type)(intptr_t) (p))

#define ptr2uint(p, type) ((type)(uintptr_t) (p))

#define container_of(ptr, type, member) ({ \
const typeof(((type *)0)->member) *__mptr = (ptr); \
(type *)((char *)__mptr - offsetof(type, member));})

#endif //EVO_MEMORY_H
