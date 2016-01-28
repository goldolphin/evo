/**
 * @author goldolphin
 *         2015-09-01 17:08:08.
 */

#include <utils/memory.h>
#include "ast.h"

static bool sbuilder_id(sbuilder_t * builder, ast_id_t * id) {
    if (id->prefix != NULL) {
        sbuilder_string(builder, id->prefix);
        sbuilder_str(builder, "$");
    }
    return sbuilder_string(builder, id->base);
}

static bool sbuilder_var_declare(sbuilder_t * builder, ast_var_declare_t * var_declare) {

    if (var_declare->type == NULL) {
        return sbuilder_id(builder, var_declare->name);
    } else {
        sbuilder_id(builder, var_declare->name);
        sbuilder_str(builder, ":");
        return sbuilder_id(builder, var_declare->type);
    }
}

static bool sbuilder_var_declare_list(sbuilder_t * builder, ast_var_declare_list_t * var_declare_list) {
    if (var_declare_list == NULL) {
        return sbuilder_str(builder, "NULL");
    }
    if (var_declare_list->next == NULL) {
        return sbuilder_var_declare(builder, var_declare_list->var);
    } else {
        sbuilder_var_declare(builder, var_declare_list->var);
        sbuilder_str(builder, ",");
        return sbuilder_var_declare_list(builder, var_declare_list->next);
    }
}

static void print_indent(int level, const char * str) {
    for (int i = 0; i < level; ++i) {
        printf("  ");
    }
    puts(str);
}

static void print_string(int level, string_t * str) {
    SBUILDER(builder, 1024);
    sbuilder_string(&builder, str);
    print_indent(level, builder.buf);
}

static void print_id(int level, ast_id_t * id) {
    SBUILDER(builder, 1024);
    sbuilder_id(&builder, id);
    print_indent(level, builder.buf);
}

static void print_var_declare(int level, ast_var_declare_t * var_declare) {
    SBUILDER(builder, 1024);
    sbuilder_var_declare(&builder, var_declare);
    print_indent(level, builder.buf);
}

static void print_var_declare_list(int level, ast_var_declare_list_t * var_declare_list) {
    SBUILDER(builder, 1024);
    sbuilder_var_declare_list(&builder, var_declare_list);
    print_indent(level, builder.buf);
}

static void print_statement_list(int level, ast_statement_list_t * statement_list) {
    if (statement_list == NULL) {
        return;
    }
    print_statement(level, statement_list->statement);
    print_statement_list(level, statement_list->next);
}

static void print_expr_list(int level, ast_expr_list_t * expr_list) {
    if (expr_list == NULL) {
        return;
    }
    print_statement(level, &expr_list->expr->super);
    print_expr_list(level, expr_list->next);
}

static void print_import(int level, ast_import_t * import) {
    SBUILDER(builder, 1024);
    sbuilder_str(&builder, "import(");
    sbuilder_id(&builder, import->name);
    sbuilder_str(&builder, ")");
    print_indent(level, builder.buf);
}

static void print_struct(int level, ast_struct_t * s) {
    print_indent(level, "struct");
    print_var_declare_list(level+1, s->members);
}

static void print_fun(int level, ast_fun_t * fun) {
    print_indent(level, "fun");
    print_var_declare_list(level+1, fun->params);
    print_id(level+1, fun->return_type);
    print_statement(level+1, &fun->body->super);
}

static void print_block(int level, ast_block_t * block) {
    print_indent(level, "block");
    print_statement_list(level+1, block->statements);
    print_statement(level+1, &block->the_last->super);
}

static void print_fun_apply(int level, ast_fun_apply_t * fun_apply) {
    print_indent(level, "fun_apply");
    print_statement(level+1, &fun_apply->function->super);
    print_expr_list(level+1, fun_apply->operands);
}

static void print_ref(int level, ast_ref_t * ref) {
    SBUILDER(builder, 1024);
    sbuilder_str(&builder, "ref(");
    sbuilder_id(&builder, ref->name);
    sbuilder_str(&builder, ")");
    print_indent(level, builder.buf);
}

static void print_struct_ref(int level, ast_struct_ref_t * ref) {
    SBUILDER(builder, 1024);
    sbuilder_format(&builder, "struct_ref(%d, ", ref->index);
    sbuilder_string(&builder, ref->name);
    sbuilder_str(&builder, ")");
    print_indent(level, builder.buf);
    print_statement(level+1, &ref->base->super);
}

static void print_let(int level, ast_let_t * let) {
    print_indent(level, "let");
    print_var_declare(level+1, let->var);
    print_statement(level+1, &let->expr->super);
}

static void print_str(int level, ast_str_t * str) {
    SBUILDER(builder, 1024);
    sbuilder_str(&builder, "str(");
    sbuilder_string(&builder, str->value);
    sbuilder_str(&builder, ")");
    print_indent(level, builder.buf);
}

static void print_double(int level, ast_double_t * d) {
    SBUILDER(builder, 1024);
    sbuilder_format(&builder, "double(%lf)", d->value);
    print_indent(level, builder.buf);
}

static void print_long(int level, ast_long_t * d) {
    SBUILDER(builder, 1024);
    sbuilder_format(&builder, "long(%ld)", d->value);
    print_indent(level, builder.buf);
}

const char *ast_category_name(ast_category_t category) {
    switch (category) {
#define AST_DEF(a, b, c, d) case a: return #a;
#include "ast_define.def"
#undef AST_DEF
        default: return NULL;
    }
}

enum {
    BASIC,
    STATEMENT,
    EXPR,
};

bool ast_is_expr(ast_category_t category) {
    switch (category) {
#define AST_DEF(a, b, c, d) case a: return c == EXPR;
#include "ast_define.def"
#undef AST_DEF
        default: return false;
    }
}

void print_statement(int level, ast_statement_t * statement) {
    switch (statement->category) {
#define AST_DEF(a, b, c, d) case a: print_##d(level, (void *)statement); break;
#include "ast_define.def"
#undef AST_DEF
        default: break;
    }
}
