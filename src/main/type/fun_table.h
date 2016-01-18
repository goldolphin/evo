/**
 * @author caofuxiang
 *         2016-01-18 17:00:00.
 */

#ifndef EVO_FUN_TABLE_H
#define EVO_FUN_TABLE_H

#include <utils/hashmap.h>
#include "fun.h"

typedef struct {
    type_t **types;
    int size;
} type_list_t;

static inline void type_list_init(type_list_t * list, type_t **types, int size) {
    list->types= types;
    list->size = size;
}

/**
 * Function type table
 */
typedef struct {
    hashmap_t map; // fun info -> type
} fun_table_t;

void fun_table_init(fun_table_t * table, size_t initial_capacity);

void fun_table_destroy(fun_table_t * table);

type_t * fun_table_put(fun_table_t * table, fun_info_t * key);

type_t * fun_table_get(fun_table_t * table, fun_info_t * key);

#endif //EVO_FUN_TABLE_H
