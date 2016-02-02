/**
 * @author caofuxiang
 *         2016-01-29 15:37:37.
 */

#ifndef EVO_SYMBOL_TABLE_H
#define EVO_SYMBOL_TABLE_H

#include <utils/hashmap.h>
#include <utils/vector.h>

typedef struct {
    size_t index;
    string_t * from_name; // module name, NULL for non-imported symbols.
    string_t * name;
    string_t * full_name; // the key.
} symbol_info_t;

struct symbol_scope_s;
typedef struct symbol_scope_s symbol_scope_t;

struct symbol_info_entry_s;
typedef struct symbol_info_entry_s symbol_info_entry_t;

typedef struct {
    size_t info_size;
    symbol_info_entry_t * symbol_list;
    symbol_scope_t * scope_stack;
    symbol_scope_t * exported_scope;
    symbol_scope_t * imported_scope;
    size_t index;
} symbol_table_t;

void symbol_table_init(symbol_table_t * table, size_t info_size);

void symbol_table_destroy(symbol_table_t * table);

void symbol_table_enter_scope(symbol_table_t * table);

void symbol_table_exit_scope(symbol_table_t * table);

symbol_info_t * symbol_table_add_symbol(symbol_table_t * table, string_t * name);

symbol_info_t * symbol_table_get_symbol(symbol_table_t * table, string_t * name);

symbol_info_t * symbol_table_get_exported_symbol(symbol_table_t * table, string_t * name);

symbol_info_t * symbol_table_add_imported_symbol(symbol_table_t * table, string_t * from_name, string_t * name);

symbol_info_t * symbol_table_get_imported_symbol(symbol_table_t * table, string_t * from_name, string_t * name);

#endif //EVO_SYMBOL_TABLE_H
