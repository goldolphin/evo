/**
 * @author caofuxiang
 *         2015-09-25 14:56:56.
 */

#ifndef EVO_TYPE_H
#define EVO_TYPE_H

#include <utils/string.h>

typedef enum {
    TC_HOLDER,
    TC_BOOL,
    TC_INT,
    TC_LONG,
    TC_DOUBLE,
    TC_STRUCT,
    TC_FUN,
    TC_STRING,
} type_category_t;

typedef struct {
    type_category_t category;
    string_t * name;
} type_t;

extern type_t * HOLDER_TYPE;
extern type_t * BOOL_TYPE;
extern type_t * INT_TYPE;
extern type_t * LONG_TYPE;
extern type_t * DOUBLE_TYPE;
extern type_t * FUN_TYPE;
extern type_t * STRING_TYPE;

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
    var_declare_list_t * members;
    struct type_struct_s * parent;
} type_struct_t;

var_declare_t * struct_member_index(type_struct_t * s, string_t * name, int *index);

#endif //EVO_TYPE_H
