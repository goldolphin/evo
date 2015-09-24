/**
 * @author goldolphin
 *         2015-09-23 16:54:54.
 */

#include "var_table.h"

typedef struct {
    symbol_list_t super;
    var_def_t def;
} var_list_t;

static symbol_list_t * new_node() {
    return &new_data(var_list_t)->super;
}

static void free_node(symbol_list_t * node) {
    free(container_of(node, var_list_t, super));
}

void var_table_init(var_table_t *table, size_t initial_capacity) {
    symbol_table_init(&table->super, initial_capacity, new_node, free_node);
}

void var_table_destroy(var_table_t * table) {
    symbol_table_destroy(&table->super);
}

bool var_table_add(var_table_t *table, string_t * name) {
    symbol_list_t *node = symbol_table_add(&table->super, name);
    if (node == NULL) {
        return false;
    }
    var_list_t *var_node = container_of(node, var_list_t, super);
    var_node->def.level = node->level;
    var_node->def.index = node->index;
    var_node->def.name = node->name;
    return true;
}

var_def_t * var_table_get(var_table_t *table, string_t * name) {
    symbol_list_t *node = symbol_table_get(&table->super, name);
    if (node == NULL) {
        return NULL;
    }
    return &container_of(node, var_list_t, super)->def;
}
