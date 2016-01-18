/**
 * @author caofuxiang
 *         2016-01-18 15:51:51.
 */

#ifndef EVO_TYPE_TABLE_H
#define EVO_TYPE_TABLE_H

#include <utils/hashmap.h>
#include <utils/string.h>
#include "type.h"
#include "fun.h"
#include "fun_table.h"

typedef struct {
    hashmap_t map; // name -> type
    fun_table_t fun_map;
} type_table_t;

void type_table_init(type_table_t *table, size_t initial_capacity);

void type_table_destroy(type_table_t * table);

void type_table_declare(type_table_t *table, string_t * name);

bool type_table_define(type_table_t *table, string_t * name, type_info_t * info);

type_t * type_table_get(type_table_t *table, string_t * name);

type_t * type_table_get_fun(type_table_t * table, fun_info_t * key);

#endif //EVO_TYPE_TABLE_H
