/**
 * @author goldolphin
 *         2015-09-15 17:23:23.
 */

#include "operator_table.h"

typedef struct {
    symbol_list_t super;
    operator_def_t def;
} operator_list_t;

static symbol_list_t * new_node() {
    return &new_data(operator_list_t)->super;
}

static void free_node(symbol_list_t * node) {
    operator_list_t *n = container_of(node, operator_list_t, super);
    free(n->def.name.value);
    free(n);
}

void operator_table_init(operator_table_t *table, size_t initial_capacity) {
    symbol_table_init(&table->super, initial_capacity, new_node, free_node);
}

void operator_table_destroy(operator_table_t * table) {
    symbol_table_destroy(&table->super);
}

bool operator_table_add(operator_table_t *table, string_t * name, bool left2right, int precedence, var_def_t * var) {
    symbol_list_t *node = symbol_table_add(&table->super, name);
    if (node == NULL) {
        return false;
    }
    operator_list_t *op_node = container_of(node, operator_list_t, super);
    uint8_t * s = new_array(uint8_t, name->len);
    memcpy(s, name->value, (size_t) name->len);
    string_init(&op_node->def.name, s, name->len);
    op_node->def.left2right = left2right;
    op_node->def.precedence = precedence;
    op_node->def.var = var;
    return true;
}

operator_def_t * operator_table_get(operator_table_t *table, string_t * name) {
    symbol_list_t *node = symbol_table_get(&table->super, name);
    if (node == NULL) {
        return NULL;
    }
    return &container_of(node, operator_list_t, super)->def;
}
