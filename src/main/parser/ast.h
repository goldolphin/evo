/**
 * @author goldolphin
 *         2015-09-01 17:08:08.
 */

#ifndef EVO_AST_H
#define EVO_AST_H

#include <stdint.h>
#include <utils/string.h>

typedef enum {
#define AST_DEF(a, b, c, d) a,
#include "ast_define.def"
#undef AST_DEF
} ast_category_t;

typedef struct {
    string_t * prefix;
    string_t * base;
} ast_id_t;

typedef struct {
    ast_id_t * name;
    ast_id_t * type;
} ast_var_declare_t;

typedef struct ast_var_declare_list_s {
    ast_var_declare_t * var;
    struct ast_var_declare_list_s * next;
} ast_var_declare_list_t;

typedef struct {
    ast_category_t category;
} ast_statement_t;

typedef struct ast_statement_list_s {
    ast_statement_t * statement;
    struct ast_statement_list_s * next;
} ast_statement_list_t;

typedef struct {
    ast_statement_t super;
} ast_expr_t;

typedef struct ast_expr_list_s {
    ast_expr_t * expr;
    struct ast_expr_list_s * next;
} ast_expr_list_t;

#define DEFINE_STATEMENT(name, body) \
typedef struct {ast_statement_t super; body} name;

#define DEFINE_EXPR(name, body) \
typedef struct {ast_expr_t super; body} name;

DEFINE_STATEMENT(ast_import_t,
    ast_id_t * name;
)

DEFINE_STATEMENT(ast_struct_t,
    ast_var_declare_list_t * members;
)

DEFINE_EXPR(ast_fun_t,
    ast_var_declare_list_t * params;
    ast_id_t * return_type;
    ast_expr_t * body;
)

DEFINE_EXPR(ast_block_t,
    ast_statement_list_t * statements;
    ast_expr_t * the_last;
)

DEFINE_EXPR(ast_fun_apply_t,
    ast_expr_t * function;
    ast_expr_list_t * operands;
)

DEFINE_EXPR(ast_ref_t,
    ast_id_t * name;
)

DEFINE_EXPR(ast_struct_ref_t,
    ast_expr_t * base;
    int index;
    string_t * name;
)

DEFINE_STATEMENT(ast_let_t,
    ast_var_declare_t * var;
    ast_expr_t * expr;
)

DEFINE_EXPR(ast_str_t,
    string_t * value;
)

DEFINE_EXPR(ast_double_t,
    double value;
)

DEFINE_EXPR(ast_long_t,
    long value;
)

const char *ast_type_name(ast_category_t category);

bool ast_is_expr(ast_category_t categoryT);

void print_statement(int level, ast_statement_t * statement);

#endif //EVO_AST_H
