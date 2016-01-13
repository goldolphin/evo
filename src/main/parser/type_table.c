/**
 * @author goldolphin
 *         2015-09-25 14:14:14.
 */

#include "type_table.h"

void type_table_init(type_table_t *table, size_t initial_capacity) {
    symbol_table_init(&table->super, initial_capacity, sizeof(type_def_t));
}

void type_table_destroy(type_table_t * table) {
    symbol_table_destroy(&table->super);
}

void type_table_declare(type_table_t *table, string_t * name) {
    symbol_t *node = symbol_table_add(&table->super, name);
    if (node != NULL) {
        type_def_t *type_def = container_of(node, type_def_t, super);
        type_def->type = HOLDER_TYPE;
    }
}

bool type_table_define(type_table_t *table, string_t * name, type_t * type) {
    symbol_t *node = symbol_table_get(&table->super, name);
    if (node == NULL) {
        node = symbol_table_add(&table->super, name);
        type_def_t *type_def = container_of(node, type_def_t, super);
        type_def->type = type;
    } else {
        type_def_t *type_def = container_of(node, type_def_t, super);
        if (type_def->type != HOLDER_TYPE) {
            return false;
        }
        type_def->type = type;
    }
    return true;
}

type_def_t * type_table_get(type_table_t *table, string_t * name) {
    symbol_t *node = symbol_table_get(&table->super, name);
    if (node == NULL) {
        return NULL;
    }
    return container_of(node, type_def_t, super);
}
