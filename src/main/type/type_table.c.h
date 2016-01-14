/**
 * @author caofuxiang
 *         2016-01-14 09:52:52.
 */

#ifndef EVO_TYPE_TABLE_C_H
#define EVO_TYPE_TABLE_C_H

#include <utils/hashmap.h>
#include "type.h"

typedef struct {
    type_info_t * info;
} type_def_t;

struct type_list_s;
typedef struct type_list_s type_list_t;
struct type_list_s {
    type_def_t def;

};

typedef struct {
    hashmap_t map;
} type_table_t;

#endif //EVO_TYPE_TABLE_C_H
