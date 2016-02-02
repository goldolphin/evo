/**
 * @author caofuxiang
 *         2016-01-29 16:42:42.
 */

#ifndef EVO_PS_MODULE_H
#define EVO_PS_MODULE_H

#include "symbol_table.h"

typedef struct {
    symbol_info_t super;
} var_def_t;

typedef struct {
    symbol_info_t super;
    bool left2right;
    int precedence;
    var_def_t * var;
} operator_def_t;

typedef enum {
    OPERATOR_TYPE_PREFIX,
    OPERATOR_TYPE_POSTFIX,
    OPERATOR_TYPE_BINARY,
    OPERATOR_TYPE_NUM,
} operator_type_t;

typedef struct {
    string_t * name;
    symbol_table_t var_table;
    symbol_table_t op_table[OPERATOR_TYPE_NUM];
} ps_module_t;

void ps_module_init(ps_module_t * module, string_t * name);

void ps_module_destroy(ps_module_t * module);

void ps_module_enter_scope(ps_module_t * module);

void ps_module_exit_scope(ps_module_t * module);

// var table
bool ps_module_add_var(ps_module_t * module, string_t * name);

var_def_t * ps_module_get_var(ps_module_t * module, string_t * name);

var_def_t * ps_module_get_exported_var(ps_module_t * module, string_t * name);

var_def_t * ps_module_add_imported_var(ps_module_t * module, ps_module_t * from, var_def_t * var);

var_def_t * ps_module_get_imported_var(ps_module_t * module, string_t * from_name, string_t * name);

// operator table
bool ps_module_add_op(ps_module_t * module, operator_type_t op_type, string_t * name, bool left2right, int precedence, var_def_t * var);

operator_def_t * ps_module_get_op(ps_module_t * module, operator_type_t op_type, string_t * name);

operator_def_t * ps_module_get_exported_op(ps_module_t * module, operator_type_t op_type, string_t * name);

operator_def_t * ps_module_add_imported_op(ps_module_t * module, operator_type_t op_type, ps_module_t * from, operator_def_t *op);

operator_def_t * ps_module_get_imported_op(ps_module_t * module, operator_type_t op_type, string_t * from_name, string_t * name);

#endif //EVO_PS_MODULE_H
