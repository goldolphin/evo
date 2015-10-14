/**
 * @author goldolphin
 *         2015-09-23 16:54:54.
 */

#ifndef EVO_VAR_TABLE_H
#define EVO_VAR_TABLE_H

#include "symbol_table.h"
#include "type.h"

typedef struct {
    int level;
    int index;
    type_t * type;
    string_t * name;
} var_def_t;

typedef struct {
    symbol_table_t super;
} var_table_t;

void var_table_init(var_table_t *table, size_t initial_capacity);

void var_table_destroy(var_table_t * table);

bool var_table_add(var_table_t *table, string_t * name, type_t * type);

var_def_t * var_table_get(var_table_t *table, string_t * name);

static inline void var_table_enter(var_table_t * table) {
    symbol_table_enter(&table->super);
}

static inline void var_table_exit(var_table_t * table) {
    symbol_table_exit(&table->super);
}

#endif //EVO_VAR_TABLE_H
