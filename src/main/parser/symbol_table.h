/**
 * @author goldolphin
 *         2015-09-21 11:44:44.
 */

#ifndef EVO_SYMBOL_TABLE_H
#define EVO_SYMBOL_TABLE_H

#include <utils/hashmap.h>
#include <utils/string.h>

struct symbol_list_s;
typedef struct symbol_list_s symbol_list_t;
struct symbol_list_s {
    symbol_list_t * outer;
    int level;
    int index;
    string_t * name;

    hashmap_iterator_t iter;
    symbol_list_t * sibling;
};

struct symbol_scope_s;
typedef struct symbol_scope_s symbol_scope_t;
struct symbol_scope_s {
    int count;
    symbol_list_t * list;
    symbol_scope_t * outer;
};

typedef symbol_list_t * (* symbol_list_new_node_t) ();
typedef void (* symbol_list_free_node_t) (symbol_list_t * node);

typedef struct {
    int level;
    symbol_scope_t * scopes;
    hashmap_t map;
    symbol_list_new_node_t new_node;
    symbol_list_free_node_t free_node;
} symbol_table_t;

void symbol_table_init(symbol_table_t * table, size_t initial_capacity, symbol_list_new_node_t new_node, symbol_list_free_node_t free_node);

void symbol_table_destroy(symbol_table_t * table);

symbol_list_t * symbol_table_add(symbol_table_t * table, string_t * name);

symbol_list_t * symbol_table_get(symbol_table_t * table, string_t * name);

void symbol_table_enter(symbol_table_t * table);

void symbol_table_exit(symbol_table_t * table);

#endif //EVO_SYMBOL_TABLE_H
