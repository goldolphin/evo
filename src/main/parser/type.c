/**
 * @author caofuxiang
 *         2015-09-25 14:56:56.
 */

#include "type.h"

static string_t s_holder_type_name = {(uint8_t *) "Holder", 6};
static type_t s_holder_type = {TC_HOLDER, &s_holder_type_name};
type_t * HOLDER_TYPE = &s_holder_type;

static string_t s_bool_type_name = {(uint8_t *) "Bool", 4};
static type_t s_bool_type = {TC_BOOL, &s_bool_type_name};
type_t * BOOL_TYPE = &s_bool_type;

static string_t s_int_type_name = {(uint8_t *) "Int", 3};
static type_t s_int_type = {TC_INT, &s_int_type_name};
type_t * INT_TYPE = &s_int_type;

static string_t s_long_type_name = {(uint8_t *) "Long", 4};
static type_t s_long_type = {TC_LONG, &s_long_type_name};
type_t * LONG_TYPE = &s_long_type;

static string_t s_double_type_name = {(uint8_t *) "Double", 6};
static type_t s_double_type = {TC_DOUBLE, &s_double_type_name};
type_t * DOUBLE_TYPE = &s_double_type;

static string_t s_fun_type_name = {(uint8_t *) "Fun", 3};
static type_t s_fun_type = {TC_FUN, &s_fun_type_name};
type_t * FUN_TYPE = &s_fun_type;

static string_t s_string_type_name = {(uint8_t *) "String", 6};
static type_t s_string_type = {TC_STRING, &s_string_type_name};
type_t * STRING_TYPE = &s_string_type;

static var_declare_t * var_declare_list_index(var_declare_list_t * list, string_t * name, int *index) {
    if (list == NULL) return NULL;
    if (string_equals(name, list->var->name)) {
        return list->var;
    }
    *index += 1;
    return var_declare_list_index(list->next, name, index);
}

static int struct_member_num_rec(type_struct_t * s, int n) {
    if (s == NULL) return n;
    for (var_declare_list_t * list = s->members; list != NULL; list = list->next) {
        ++ n;
    }
    return struct_member_num_rec(s->parent, n);
}

static var_declare_t * struct_member_index_rec(type_struct_t * s, string_t * name, int *index) {
    if (s == NULL) return NULL;
    var_declare_t * var = var_declare_list_index(s->members, name, index);
    if (var != NULL) {
        *index += struct_member_num_rec(s->parent, 0);
        return var;
    }
    return struct_member_index_rec(s->parent, name, index);
}

var_declare_t * struct_member_index(type_struct_t * s, string_t * name, int *index) {
    *index = 0;
    return struct_member_index_rec(s, name, index);
}
