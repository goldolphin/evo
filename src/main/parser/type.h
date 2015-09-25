/**
 * @author caofuxiang
 *         2015-09-25 14:56:56.
 */

#ifndef EVO_TYPE_H
#define EVO_TYPE_H

#include <utils/string.h>

typedef enum {
    TC_BOOL,
    TC_LONG,
    TC_DOUBLE,
    TC_STRUCT,
    TC_FUN,
} type_category_t;

typedef struct {
    type_category_t category;
    string_t * name;
} type_t;

typedef struct {
    string_t * name;
    type_t * type;
} var_declare_t;

typedef struct var_declare_list_s {
    var_declare_t * var;
    struct var_declare_list_s * next;
} var_declare_list_t;

typedef struct type_struct_s {
    type_t super;
    var_declare_list_t members;
    struct type_struct_s * parent;
} type_struct_t;

typedef struct {
    type_t super;
    var_declare_list_t params;
    type_t * return_type;
} type_fun_t;

#endif //EVO_TYPE_H
