/**
 * @author caofuxiang
 *         2016-01-29 16:42:42.
 */

#include <utils/string.h>
#include "ps_module.h"
#include "symbol_table.h"

void ps_module_init(ps_module_t * module, string_t * name) {
    module->name = string_dup(name);
    symbol_table_init(&module->var_table, sizeof(var_def_t));
    symbol_table_init(&module->prefix_table, sizeof(operator_def_t));
    symbol_table_init(&module->postfix_table, sizeof(operator_def_t));
    symbol_table_init(&module->binary_table, sizeof(operator_def_t));

    symbol_table_init(&module->imported_var_table, sizeof(imported_var_def_t));
    symbol_table_init(&module->imported_prefix_table, sizeof(imported_operator_def_t));
    symbol_table_init(&module->imported_postfix_table, sizeof(imported_operator_def_t));
    symbol_table_init(&module->imported_binary_table, sizeof(imported_operator_def_t));
}

void ps_module_destroy(ps_module_t * module) {
    string_free(module->name);
    symbol_table_destroy(&module->var_table);
    symbol_table_destroy(&module->prefix_table);
    symbol_table_destroy(&module->postfix_table);
    symbol_table_destroy(&module->binary_table);

    symbol_table_destroy(&module->imported_var_table);
    symbol_table_destroy(&module->imported_prefix_table);
    symbol_table_destroy(&module->imported_postfix_table);
    symbol_table_destroy(&module->imported_binary_table);
}

void ps_module_enter_scope(ps_module_t * module) {
    symbol_table_enter_scope(&module->var_table);
    symbol_table_enter_scope(&module->prefix_table);
    symbol_table_enter_scope(&module->postfix_table);
    symbol_table_enter_scope(&module->binary_table);
}

void ps_module_exit_scope(ps_module_t * module) {
    symbol_table_exit_scope(&module->var_table);
    symbol_table_exit_scope(&module->prefix_table);
    symbol_table_exit_scope(&module->postfix_table);
    symbol_table_exit_scope(&module->binary_table);
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

#define BUF(buf) SBUILDER(buf, 1024)
#define BUF2STR(buf, str) string_t str; string_init(&str, buf.buf, sbuilder_len(&buf))

imported_var_def_t * ps_module_add_imported_var(ps_module_t * module, ps_module_t * from, var_def_t * var) {
    BUF(buf);
    sbuilder_string(&buf, from->name);
    sbuilder_str(&buf, ".");
    sbuilder_string(&buf, var->super.name);
    BUF2STR(buf, full_name);
    symbol_info_t *symbol_info = symbol_table_add_symbol(&module->var_table, &full_name);
    if (symbol_info != NULL) {
        imported_var_def_t *info = container_of(symbol_info, imported_var_def_t, super);
        info->module = from;
        info->var = var;
        return info;
    }
    return NULL;
}

imported_var_def_t * ps_module_get_imported_var(ps_module_t * module, string_t * from_name, string_t * name) {
    BUF(buf);
    sbuilder_string(&buf, from_name);
    sbuilder_str(&buf, ".");
    sbuilder_string(&buf, name);
    BUF2STR(buf, full_name);
    symbol_info_t *symbol_info = symbol_table_get_symbol(&module->var_table, &full_name);
    if (symbol_info != NULL) {
        return container_of(symbol_info, imported_var_def_t, super);
    }
    return NULL;
}

bool ps_module_add_prefix(ps_module_t * module, string_t * name) {
    return symbol_table_add_symbol(&module->prefix_table, name) != NULL;
}

operator_def_t * ps_module_get_prefix(ps_module_t * module, string_t * name) {
    symbol_info_t * info = symbol_table_get_symbol(&module->prefix_table, name);
    if (info == NULL) return NULL;
    return container_of(info, operator_def_t, super);
}

operator_def_t * ps_module_get_exported_prefix(ps_module_t * module, string_t * name) {
    symbol_info_t * info = symbol_table_get_exported_symbol(&module->prefix_table, name);
    if (info == NULL) return NULL;
    return container_of(info, operator_def_t, super);
}

bool ps_module_add_postfix(ps_module_t * module, string_t * name) {
    return symbol_table_add_symbol(&module->postfix_table, name) != NULL;
}

operator_def_t * ps_module_get_postfix(ps_module_t * module, string_t * name) {
    symbol_info_t * info = symbol_table_get_symbol(&module->postfix_table, name);
    if (info == NULL) return NULL;
    return container_of(info, operator_def_t, super);
}

operator_def_t * ps_module_get_exported_postfix(ps_module_t * module, string_t * name) {
    symbol_info_t * info = symbol_table_get_exported_symbol(&module->postfix_table, name);
    if (info == NULL) return NULL;
    return container_of(info, operator_def_t, super);
}

bool ps_module_add_binary(ps_module_t * module, string_t * name) {
    return symbol_table_add_symbol(&module->binary_table, name) != NULL;
}

operator_def_t * ps_module_get_binary(ps_module_t * module, string_t * name) {
    symbol_info_t * info = symbol_table_get_symbol(&module->binary_table, name);
    if (info == NULL) return NULL;
    return container_of(info, operator_def_t, super);
}

operator_def_t * ps_module_get_exported_binary(ps_module_t * module, string_t * name) {
    symbol_info_t * info = symbol_table_get_exported_symbol(&module->binary_table, name);
    if (info == NULL) return NULL;
    return container_of(info, operator_def_t, super);
}
