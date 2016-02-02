/**
 * @author caofuxiang
 *         2016-01-29 15:37:37.
 */

#include <utils/string.h>
#include <utils/pair.h>
#include "symbol_table.h"

#define SCOPE_MAP_INITIAL_CAPACITY 64

struct symbol_scope_s {
    hashmap_t symbol_map;   // name -> info
    symbol_scope_t * outer;
};

static symbol_scope_t * symbol_scope_new(symbol_scope_t * outer) {
    symbol_scope_t * scope = new_data(symbol_scope_t);
    hashmap_init1(&scope->symbol_map, SCOPE_MAP_INITIAL_CAPACITY, string_hash_func, string_equal_func);
    scope->outer = outer;
    return scope;
}

static void symbol_scope_free(symbol_scope_t * scope) {
    hashmap_destroy(&scope->symbol_map);
    free(scope);
}

struct symbol_info_entry_s {
    symbol_info_t * info;
    symbol_info_entry_t * next;
};

static symbol_info_entry_t * symbol_info_entry_new(size_t info_size, size_t index, string_t * from_name, string_t * name,
                                                   string_t * full_name, symbol_info_entry_t * next) {
    symbol_info_entry_t * entry = new_data(symbol_info_entry_t);
    entry->info = (symbol_info_t *) new_array(char, info_size);
    entry->info->index = index;
    entry->info->name = string_dup(from_name);
    entry->info->name = string_dup(name);
    entry->info->full_name = string_dup(full_name);
    entry->next = next;
    return entry;
}

static void symbol_info_entry_free(symbol_info_entry_t * entry) {
    string_free(entry->info->from_name);
    string_free(entry->info->name);
    string_free(entry->info->full_name);
    free(entry->info);
    free(entry);
}

void symbol_table_init(symbol_table_t * table, size_t info_size) {
    table->info_size = info_size;
    table->symbol_list = NULL;
    table->scope_stack = NULL;
    table->index = 0;
    symbol_table_enter_scope(table);
    table->exported_scope = table->scope_stack;
    table->imported_scope = symbol_scope_new(NULL);
}

void symbol_table_destroy(symbol_table_t * table) {
    while (table->symbol_list != NULL) {
        symbol_info_entry_t *entry = table->symbol_list;
        table->symbol_list = entry->next;
        symbol_info_entry_free(entry);
    }        
    while (table->scope_stack != NULL) {
        symbol_table_exit_scope(table);
    }
    symbol_scope_free(table->imported_scope);
}

void symbol_table_enter_scope(symbol_table_t * table) {
    table->scope_stack = symbol_scope_new(table->scope_stack);
}

void symbol_table_exit_scope(symbol_table_t * table) {
    symbol_scope_t *scope = table->scope_stack;
    table->scope_stack = scope->outer;
    symbol_scope_free(scope);
}

static symbol_info_t * symbol_scope_add_symbol(symbol_table_t * table, symbol_scope_t *scope,
                                               string_t * from_name, string_t * name, string_t * full_name) {
    pair_t kv;
    if (hashmap_get(&scope->symbol_map, name, &kv)) {
        return NULL;
    }
    table->symbol_list = symbol_info_entry_new(table->info_size, table->index, from_name, name, full_name, table->symbol_list);
    hashmap_put(&scope->symbol_map, table->symbol_list->info->name, table->symbol_list->info);
    ++ table->index;
    return table->symbol_list->info;
}

static symbol_info_t * symbol_scope_get_symbol(symbol_scope_t *scope, string_t * name) {
    pair_t kv;
    if (hashmap_get(&scope->symbol_map, name, &kv)) {
        return kv.value;
    }
    return NULL;
}


symbol_info_t * symbol_table_add_symbol(symbol_table_t * table, string_t * name) {
    return symbol_scope_add_symbol(table, table->scope_stack, NULL, name, name);
}

symbol_info_t * symbol_table_get_symbol(symbol_table_t * table, string_t * name) {
    return symbol_scope_get_symbol(table->scope_stack, name);
}

symbol_info_t * symbol_table_get_exported_symbol(symbol_table_t * table, string_t * name) {
    return symbol_scope_get_symbol(table->exported_scope, name);
}

#define BUF(buf) SBUILDER(buf, 1024)
#define BUF2STR(buf, str) string_t str; string_init(&str, buf.buf, sbuilder_len(&buf))

symbol_info_t * symbol_table_add_imported_symbol(symbol_table_t * table, string_t * from_name, string_t * name) {
    BUF(buf);
    sbuilder_string(&buf, from_name);
    sbuilder_str(&buf, ".");
    sbuilder_string(&buf, name);
    BUF2STR(buf, full_name);
    return symbol_scope_add_symbol(table, table->imported_scope, from_name, name, &full_name);
}

symbol_info_t * symbol_table_get_imported_symbol(symbol_table_t * table, string_t * from_name, string_t * name) {
    BUF(buf);
    sbuilder_string(&buf, from_name);
    sbuilder_str(&buf, ".");
    sbuilder_string(&buf, name);
    BUF2STR(buf, full_name);
    return symbol_scope_get_symbol(table->imported_scope, &full_name);
}

bool sbuilder_symbol_info(sbuilder_t * builder, symbol_info_t * info) {
    sbuilder_format(builder, "%d, ", info->index);
    if (info->from_name != NULL) {
        sbuilder_string(builder, info->from_name);
        sbuilder_str(builder, ".");
    }
    return sbuilder_string(builder, info->name);
}
