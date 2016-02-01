/**
 * @author caofuxiang
 *         2016-01-29 16:42:42.
 */

#ifndef EVO_PS_MODULE_H
#define EVO_PS_MODULE_H

#include "symbol_table.h"

typedef struct {
    string_t * name;
    symbol_table_t var_table;
    symbol_table_t prefix_table;
    symbol_table_t postfix_table;
    symbol_table_t binary_table;

    symbol_table_t imported_var_table;
    symbol_table_t imported_prefix_table;
    symbol_table_t imported_postfix_table;
    symbol_table_t imported_binary_table;
} ps_module_t;

typedef struct {
    symbol_info_t super;
} var_def_t;

typedef struct {
    symbol_info_t super;
    bool left2right;
    int precedence;
    var_def_t * var;
} operator_def_t;

typedef struct {
    symbol_info_t super;
    ps_module_t * module;
    var_def_t * var;
} imported_var_def_t;

typedef struct {
    symbol_info_t super;
    ps_module_t * module;
    operator_def_t * op;
} imported_operator_def_t;

void ps_module_init(ps_module_t * module, string_t * name);

void ps_module_destroy(ps_module_t * module);

void ps_module_enter_scope(ps_module_t * module);

void ps_module_exit_scope(ps_module_t * module);

bool ps_module_add_var(ps_module_t * module, string_t * name);

var_def_t * ps_module_get_var(ps_module_t * module, string_t * name);

var_def_t * ps_module_get_exported_var(ps_module_t * module, string_t * name);

bool ps_module_add_prefix(ps_module_t * module, string_t * name);

operator_def_t * ps_module_get_prefix(ps_module_t * module, string_t * name);

operator_def_t * ps_module_get_exported_prefix(ps_module_t * module, string_t * name);

bool ps_module_add_postfix(ps_module_t * module, string_t * name);

operator_def_t * ps_module_get_postfix(ps_module_t * module, string_t * name);

operator_def_t * ps_module_get_exported_postfix(ps_module_t * module, string_t * name);

bool ps_module_add_binary(ps_module_t * module, string_t * name);

operator_def_t * ps_module_get_binary(ps_module_t * module, string_t * name);

operator_def_t * ps_module_get_exported_binary(ps_module_t * module, string_t * name);

#endif //EVO_PS_MODULE_H
