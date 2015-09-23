/**
 * @author goldolphin
 *         2015-09-22 19:34:34.
 */

#ifndef EVO_LIST_H
#define EVO_LIST_H

#include <stddef.h>
#include <stdbool.h>
#include "memory.h"

#define DECLARE_LIST(NAME, TYPE) \
struct NAME##_s; \
typedef struct NAME##_s NAME##_t; \
struct NAME##_s { \
    TYPE data; \
    NAME##_t *next; \
}; \
 \
static inline void NAME##_init(NAME##_t * list, TYPE data, NAME##_t *next) { \
    list->data = data; \
    list->next = next; \
}

#endif //EVO_LIST_H
