/**
 * @author goldolphin
 *         2015-09-21 11:44:44.
 */

#include <utils/string.h>
#include <utils/pair.h>
#include "symbol_table.h"



void symbol_table_init(symbol_table_t * table, size_t initial_capacity, symbol_list_free_node_t free_node) {
    table->level = 0;
    table->scopes = NULL;
    hashmap_init1(&table->map, initial_capacity, string_hash_func, string_equal_func);
    table->free_node = free_node;

    symbol_table_enter(table);
}

void symbol_table_destroy(symbol_table_t * table) {
    while (table->scopes != NULL) {
        symbol_table_exit(table);
    }
    hashmap_destroy(&table->map);
}

void * symbol_table_get(symbol_table_t * table, string_t * name) {
    hashmap_iterator_t it = hashmap_find(&table->map, name);
    if (it != hashmap_end(&table->map)) {
        pair_t kv;
        hashmap_iterator_get(it, &kv);
        return kv.value;
    }
    return NULL;
}

static inline symbol_scope_t * make_scope(symbol_list_t * list, symbol_scope_t * outer) {
    symbol_scope_t *scope = new_data(symbol_scope_t);
    scope->list = list;
    scope->outer = outer;
    return scope;
}

void symbol_table_enter(symbol_table_t * table) {
    ++ table->level;
    table->scopes = make_scope(NULL, table->scopes);
}

void symbol_table_exit(symbol_table_t * table) {
    -- table->level;
    symbol_scope_t * current = table->scopes;
    for (symbol_list_t * head = current->list; head != NULL; ) {
        symbol_list_t * next = head->next;
        table->free_node(head);
        head = next;
    }
    table->scopes = current->outer;
    free(current);
}
