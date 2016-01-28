/**
 * @author caofuxiang
 *         2016-01-29 16:42:42.
 */

#include <utils/string.h>
#include "ps_module.h"

void ps_module_init(ps_module_t * module) {
    symbol_table_init(&module->var_table);
    symbol_table_init(&module->prefix_table);
    symbol_table_init(&module->postfix_table);
    symbol_table_init(&module->binary_table);
}

void ps_module_destroy(ps_module_t * module) {
    symbol_table_destroy(&module->var_table);
    symbol_table_destroy(&module->prefix_table);
    symbol_table_destroy(&module->postfix_table);
    symbol_table_destroy(&module->binary_table);
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

bool ps_module_add_var(ps_module_t * module, string_t * name, void * extra);

symbol_info_t * ps_module_get_symbol(ps_module_t * module, string_t * name);

symbol_info_t * ps_module_get_exported_symbol(ps_module_t * module, string_t * name);
