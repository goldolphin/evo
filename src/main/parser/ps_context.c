/**
 * @author caofuxiang
 *         2016-01-29 16:47:47.
 */

#include <utils/string.h>
#include <utils/pair.h>
#include "ps_context.h"

#define MODULE_MAP_INITIAL_CAPACITY 64

typedef struct {
    list_entry_t super;
    ps_module_t * module;
} imported_entry_t;

static void clear_imported_modules(ps_context_t * context) {
    while (!list_is_empty(&context->imported_modules)) {
        list_entry_t *entry = list_pop_back(&context->imported_modules);
        free(container_of(entry, imported_entry_t, super));
    }
}

void ps_context_init(ps_context_t * context) {
    hashmap_init1(&context->module_map, MODULE_MAP_INITIAL_CAPACITY, string_hash_func, string_equal_func);
    list_init(&context->imported_modules);
    context->current_module = NULL;
}

void ps_context_destroy(ps_context_t * context) {
    for (hashmap_iterator_t iter = hashmap_begin(&context->module_map);
        iter != hashmap_end(&context->module_map);
        iter = hashmap_next(&context->module_map, iter)) {
        pair_t kv;
        hashmap_iterator_get(iter, &kv);
        ps_module_destroy(kv.value);
        free(kv.value);
    }
    hashmap_destroy(&context->module_map);
    clear_imported_modules(context);
}

bool ps_context_add_module(ps_context_t * context, string_t * name) {
    pair_t kv;
    if (hashmap_get(&context->module_map, name, &kv)) {
        return false;
    }
    ps_module_t * module = new_data(ps_module_t);
    ps_module_init(module, name);
    hashmap_put(&context->module_map, name, module);
    clear_imported_modules(context);
    context->current_module = module;
    return true;
}

ps_module_t * ps_context_get_module(ps_context_t * context, string_t * name) {
    pair_t kv;
    if (hashmap_get(&context->module_map, name, &kv)) {
        return kv.value;
    }
    return NULL;
}

bool ps_context_import_module(ps_context_t * context, string_t * name) {
    ps_module_t * to_import = ps_context_get_module(context, name);
    if (to_import == NULL) return false;

    for (list_entry_t * entry = list_peek_back(&context->imported_modules);
        entry != NULL;
        entry = list_prev(&context->imported_modules, entry)) {
        imported_entry_t *imported_entry = container_of(entry, imported_entry_t, super);
        if (imported_entry->module == to_import) {
            list_remove(&context->imported_modules, entry);
            list_push_back(&context->imported_modules, entry);
            return true;
        }
    }

    imported_entry_t * imported_entry = new_data(imported_entry_t);
    imported_entry->module = to_import;
    list_push_back(&context->imported_modules, &imported_entry->super);
    return true;
}

void ps_context_enter_scope(ps_context_t * context) {
    ps_module_enter_scope(context->current_module);
}

void ps_context_exit_scope(ps_context_t * context) {
    ps_module_exit_scope(context->current_module);
}

bool ps_context_add_var(ps_context_t * context, string_t * name) {
    return ps_module_add_var(context->current_module, name);
}

var_def_t * ps_context_get_var(ps_context_t * context, string_t * name);

var_def_t * ps_context_get_exported_var(ps_context_t * context, string_t * name);
