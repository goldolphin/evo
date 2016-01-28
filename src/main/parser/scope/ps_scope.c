/**
 * @author caofuxiang
 *         2016-01-28 11:28:28.
 */

#include <utils/string.h>
#include <ir/ir_ref.h>
#include "ps_scope.h"

void ps_scope_init(ps_scope_t * scope, size_t initial_capacity) {
    hashmap_init1(&scope->local_map, initial_capacity, string_hash_func, string_equal_func);
}

void ps_scope_destroy(ps_scope_t * scope) {
    hashmap_destroy(&scope->local_map);
}

bool ps_scope_define(ps_scope_t * scope, string_t * name, int index) {
    pair_t kv;
    if (hashmap_get(&scope->local_map, name, &kv)) {
        return false;
    }
    ir_ref_t * ref = new_data(ir_ref_t);

    hashmap_put(&scope->local_map, string_dup(name),
    return true;
}