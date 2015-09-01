/**
 * @author goldolphin
 *         2015-09-01 17:08:08.
 */

#ifndef EVO_AST_H
#define EVO_AST_H

#include <stdint.h>
#include <utils/string.h>

typedef struct {
    string_t name;
} ast_id_t;

typedef struct {
    ast_id_t id;
} ast_var_declare_t;

typedef struct ast_var_declare_list_s {
    ast_var_declare_t var;
    struct ast_var_declare_list_s * next;
} ast_var_declare_list_t;

typedef struct ast_module_id_s {
    ast_id_t id;
    struct ast_module_id_s * parent;
} ast_module_id_t;

typedef struct {
    ast_id_t id;
    ast_var_declare_list_t * members;

} ast_struct_t;


#endif //EVO_AST_H
