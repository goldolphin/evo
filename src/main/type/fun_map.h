/**
 * @author caofuxiang
 *         2016-01-14 15:50:50.
 */

#ifndef EVO_FUN_MAP_H
#define EVO_FUN_MAP_H

#include <utils/hashmap.h>
#include "fun.h"

typedef struct {
    hashmap_t map;
} fun_map_t;

void fun_map_init(fun_map_t * map, size_t initial_capacity);

fun_info_t * fun_map_get(fun_info_t * type);

#endif //EVO_FUN_MAP_H
