/**
 * @author goldolphin
 *         2015-09-01 17:08:08.
 */

#ifndef EVO_AST_H
#define EVO_AST_H

#include <stdint.h>
#include <utils/string.h>

typedef enum {
#define AST_DEF(a, b) a,
#include "ast_define.def"
#undef AST_DEF
} ast_type_t;

const char *ast_type_name(ast_type_t type);

typedef struct {
    string_t name;
} ast_id_t;

typedef struct ast_cid_s {
    ast_id_t id;
    struct ast_cid_s * parent;
} ast_cid_t;

typedef struct {
    ast_id_t id;
    ast_cid_t type;
} ast_var_declare_t;

typedef struct ast_var_declare_list_s {
    ast_var_declare_t var;
    struct ast_var_declare_list_s * next;
} ast_var_declare_list_t;

typedef struct {

} ast_statement_t;

typedef struct ast_statement_list_s {
    ast_statement_t statement;
    struct ast_statement_list_s * next;
} ast_statement_list_t;

typedef struct {

} ast_expr_t;

typedef struct ast_expr_list_s {
    ast_expr_t expr;
    struct ast_expr_list_s * next;
} ast_expr_list_t;

typedef struct {
    ast_cid_t module;
} ast_import_t;

typedef struct {
    ast_id_t id;
    ast_var_declare_list_t * members;
    ast_cid_t * parent;
} ast_struct_t;

typedef struct {
    ast_var_declare_t var;
    ast_expr_t expr;
} ast_let_t;

typedef struct {
    ast_var_declare_list_t * params;
    ast_cid_t return_type;
    ast_expr_t body;
} ast_fun_t;

typedef struct {
    ast_statement_list_t * statements;
    ast_expr_t the_last;
} ast_block_t;

typedef struct {
    ast_expr_t function;
    ast_expr_list_t * operands;
} ast_fun_apply_t;

typedef struct {
    ast_cid_t id;
    ast_expr_t * base;
} ast_ref_t;

typedef struct {
    string_t value;
} ast_str_t;

typedef struct {
    double value;
} ast_double_t;

#endif //EVO_AST_H
