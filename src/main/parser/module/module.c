/**
 * @author caofuxiang
 *         2016-01-18 20:36:36.
 */

#include <utils/vector.h>
#include "module.h"

#define SYMBOL_TABLE_INITIAL_CAPACITY 4096
#define IMPORTED_MODULES_INITIAL_CAPACITY 16

static inline bool define_op(module_t * module, operator_table_t * table, string_t * op, bool left2right, int precedence, string_t * var) {
    var_def_t * var_def = module_get_var(module, var);
    if (var_def == NULL) return false;
    return operator_table_add(table, op, left2right, precedence, var_def);
}

bool module_define_prefix(module_t * module, string_t * op, int precedence, string_t * var) {
    return define_op(module, &module->prefix_table, op, true, precedence, var);
}

bool module_define_postfix(module_t * module, string_t * op, int precedence, string_t * var) {
    return define_op(module, &module->postfix_table, op, true, precedence, var);
}

bool module_define_binary(module_t * module, string_t * op, bool left2right, int precedence, string_t * var) {
    return define_op(module, &module->binary_table, op, left2right, precedence, var);
}

void module_init(module_t * module) {
    var_table_init(&module->var_table, SYMBOL_TABLE_INITIAL_CAPACITY);
    type_table_init(&module->type_table, SYMBOL_TABLE_INITIAL_CAPACITY);
    operator_table_init(&module->prefix_table, SYMBOL_TABLE_INITIAL_CAPACITY);
    operator_table_init(&module->postfix_table, SYMBOL_TABLE_INITIAL_CAPACITY);
    operator_table_init(&module->binary_table, SYMBOL_TABLE_INITIAL_CAPACITY);
    vector_init(&module->imported_modules, IMPORTED_MODULES_INITIAL_CAPACITY);
}

void module_destroy(module_t * module) {
    var_table_destroy(&module->var_table);
    type_table_destroy(&module->type_table);
    operator_table_destroy(&module->prefix_table);
    operator_table_destroy(&module->postfix_table);
    operator_table_destroy(&module->binary_table);
    vector_destroy(&module->imported_modules);
}

void module_enter_scope(module_t * module) {
    var_table_enter(&module->var_table);
    operator_table_enter(&module->prefix_table);
    operator_table_enter(&module->postfix_table);
    operator_table_enter(&module->binary_table);
}

void module_exit_scope(module_t * module) {
    var_table_exit(&module->var_table);
    operator_table_exit(&module->prefix_table);
    operator_table_exit(&module->postfix_table);
    operator_table_exit(&module->binary_table);
}

bool module_import_module(module_t * module, module_t * to_import) {
    for (size_t i = 0; i < vector_size(&module->imported_modules); ++i) {
        if (to_import == vector_get(&module->imported_modules, i)) {
            return false;
        }
    }
    vector_push(&module->imported_modules, to_import);
    return true;
}
