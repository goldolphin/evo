/**
 * @author goldolphin
 *         2015-09-15 17:23:23.
 */

#ifndef EVO_OPERATOR_TABLE_H
#define EVO_OPERATOR_TABLE_H

#include <utils/hashmap.h>
#include "symbol_table.h"

typedef struct {
    string_t name;
    bool left2right;
    int precedence;
    int level;
} operator_def_t;

typedef struct {
    symbol_table_t super;
} operator_table_t;

void operator_table_init(operator_table_t *table, size_t initial_capacity);

void operator_table_destroy(operator_table_t * table);

bool operator_table_add(operator_table_t *table, string_t * name, bool left2right, int precedence);

static inline operator_def_t * operator_table_get(operator_table_t *table, string_t * name) {
    return symbol_table_get(&table->super, name);
}

static inline void operator_table_enter(operator_table_t * table) {
    symbol_table_enter(&table->super);
}

static inline void operator_table_exit(operator_table_t * table) {
    symbol_table_exit(&table->super);
}

#endif //EVO_OPERATOR_TABLE_H
