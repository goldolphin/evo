/**
 * @author caofuxiang
 *         2016-01-13 15:53:53.
 */

#include "fun.h"
#include "type.h"

void fun_info_init(fun_info_t * fun_info, fun_param_t * params, int params_num, type_t * return_type) {
    type_info_init(&fun_info->super, TC_FUN);
    fun_info->params = params;
    fun_info->params_num = params_num;
    type_copy(&fun_info->return_type, return_type);
}

fun_param_t * fun_info_param(fun_info_t * fun_info, int index) {
    if (index >= 0 && index < fun_info->params_num) {
        return &fun_info->params[index];
    }
    return NULL;
}

bool sbuilder_fun_info(sbuilder_t * builder, fun_info_t * fun_info) {
    for (int i = 0; i < fun_info->params_num; ++i) {
        if (i > 0) {
            sbuilder_str(builder, ", ");
        }
        sbuilder_type(builder, &fun_info->params[i].type);
    }
    sbuilder_str(builder, ": ");
    return sbuilder_type(builder, &fun_info->return_type);
}
