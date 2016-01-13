/**
 * @author caofuxiang
 *         2016-01-13 15:53:53.
 */

#ifndef EVO_FUN_H
#define EVO_FUN_H

#include "type.h"

typedef struct {
    string_t * name;
    type_t type;
} fun_param_t;

typedef struct {
    type_info_t super;
    fun_param_t * params;
    int params_num;
    type_t return_type;
} fun_info_t;

void fun_info_init(fun_info_t * fun_info, fun_param_t * params, int params_num, type_t * return_type);

int fun_info_param_index(fun_info_t * fun_info, string_t * name);

fun_param_t * fun_info_param(fun_info_t * fun_info, int index);

type_t * fun_info_return_type(fun_info_t * fun_info);

bool sbuilder_fun_info(sbuilder_t * builder, fun_info_t * fun_info);

#endif //EVO_FUN_H
