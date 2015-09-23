/**
 * @author goldolphin
 *         2015-09-23 16:54:54.
 */

#include "var_table.h"

typedef struct {
    symbol_list_t super;
    var_def_t def;
} var_list_t;

static var_list_t * make_var_list(string_t * name, int level, symbol_list_t * next) {
    var_list_t * list = new_data(var_list_t);
    uint8_t * s = new_array(uint8_t, name->len);
    memcpy(s, name->value, (size_t) name->len);
    string_init(&list->def.name, s, name->len);
    list->def.level = level;
    list->super.next = next;
    return list;
}

static void free_node(symbol_list_t * node) {
    var_list_t *n = container_of(node, var_list_t, super);
    free(n->def.name.value);
    free(n);
}

void var_table_init(var_table_t *table, size_t initial_capacity) {
    symbol_table_init(&table->super, initial_capacity, free_node);
}

void var_table_destroy(var_table_t * table) {
    symbol_table_destroy(&table->super);
}

bool var_table_add(var_table_t *table, string_t * name) {
    if (hashmap_find(&table->super.map, name) != NULL) {
        return false;
    }
    var_list_t * head = make_var_list(name, table->super.level, table->super.scopes->list);
    symbol_table_add(&table->super, &head->def.name, &head->def);
    return true;
}
