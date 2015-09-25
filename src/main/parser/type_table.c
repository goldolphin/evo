/**
 * @author goldolphin
 *         2015-09-25 14:14:14.
 */

#include "type_table.h"

typedef struct {
    symbol_list_t super;
    type_def_t def;
} type_list_t;

static symbol_list_t * new_node() {
    return &new_data(type_list_t)->super;
}

static void free_node(symbol_list_t * node) {
    free(container_of(node, type_list_t, super));
}

void type_table_init(type_table_t *table, size_t initial_capacity) {
    symbol_table_init(&table->super, initial_capacity, new_node, free_node);
}

void type_table_destroy(type_table_t * table) {
    symbol_table_destroy(&table->super);
}

bool type_table_add(type_table_t *table, string_t * name) {
    symbol_list_t *node = symbol_table_add(&table->super, name);
    if (node == NULL) {
        return false;
    }
    type_list_t *type_node = container_of(node, type_list_t, super);
    type_node->def.level = node->level;
    type_node->def.index = node->index;
    type_node->def.name = node->name;
    return true;
}

type_def_t * type_table_get(type_table_t *table, string_t * name) {
    symbol_list_t *node = symbol_table_get(&table->super, name);
    if (node == NULL) {
        return NULL;
    }
    return &container_of(node, type_list_t, super)->def;
}
