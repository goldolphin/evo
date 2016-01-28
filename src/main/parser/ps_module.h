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


typedef struct {
    symbol_table_t var_table;
    symbol_table_t prefix_table;
    symbol_table_t postfix_table;
    symbol_table_t binary_table;
} ps_module_t;
#endif //EVO_PS_MODULE_H
