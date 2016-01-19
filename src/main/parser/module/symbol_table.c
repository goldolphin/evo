/**
 * @author goldolphin
 *         2015-09-21 11:44:44.
 */

#include <utils/string.h>
#include <utils/pair.h>
#include "symbol_table.h"

struct symbol_list_s;
typedef struct symbol_list_s symbol_list_t;
struct symbol_list_s {
    symbol_list_t * outer;
    hashmap_iterator_t iter;
    symbol_list_t * sibling;
    symbol_t symbol[0];
};

struct symbol_scope_s {
    int count;
    symbol_list_t * list;
    symbol_scope_t * outer;
};

static symbol_list_t *new_symbol_node(symbol_table_t *table) {
    return (symbol_list_t *) new_array(uint8_t, sizeof(symbol_list_t) + table->symbol_instance_size);
}

static void delete_symbol_node(symbol_table_t *table, symbol_list_t *node) {
    free(node);
}

void symbol_table_init(symbol_table_t * table, size_t initial_capacity, size_t symbol_instance_size) {
    table->level = 0;
    table->scopes = NULL;
    hashmap_init1(&table->map, initial_capacity, string_hash_func, string_equal_func);
    table->symbol_instance_size = symbol_instance_size;

    symbol_table_enter(table);
}

void symbol_table_destroy(symbol_table_t * table) {
    while (table->scopes != NULL) {
        symbol_table_exit(table);
    }
    hashmap_destroy(&table->map);
}

symbol_t * symbol_table_add(symbol_table_t * table, string_t * name) {
    hashmap_iterator_t it = hashmap_find(&table->map, name);
    symbol_list_t * outer = NULL;
    symbol_list_t * node;
    string_t * key;
    if (it == hashmap_end(&table->map)) {
        key = string_dup(name);
        node = new_symbol_node(table);
        it = hashmap_put(&table->map, key, node);
        ensure(it != hashmap_end(&table->map));
    } else {
        pair_t kv;
        hashmap_iterator_get(it, &kv);
        outer = kv.value;
        ensure(outer->symbol->level <= table->level);
        if (outer->symbol->level == table->level) {
            return NULL;
        }
        key = kv.key;
        node = new_symbol_node(table);
        hashmap_iterator_set_value(it, node);
    }
    node->outer = outer;
    node->symbol->level = table->level;
    node->symbol->index = table->scopes->count;
    node->symbol->name = key;

    node->iter = it;
    node->sibling = table->scopes->list;
    table->scopes->list = node;
    ++ table->scopes->count;
    return node->symbol;
}

symbol_t * symbol_table_get(symbol_table_t * table, string_t * name) {
    hashmap_iterator_t it = hashmap_find(&table->map, name);
    if (it != hashmap_end(&table->map)) {
        pair_t kv;
        hashmap_iterator_get(it, &kv);
        return ((symbol_list_t * )kv.value)->symbol;
    }
    return NULL;
}

static inline symbol_scope_t * make_scope(symbol_list_t * list, symbol_scope_t * outer) {
    symbol_scope_t *scope = new_data(symbol_scope_t);
    scope->count = 0;
    scope->list = list;
    scope->outer = outer;
    return scope;
}

void symbol_table_enter(symbol_table_t * table) {
    ++ table->level;
    table->scopes = make_scope(NULL, table->scopes);
}

void symbol_table_exit(symbol_table_t * table) {
    symbol_scope_t * current = table->scopes;
    for (symbol_list_t * head = current->list; head != NULL; ) {
        symbol_list_t * sibling = head->sibling;
        if (head->outer == NULL) {
            pair_t kv;
            hashmap_iterator_get(head->iter, &kv);
            hashmap_remove(&table->map, head->iter);
            string_free(kv.key);
        } else {
            hashmap_iterator_set_value(head->iter, head->outer);
        }
        delete_symbol_node(table, head);
        head = sibling;
    }
    table->scopes = current->outer;
    free(current);
    -- table->level;
}
