/**
 * @author caofuxiang
 *         2016-01-18 20:36:36.
 */

#include "module.h"

#define SYMBOL_TABLE_INITIAL_CAPACITY 4096

static inline var_def_t * module_get_var(module_t * module, string_t * name) {
    return var_table_get(&module->var_table, name);
}

static inline bool module_define_var(module_t * module, string_t * name, type_t * type) {
    return var_table_add(&module->var_table, name, type);
}

static inline void add_op(module_t * module, operator_table_t * table, const char * op, int precedence, const char * var) {
    string_t var_name;
    string_init(&var_name, (uint8_t *) var, (int) strlen(var));
    var_def_t * var_def = module_get_var(module, &var_name);

    string_t op_name;
    string_init(&op_name, (uint8_t *) op, (int) strlen(op));
    operator_table_add(table, &op_name, true, precedence, var_def);
}

void module_add_prefix(module_t * module, const char * op, int precedence, const char * var) {
    add_op(module, &module->prefix_table, op, precedence, var);
}

void module_add_postfix(module_t * module, const char * op, int precedence, const char * var) {
    add_op(module, &module->postfix_table, op, precedence, var);
}

void module_add_binary(module_t * module, const char * op, bool left2right, int precedence, const char * var) {
    add_op(module, &module->binary_table, op, precedence, var);
}

void module_init(module_t * module) {
    var_table_init(&module->var_table, SYMBOL_TABLE_INITIAL_CAPACITY);
    type_table_init(&module->type_table, SYMBOL_TABLE_INITIAL_CAPACITY);
    operator_table_init(&module->prefix_table, SYMBOL_TABLE_INITIAL_CAPACITY);
    operator_table_init(&module->postfix_table, SYMBOL_TABLE_INITIAL_CAPACITY);
    operator_table_init(&module->binary_table, SYMBOL_TABLE_INITIAL_CAPACITY);
}

void module_destroy(module_t * module) {
    var_table_destroy(&module->var_table);
    type_table_destroy(&module->type_table);
    operator_table_destroy(&module->prefix_table);
    operator_table_destroy(&module->postfix_table);
    operator_table_destroy(&module->binary_table);
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
