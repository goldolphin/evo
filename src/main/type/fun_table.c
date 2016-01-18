/**
 * @author caofuxiang
 *         2016-01-18 17:00:00.
 */

#include <utils/pair.h>
#include "fun_table.h"

static size_t hash_func (void * key) {
    fun_info_t * info = key;
    size_t h = ptr2uint(info->return_type, size_t);
    for (int i = 0; i < info->params_num; i ++) {
        h = 31*h + ptr2uint(info->params[i].type, size_t);
    }
    return h;
}

static bool equal_func (void * key1, void * key2) {
    fun_info_t * info1 = key1;
    fun_info_t * info2 = key2;
    if (info1->return_type != info2->return_type ||
            info1->params_num != info2->params_num) {
        return false;
    }
    for (int i = 0; i < info1->params_num; ++i) {
        if (info1->params[i].type != info2->params[i].type) return false;
    }
    return true;
}

void fun_table_init(fun_table_t * table, size_t initial_capacity) {
    hashmap_init1(&table->map, initial_capacity, hash_func, equal_func);
}

void fun_table_destroy(fun_table_t * table) {
    hashmap_destroy(&table->map);
}

type_t * fun_table_put(fun_table_t * table, fun_info_t * key) {
    pair_t kv;
    if (hashmap_get(&table->map, key, &kv)) {
        return NULL;
    }
    fun_info_t * info = new_data(fun_info_t);
    fun_param_t * params = new_array(fun_param_t, key->params_num);
    for (int i = 0; i < key->params_num; ++i) {
        params[i].type = key->params[i].type;
    }
    fun_info_init(info, params, key->params_num, key->return_type);
    type_t * type = new_data(type_t);
    type_init(type, &info->super);
    hashmap_put(&table->map, info, type);
    return type;
}

type_t * fun_table_get(fun_table_t * table, fun_info_t * key) {
    pair_t kv;
    if (hashmap_get(&table->map, key, &kv)) {
        return kv.value;
    }
    return NULL;
}
