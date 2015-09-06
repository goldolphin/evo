/**
 * @author goldolphin
 *         2015-09-01 17:08:08.
 */

#include "ast.h"

const char *ast_type_name(ast_type_t type) {
    switch (type) {
#define AST_DEF(a, b, c, d) case a: return #a;
#include "ast_define.def"
#undef AST_DEF
        default: return NULL;
    }
}

bool ast_is_expr(ast_type_t type) {
    enum {
        BASIC,
        STATEMENT,
        EXPR,
    };
    switch (type) {
#define AST_DEF(a, b, c, d) case a: return c == EXPR;
#include "ast_define.def"
#undef AST_DEF
        default: return false;
    }
}
