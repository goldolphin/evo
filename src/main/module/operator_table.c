/**
 * @author goldolphin
 *         2015-09-15 17:23:23.
 */

#include "operator_table.h"

void operator_table_init(operator_table_t *table, size_t initial_capacity) {
    symbol_table_init(&table->super, initial_capacity, sizeof(operator_def_t));
}

void operator_table_destroy(operator_table_t * table) {
    symbol_table_destroy(&table->super);
}

bool operator_table_add(operator_table_t *table, string_t * name, bool left2right, int precedence, var_def_t * var) {
    symbol_t *node = symbol_table_add(&table->super, name);
    if (node == NULL) {
        return false;
    }
    operator_def_t *op_def = container_of(node, operator_def_t, super);
    op_def->left2right = left2right;
    op_def->precedence = precedence;
    op_def->var = var;
    return true;
}

operator_def_t * operator_table_get(operator_table_t *table, string_t * name) {
    symbol_t *node = symbol_table_get(&table->super, name);
    if (node == NULL) {
        return NULL;
    }
    return container_of(node, operator_def_t, super);
}
