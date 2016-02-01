/**
 * @author caofuxiang
 *         2016-02-01 16:38:38.
 */

#include "symbol_name.h"

void symbol_name_init(symbol_name_t * symbol_name, string_t * module_name, string_t * name) {
    symbol_name->module_name = string_dup(module_name);
    symbol_name->name = string_dup(name);
}

void symbol_name_destroy(symbol_name_t * symbol_name) {
    string_free(symbol_name->module_name);
    string_free(symbol_name->name);
}

symbol_name_t * symbol_name_new(string_t * module_name, string_t * name) {
    symbol_name_t * symbol_name = new_data(symbol_name_t);
    symbol_name_init(symbol_name, module_name, name);
    return symbol_name;
}

void symbol_name_free(symbol_name_t * symbol_name) {
    symbol_name_destroy(symbol_name);
    free(symbol_name);
}

bool symbol_name_equals(symbol_name_t * name1, symbol_name_t * name2) {
    return string_equals(name1->module_name, name2->module_name) && string_equals(name1->name, name2->name);
}

size_t symbol_name_hash_func(void * key) {
    symbol_name_t * symbol_name = key;
    size_t h = 0;
    if (symbol_name->module_name != NULL) {
        h = string_hash_func(symbol_name->module_name);
    }
    h = 31*h + string_hash_func(symbol_name->name);
    return h;
}

bool symbol_name_equal_func (void * key1, void * key2) {
    return symbol_name_equals(key1, key2);
}
