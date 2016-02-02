/**
 * @author caofuxiang
 *         2016-01-29 16:42:42.
 */

#include <utils/string.h>
#include "ps_module.h"

void ps_module_init(ps_module_t * module, string_t * name) {
    module->name = string_dup(name);
    symbol_table_init(&module->var_table, sizeof(var_def_t));
    for (int i = 0; i < OPERATOR_TYPE_NUM; ++i) {
        symbol_table_init(&module->op_table[i], sizeof(operator_def_t));
    }
}

void ps_module_destroy(ps_module_t * module) {
    string_free(module->name);
    symbol_table_destroy(&module->var_table);
    for (int i = 0; i < OPERATOR_TYPE_NUM; ++i) {
        symbol_table_destroy(&module->op_table[i]);
    }
}

void ps_module_enter_scope(ps_module_t * module) {
    symbol_table_enter_scope(&module->var_table);
    for (int i = 0; i < OPERATOR_TYPE_NUM; ++i) {
        symbol_table_enter_scope(&module->op_table[i]);
    }
}

void ps_module_exit_scope(ps_module_t * module) {
    symbol_table_exit_scope(&module->var_table);
    for (int i = 0; i < OPERATOR_TYPE_NUM; ++i) {
        symbol_table_exit_scope(&module->op_table[i]);
    }
}

bool ps_module_add_var(ps_module_t * module, string_t * name) {
    return symbol_table_add_symbol(&module->var_table, name) != NULL;
}

var_def_t * ps_module_get_var(ps_module_t * module, string_t * name) {
    symbol_info_t * info = symbol_table_get_symbol(&module->var_table, name);
    if (info == NULL) return NULL;
    return container_of(info, var_def_t, super);
}

var_def_t * ps_module_get_exported_var(ps_module_t * module, string_t * name) {
    symbol_info_t * info = symbol_table_get_exported_symbol(&module->var_table, name);
    if (info == NULL) return NULL;
    return container_of(info, var_def_t, super);
}

var_def_t * ps_module_add_imported_var(ps_module_t * module, ps_module_t * from, var_def_t * var) {
    symbol_info_t *info = symbol_table_add_imported_symbol(&module->var_table, from->name, var->super.name);
    if (info == NULL) return NULL;
    return container_of(info, var_def_t, super);
}

var_def_t * ps_module_get_imported_var(ps_module_t * module, string_t * from_name, string_t * name) {
    symbol_info_t *symbol_info = symbol_table_get_imported_symbol(&module->var_table, from_name, name);
    if (symbol_info != NULL) {
        return container_of(symbol_info, var_def_t, super);
    }
    return NULL;
}

bool ps_module_add_op(ps_module_t * module, operator_type_t op_type, string_t * name, bool left2right, int precedence, var_def_t * var) {
    symbol_info_t *symbol_info = symbol_table_add_symbol(&module->op_table[op_type], name);
    if (symbol_info == NULL) return false;
    operator_def_t * def = container_of(symbol_info, operator_def_t, super);
    def->left2right = left2right;
    def->precedence = precedence;
    def->var = var;
    return true;
}

operator_def_t * ps_module_get_op(ps_module_t * module, operator_type_t op_type, string_t * name) {
    symbol_info_t * info = symbol_table_get_symbol(&module->op_table[op_type], name);
    if (info == NULL) return NULL;
    return container_of(info, operator_def_t, super);
}

operator_def_t * ps_module_get_exported_op(ps_module_t * module, operator_type_t op_type, string_t * name) {
    symbol_info_t * info = symbol_table_get_exported_symbol(&module->op_table[op_type], name);
    if (info == NULL) return NULL;
    return container_of(info, operator_def_t, super);
}

operator_def_t * ps_module_add_imported_op(ps_module_t * module, operator_type_t op_type, ps_module_t * from, operator_def_t *op) {
    symbol_info_t *symbol_info = symbol_table_add_imported_symbol(&module->op_table[op_type], from->name, op->super.name);
    if (symbol_info != NULL) {
        operator_def_t *info = container_of(symbol_info, operator_def_t, super);
        info->left2right = op->left2right;
        info->precedence = op->precedence;
        info->var = op->var;
        return info;
    }
    return NULL;
}

operator_def_t * ps_module_get_imported_op(ps_module_t * module, operator_type_t op_type, string_t * from_name, string_t * name) {
    symbol_info_t *symbol_info = symbol_table_get_imported_symbol(&module->op_table[op_type], from_name, name);
    if (symbol_info != NULL) {
        return container_of(symbol_info, operator_def_t, super);
    }
    return NULL;
}
