/**
 * @author goldolphin
 *         2015-09-01 17:08:08.
 */

#ifndef EVO_IR_H
#define EVO_IR_H

#include <stdint.h>
#include <utils/string.h>

typedef enum {
#define IR_DEF(a, b, c, d) a,
#include "ir_define.def"
#undef IR_DEF
} ir_category_t;

typedef struct {
    string_t * name;
} ir_var_declare_t;

typedef struct ir_var_declare_list_s {
    ir_var_declare_t * var;
    struct ir_var_declare_list_s * next;
} ir_var_declare_list_t;

typedef struct {
    ir_category_t category;
} ir_statement_t;

typedef struct ir_statement_list_s {
    ir_statement_t * statement;
    struct ir_statement_list_s * next;
} ir_statement_list_t;

typedef struct {
    ir_statement_t super;
} ir_expr_t;

typedef struct ir_expr_list_s {
    ir_expr_t * expr;
    struct ir_expr_list_s * next;
} ir_expr_list_t;

#define DEFINE_STATEMENT(name, body) \
typedef struct {ir_statement_t super; body} name;

#define DEFINE_EXPR(name, body) \
typedef struct {ir_expr_t super; body} name;

DEFINE_STATEMENT(ir_import_t,
    string_t * module_name;
)

DEFINE_STATEMENT(ir_struct_t,
    ir_var_declare_list_t * members;
)

DEFINE_EXPR(ir_fun_t,
    int param_num;
    ir_var_declare_list_t * params;
    ir_expr_t * body;
)

DEFINE_EXPR(ir_block_t,
    ir_statement_list_t * statements;
    ir_expr_t * the_last;
)

DEFINE_EXPR(ir_fun_apply_t,
    ir_expr_t * function;
    ir_expr_list_t * operands;
)

DEFINE_EXPR(ir_ref_t,
    int level;
    int index;
    string_t * name;
)

DEFINE_EXPR(ir_struct_ref_t,
    ir_expr_t * base;
    int index;
    string_t * name;
)

DEFINE_STATEMENT(ir_let_t,
    ir_ref_t * ref;
    ir_expr_t * expr;
)

DEFINE_EXPR(ir_str_t,
    string_t * value;
)

DEFINE_EXPR(ir_double_t,
    double value;
)

DEFINE_EXPR(ir_long_t,
    long value;
)

const char *ir_type_name(ir_category_t category);

bool ir_is_expr(ir_category_t categoryT);

void print_statement(int level, ir_statement_t * statement);

#endif //EVO_IR_H
