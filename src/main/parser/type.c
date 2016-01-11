/**
 * @author caofuxiang
 *         2015-09-25 14:56:56.
 */

#include "type.h"

#define DEFINE_TYPE(var, category, name) \
static string_t s_##var##_name = STRING_LITERAL(name); \
static type_t s_##var = {category, &s_##var##_name}; \
type_t * var = &s_##var;

DEFINE_TYPE(HOLDER_TYPE, TC_HOLDER, "Holder")
DEFINE_TYPE(UNIT_TYPE, TC_UNIT, "Unit")
DEFINE_TYPE(BOOL_TYPE, TC_BOOL, "Bool")
DEFINE_TYPE(INT_TYPE, TC_INT, "Int")
DEFINE_TYPE(LONG_TYPE, TC_LONG, "Long")
DEFINE_TYPE(DOUBLE_TYPE, TC_DOUBLE, "Double")
DEFINE_TYPE(FUN_TYPE, TC_FUN, "Fun")
DEFINE_TYPE(STRING_TYPE, TC_STRING, "String")

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
