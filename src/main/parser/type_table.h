/**
 * @author goldolphin
 *         2015-09-25 14:14:14.
 */

#ifndef EVO_TYPE_TABLE_H
#define EVO_TYPE_TABLE_H

#include "symbol_table.h"

typedef struct {
    int level;
    int index;
    string_t * name;
} type_def_t;

typedef struct {
    symbol_table_t super;
} type_table_t;

void type_table_init(type_table_t *table, size_t initial_capacity);

void type_table_destroy(type_table_t * table);

bool type_table_add(type_table_t *table, string_t * name);

type_def_t * type_table_get(type_table_t *table, string_t * name);

static inline void type_table_enter(type_table_t * table) {
    symbol_table_enter(&table->super);
}

static inline void type_table_exit(type_table_t * table) {
    symbol_table_exit(&table->super);
}

#endif //EVO_TYPE_TABLE_H
