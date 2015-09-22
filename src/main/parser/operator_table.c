/**
 * @author goldolphin
 *         2015-09-15 17:23:23.
 */

#include <utils/string.h>
#include <utils/pair.h>
#include "operator_table.h"

#define OPERATOR_DEF_MAP_INITIAL_SIZE 128

static operator_def_t * make_operator_def(string_t * name, bool left2right, int precedence) {
    operator_def_t * def = new_data(operator_def_t);
    uint8_t * s = new_array(uint8_t, name->len);
    memcpy(s, name->value, (size_t) name->len);
    string_init(&def->name, s, name->len);
    def->left2right = left2right;
    def->precedence = precedence;
    return def;
}

static void destroy_operator_def(operator_def_t * def) {
    free(def->name.value);
    free(def);
}

void operator_table_init(operator_table_t *table) {
    hashmap_init1(&table->operator_def_map, OPERATOR_DEF_MAP_INITIAL_SIZE, string_hash_func, string_equal_func);
}

void operator_table_destroy(operator_table_t * table) {
    for (hashmap_iterator_t it = hashmap_begin(&table->operator_def_map);
         it != hashmap_end(&table->operator_def_map);
         it = hashmap_next(&table->operator_def_map, it)) {
        pair_t kv;
        hashmap_iterator_get(it, &kv);
        destroy_operator_def(kv.value);
    }
    hashmap_destroy(&table->operator_def_map);
}

bool operator_table_add(operator_table_t *table, string_t * name, bool left2right, int precedence) {
    if (hashmap_find(&table->operator_def_map, name) != NULL) {
        return false;
    }
    operator_def_t *def = make_operator_def(name, left2right, precedence);
    ensure(hashmap_put(&table->operator_def_map, &def->name, def));
    return true;
}

operator_def_t * operator_table_get(operator_table_t *table, string_t * value) {
    pair_t kv;
    if (hashmap_get(&table->operator_def_map, value, &kv)) {
        return kv.value;
    }
    return NULL;
}
