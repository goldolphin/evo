/**
 * @author goldolphin
 *         2015-09-15 17:23:23.
 */

#include "operator_table.h"

typedef struct {
    symbol_list_t super;
    operator_def_t def;
} operator_list_t;

static operator_list_t * make_operator_list(string_t * name, bool left2right, int precedence, int level, symbol_list_t * next) {
    operator_list_t * list = new_data(operator_list_t);
    uint8_t * s = new_array(uint8_t, name->len);
    memcpy(s, name->value, (size_t) name->len);
    string_init(&list->def.name, s, name->len);
    list->def.left2right = left2right;
    list->def.precedence = precedence;
    list->def.level = level;
    list->super.next = next;
    return list;
}

static void free_node(symbol_list_t * node) {
    operator_list_t *n = container_of(node, operator_list_t, super);
    free(n->def.name.value);
    free(n);
}

void operator_table_init(operator_table_t *table, size_t initial_capacity) {
    symbol_table_init(&table->super, initial_capacity, free_node);
}

void operator_table_destroy(operator_table_t * table) {
    symbol_table_destroy(&table->super);
}

bool operator_table_add(operator_table_t *table, string_t * name, bool left2right, int precedence) {
    if (hashmap_find(&table->super.map, name) != NULL) {
        return false;
    }
    operator_list_t * head = make_operator_list(name, left2right, precedence, table->super.level, table->super.scopes->list);
    symbol_table_add(&table->super, &head->def.name, &head->def);
    return true;
}
