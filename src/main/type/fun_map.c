/**
 * @author caofuxiang
 *         2016-01-14 15:50:50.
 */

#include "fun_map.h"

void fun_map_init(fun_map_t * map, size_t initial_capacity) {
    hashmap_init1(&map->map, initial_capacity, string_hash_func, string_equal_func);

}

fun_info_t * fun_map_get(fun_info_t * type);
