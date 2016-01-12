/**
 * @author goldolphin
 *         2015-09-21 11:44:44.
 */

#ifndef EVO_SYMBOL_TABLE_H
#define EVO_SYMBOL_TABLE_H

#include <utils/hashmap.h>
#include <utils/string.h>

typedef struct {
    int level;
    int index;
    string_t * name;
} symbol_t;

struct symbol_scope_s;
typedef struct symbol_scope_s symbol_scope_t;

typedef struct {
    int level;
    symbol_scope_t * scopes;
    hashmap_t map;
    size_t symbol_instance_size;
} symbol_table_t;

void symbol_table_init(symbol_table_t * table, size_t initial_capacity, size_t symbol_instance_size);

void symbol_table_destroy(symbol_table_t * table);

symbol_t * symbol_table_add(symbol_table_t * table, string_t * name);

symbol_t * symbol_table_get(symbol_table_t * table, string_t * name);

void symbol_table_enter(symbol_table_t * table);

void symbol_table_exit(symbol_table_t * table);

#endif //EVO_SYMBOL_TABLE_H
