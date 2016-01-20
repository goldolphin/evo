/**
 * @author caofuxiang
 *         2016-01-18 20:36:36.
 */

#ifndef EVO_MODULE_H
#define EVO_MODULE_H

#include <type/type_table.h>
#include <utils/vector.h>
#include "operator_table.h"

typedef struct {
    var_table_t var_table;
    type_table_t type_table;
    operator_table_t prefix_table;
    operator_table_t postfix_table;
    operator_table_t binary_table;
    vector_t imported_modules;
} module_t;

void module_init(module_t * module);

void module_destroy(module_t * module);

void module_enter_scope(module_t * module);

void module_exit_scope(module_t * module);

static inline var_def_t * module_get_var(module_t * module, string_t * name) {
    return var_table_get(&module->var_table, name);
}

static inline bool module_define_var(module_t * module, string_t * name, type_t * type) {
    return var_table_add(&module->var_table, name, type);
}

bool module_define_prefix(module_t * module, string_t * op, int precedence, string_t * var);

bool module_define_postfix(module_t * module, string_t * op, int precedence, string_t * var);

bool module_define_binary(module_t * module, string_t * op, bool left2right, int precedence, string_t * var);

static inline operator_def_t * module_get_prefix(module_t * module, string_t * name) {
    return operator_table_get(&module->prefix_table, name);
}

static inline operator_def_t * module_get_postfix(module_t * module, string_t * name) {
    return operator_table_get(&module->postfix_table, name);
}

static inline operator_def_t * module_get_binary(module_t * module, string_t * name) {
    return operator_table_get(&module->binary_table, name);
}

static inline type_t * module_get_type(module_t * module, string_t * name) {
    return type_table_get(&module->type_table, name);
}

static inline void module_declare_type(module_t * module, string_t * name) {
    type_table_declare(&module->type_table, name);
}

static inline bool module_define_type(module_t * module, string_t * name, type_info_t * info) {
    return type_table_define(&module->type_table, name, info);
}

bool module_import_module(module_t * module, module_t * to_import);

static inline size_t module_imported_modules_count(module_t * module) {
    return vector_size(&module->imported_modules);
}

static inline module_t * module_imported_module(module_t * module, size_t index) {
    return vector_get(&module->imported_modules, index);
}

#endif //EVO_MODULE_H
