/**
 * @author caofuxiang
 *         2016-01-18 20:36:36.
 */

#ifndef EVO_MODULE_H
#define EVO_MODULE_H

#include <type/type_table.h>
#include "operator_table.h"

typedef struct {
    var_table_t var_table;
    type_table_t type_table;
    operator_table_t prefix_table;
    operator_table_t postfix_table;
    operator_table_t binary_table;
} module_t;

#endif //EVO_MODULE_H
