/**
 * @author caofuxiang
 *         2016-01-29 16:47:47.
 */

#ifndef EVO_PS_CONTEXT_H
#define EVO_PS_CONTEXT_H

#include <utils/list.h>
#include "ps_module.h"

typedef struct {

} ps_symbol_t;

typedef struct {
    hashmap_t module_map; // name -> module
    list_t imported_modules;
    ps_module_t * current_module;
} ps_context_t;

void ps_context_init(ps_context_t * context);

void ps_context_destroy(ps_context_t * context);

bool ps_context_add_module(ps_context_t * context, string_t * name);

ps_module_t * ps_context_get_module(ps_context_t * context, string_t * name);

bool ps_context_import_module(ps_context_t * context, string_t * name);

void ps_context_enter_scope(ps_context_t * context);

void ps_context_exit_scope(ps_context_t * context);

bool ps_context_add_var(ps_context_t * context, string_t * name);

var_def_t * ps_context_get_var(ps_context_t * context, string_t * from_name, string_t * name);

bool ps_context_add_op(ps_context_t * context, operator_type_t type, string_t * name, bool left2right, int precedence, var_def_t * var);

operator_def_t * ps_context_get_op(ps_context_t * context, operator_type_t type, string_t * from_name, string_t * name);

#endif //EVO_PS_CONTEXT_H
