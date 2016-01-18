/**
 * @author caofuxiang
 *         2016-01-18 15:51:51.
 */

#include <utils/pair.h>
#include "type_table.h"

void type_table_init(type_table_t *table, size_t initial_capacity) {
    hashmap_init1(&table->map, initial_capacity, string_hash_func, string_equal_func);
    fun_table_init(&table->fun_map, initial_capacity);
}

void type_table_destroy(type_table_t * table) {
    hashmap_destroy(&table->map);
    fun_table_destroy(&table->fun_map);
}

void type_table_declare(type_table_t *table, string_t * name) {
    pair_t kv;
    if (!hashmap_get(&table->map, name, &kv)) {
        type_t * type = new_data(type_t);
        type_init(type, TYPE_INFO_HOLDER);
        hashmap_put(&table->map, name, type);
    }
}

bool type_table_define(type_table_t *table, string_t * name, type_info_t * info) {
    pair_t kv;
    type_t * type;
    if (hashmap_get(&table->map, name, &kv)) {
        type = kv.value;
    } else {
        type = new_data(type_t);
        type_init(type, TYPE_INFO_HOLDER);
        hashmap_put(&table->map, name, type);
    }
    if (type->info == TYPE_INFO_HOLDER) {
        type->info = info;
        return true;
    }
    return false;
}

type_t * type_table_get(type_table_t *table, string_t * name) {
    pair_t kv;
    if (hashmap_get(&table->map, name, &kv)) {
        return kv.value;
    }
    return NULL;
}

type_t * type_table_get_fun(type_table_t * table, fun_info_t * key) {
    type_t * type = fun_table_get(&table->fun_map, key);
    if (type == NULL) {
        type = fun_table_put(&table->fun_map, key);
        ensure(type != NULL);
    }
    return type;
}