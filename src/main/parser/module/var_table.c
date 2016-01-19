/**
 * @author goldolphin
 *         2015-09-23 16:54:54.
 */

#include "var_table.h"

void var_table_init(var_table_t *table, size_t initial_capacity) {
    symbol_table_init(&table->super, initial_capacity, sizeof(var_def_t));
}

void var_table_destroy(var_table_t * table) {
    symbol_table_destroy(&table->super);
}

bool var_table_add(var_table_t *table, string_t * name, type_t * type) {
    symbol_t *node = symbol_table_add(&table->super, name);
    if (node == NULL) {
        return false;
    }
    var_def_t *var_def = container_of(node, var_def_t, super);
    var_def->type = type;
    return true;
}

var_def_t * var_table_get(var_table_t *table, string_t * name) {
    symbol_t *node = symbol_table_get(&table->super, name);
    if (node == NULL) {
        return NULL;
    }
    return container_of(node, var_def_t, super);
}
