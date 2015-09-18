/**
 * @author caofuxiang
 *         2015-09-15 17:23:23.
 */

#ifndef EVO_OPERATOR_TABLE_H
#define EVO_OPERATOR_TABLE_H

#include <utils/hashmap.h>

typedef struct {
    string_t name;
    bool left2right;
    int precedence;
} operator_def_t;

typedef struct {
    hashmap_t operator_def_map;
} operator_table_t;

void operator_table_init(operator_table_t *table);

void operator_table_destroy(operator_table_t * table);

bool operator_table_add(operator_table_t *table, string_t * name, bool left2right, int precedence);

operator_def_t * operator_table_get(operator_table_t *table, string_t * value);

#endif //EVO_OPERATOR_TABLE_H
