/**
 * @author goldolphin
 *         2015-08-28 16:35:35.
 */

#include <lexer/token.h>
#include "parser.h"
#include "utils.h"
#include "ast.h"

#define SYMBOL_TABLE_INITIAL_CAPACITY 4096

// Utilities

void parser_init(parser_t * parser) {
}

void parser_destroy(parser_t * parser) {
}

// Basic
ast_id_t * make_id(string_t * prefix, string_t * base) {
    ast_id_t * id = new_data(ast_id_t);
    id->prefix = string_dup(prefix);
    id->base = string_dup(base);
    return id;
}

ast_var_declare_t * make_var_declare(ast_id_t * name, ast_id_t * type) {
    ast_var_declare_t * var_declare = new_data(ast_var_declare_t);
    var_declare->name = name;
    var_declare->type = type;
    return var_declare;
}

ast_var_declare_list_t * make_var_declare_list(ast_var_declare_t * var, ast_var_declare_list_t * next) {
    ast_var_declare_list_t * var_declare_list = new_data(ast_var_declare_list_t);
    var_declare_list->var = var;
    var_declare_list->next = next;
    return var_declare_list;
}

ast_statement_list_t * make_statement_list(ast_statement_t * statement, ast_statement_list_t * next) {
    ast_statement_list_t * list = new_data(ast_statement_list_t);
    list->statement = statement;
    list->next = next;
    return list;
}

ast_expr_list_t * make_expr_list(ast_expr_t * expr, ast_expr_list_t * next) {
    ast_expr_list_t * expr_list = new_data(ast_expr_list_t);
    expr_list->expr = expr;
    expr_list->next = next;
    return expr_list;
}

// Statement
ast_import_t * make_import(ast_id_t * name) {
    ast_import_t * import = new_data(ast_import_t);
    import->super.category = AST_IMPORT;
    import->name = name;
    return import;
}

ast_struct_t * make_struct(ast_var_declare_list_t * members) {
    ast_struct_t * s = new_data(ast_struct_t);
    s->super.category = AST_STRUCT;
    s->members = members;
    return s;
}

ast_let_t * make_let(ast_var_declare_t * var, ast_expr_t * expr) {
    ast_let_t * let = new_data(ast_let_t);
    let->super.category = AST_LET;
    let->var = var;
    let->expr = expr;
    return let;
}

// Expr
void expr_init(ast_expr_t * expr, ast_category_t category) {
    expr->super.category = category;
}

ast_fun_t * make_fun(ast_var_declare_list_t * params, ast_id_t * return_type, ast_expr_t * body) {
    ast_fun_t * fun = new_data(ast_fun_t);
    expr_init(&fun->super, AST_FUN);
    fun->params = params;
    fun->return_type = return_type;
    fun->body = body;
    return fun;
}

ast_block_t * make_block(ast_statement_list_t * statements, ast_expr_t * the_last) {
    ast_block_t * block = new_data(ast_block_t);
    expr_init(&block->super, AST_BLOCK);
    block->statements = statements;
    block->the_last = the_last;
    return block;
}

ast_str_t * make_str(string_t * value) {
    ast_str_t * str = new_data(ast_str_t);
    expr_init(&str->super, AST_STR);
    str->value = string_dup(value);
    return str;
}

ast_double_t * make_double(double value) {
    ast_double_t * d = new_data(ast_double_t);
    expr_init(&d->super, AST_DOUBLE);
    d->value = value;
    return d;
}

ast_long_t * make_long(long value) {
    ast_long_t * d = new_data(ast_long_t);
    expr_init(&d->super, AST_LONG);
    d->value = value;
    return d;
}

ast_fun_apply_t * make_fun_apply(ast_expr_t * function, ast_expr_list_t * operands) {
    ast_fun_apply_t * fun_apply = new_data(ast_fun_apply_t);
    expr_init(&fun_apply->super, AST_FUN_APPLY);
    fun_apply->function = function;
    fun_apply->operands = operands;
    return fun_apply;
}

ast_ref_t * make_ref(ast_id_t * name) {
    ast_ref_t * ref = new_data(ast_ref_t);
    expr_init(&ref->super, AST_REF);
    ref->name = name;
    return ref;
}

ast_struct_ref_t * make_struct_ref(ast_expr_t * base, int index, string_t * name) {
    ast_struct_ref_t * ref = new_data(ast_struct_ref_t);
    expr_init(&ref->super, AST_STRUCT_REF);
    ref->base = base;
    ref->index = index;
    ref->name = name;
    return ref;
}

// Parsers

#define BUF(buf) SBUILDER(buf, 1024)
#define BUF2STR(buf, str) string_t str; string_init(&str, buf.buf, sbuilder_len(&buf))

ast_id_t * parse_id(token_stream_t * stream) {
    token_t *token = token_stream_peek(stream);
    if (token->type != TOKEN_ID) {
        return NULL;
    }
    token_stream_poll(stream);
    return make_id(NULL, &token->value);
}

ast_id_t * parse_cid(token_stream_t * stream) {
    BUF(prefix_buf);
    BUF(base_buf);
    while (true) {
        token_t *token = token_stream_peek(stream);
        if (token->type != TOKEN_ID) {
            break;
        }
        if (sbuilder_len(&base_buf) > 0) {
            if (sbuilder_len(&prefix_buf) > 0) sbuilder_str(&prefix_buf, ".");
            sbuilder_str(&prefix_buf, base_buf.buf);
            sbuilder_reset(&base_buf);
            sbuilder_string(&base_buf, &token->value);
        }
        token_stream_poll(stream);
    }
    if (sbuilder_len(&base_buf) > 0) {
        BUF2STR(base_buf, base);
        if (sbuilder_len(&prefix_buf) > 0) {
            BUF2STR(prefix_buf, prefix);
            return make_id(&prefix, &base);
        }
        return make_id(NULL, &base);
    }
    return NULL;
}

ast_expr_t * parse_expr(token_stream_t * stream);

ast_import_t * parse_import(token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_IMPORT, stream);
    ast_id_t *name = parse_cid(stream);
    require(name != NULL, "Need cid!", stream);
    return make_import(name);
}

ast_var_declare_t * parse_var_declare(token_stream_t * stream) {
    ast_id_t * name = parse_id(stream);
    if (name == NULL) return NULL;

    ast_id_t * type = NULL;
    if (token_stream_peek(stream)->type == TOKEN_COLON) {
        token_stream_poll(stream);
        type = parse_cid(stream);
        require(type != NULL, "Need cid!", stream);
    }
    return make_var_declare(name, type);
}

ast_var_declare_list_t * parse_var_declare_list(token_stream_t * stream) {
    ast_var_declare_t * var_declare = parse_var_declare(stream);
    if (var_declare == NULL) return NULL;
    token_t * token = token_stream_peek(stream);
    if (token->type == TOKEN_COMMA) {
        token_stream_poll(stream);
        return make_var_declare_list(var_declare, parse_var_declare_list(stream));
    } else {
        return make_var_declare_list(var_declare, NULL);
    }
}

ast_struct_t * parse_struct(token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_STRUCT, stream);
    token_t * token = token_stream_poll(stream);
    require_token(token, TOKEN_ID, stream);
    require_token(token_stream_poll(stream), TOKEN_LPAREN, stream);
    ast_var_declare_list_t * members = parse_var_declare_list(stream);
    require_token(token_stream_poll(stream), TOKEN_RPAREN, stream);
    return make_struct(members);
}

ast_let_t * parse_let(token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_LET, stream);
    ast_var_declare_t * var = parse_var_declare(stream);
    require(var != NULL, "Need variable", stream);
    require_id(token_stream_poll(stream), "=", stream);
    ast_expr_t * expr = parse_expr(stream);
    require(expr != NULL, "Need expr", stream);
    return make_let(var, expr);
}


ast_statement_t * parse_statement(token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token->type == TOKEN_END) {
        return NULL;
    }
    SBUILDER(builder, 1024);

    switch (token->type) {
        case TOKEN_IMPORT:
            return &parse_import(stream)->super;
        case TOKEN_STRUCT:
            return &parse_struct(stream)->super;
        case TOKEN_LET:
            return &parse_let(stream)->super;
        default:
            break;
    }
    ast_expr_t * expr = parse_expr(stream);
    if (expr == NULL) return NULL;
    return &expr->super;
}

static bool parse_block_rec(token_stream_t * stream, ast_statement_list_t ** p_statements, ast_expr_t ** p_the_last) {
    ast_statement_t * statement = parse_statement(stream);
    if (statement == NULL) {
        return false;
    }
    ast_statement_list_t * statements;
    ast_expr_t * the_last;
    if (parse_block_rec(stream, &statements, &the_last)) {
        *p_statements = make_statement_list(statement, statements);
        *p_the_last = the_last;
        return true;
    } else {
        require(ast_is_expr(statement->category), "Need expr", stream);
        *p_statements = NULL;
        *p_the_last = container_of(statement, ast_expr_t, super);
        return true;
    }
}

ast_block_t * parse_block(token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_LPAREN, stream);
    ast_statement_list_t * statements;
    ast_expr_t * the_last;
    require(parse_block_rec(stream, &statements, &the_last), "Need block", stream);
    require_token(token_stream_poll(stream), TOKEN_RPAREN, stream);
    return make_block(statements, the_last);
}

ast_fun_t * parse_fun(token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_FUN, stream);

    require_token(token_stream_poll(stream), TOKEN_LPAREN, stream);
    ast_var_declare_list_t * params = parse_var_declare_list(stream);
    require_token(token_stream_poll(stream), TOKEN_RPAREN, stream);

    ast_id_t * return_type = NULL;
    if (token_stream_peek(stream)->type == TOKEN_COLON) {
        token_stream_poll(stream);
        return_type = parse_cid(stream);
        require(return_type != NULL, "Need cid", stream);
    }

    ast_expr_t * body = parse_expr(stream);
    require(body != NULL, "Need body", stream);
    return make_fun(params, return_type, body);
}

ast_str_t * parse_str(token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token->type != TOKEN_STRING) {
        return NULL;
    }
    token_stream_poll(stream);
    return make_str(&token->value);
}

ast_double_t * parse_double(token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token->type != TOKEN_DOUBLE) {
        return NULL;
    }
    token_stream_poll(stream);
    char str[token->value.len+1];
    memcpy(str, token->value.value, (size_t) token->value.len);
    str[token->value.len] = '\0';
    return make_double(atof(str));
}

ast_long_t * parse_long(token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token->type != TOKEN_LONG) {
        return NULL;
    }
    token_stream_poll(stream);
    char str[token->value.len+1];
    memcpy(str, token->value.value, (size_t) token->value.len);
    str[token->value.len] = '\0';
    return make_long(atol(str));
}

ast_expr_list_t * parse_expr_list(token_stream_t * stream) {
    ast_expr_t * expr = parse_expr(stream);
    if (expr == NULL) return NULL;
    token_t * token = token_stream_peek(stream);
    if (token->type == TOKEN_COMMA) {
        token_stream_poll(stream);
        return make_expr_list(expr, parse_expr_list(stream));
    } else {
        return make_expr_list(expr, NULL);
    }
}

ast_fun_apply_t * parse_fun_apply(ast_expr_t * function, token_stream_t * stream) {
    require(function != NULL, "Need function", stream);
    require_token(token_stream_poll(stream), TOKEN_LPAREN, stream);
    ast_expr_list_t * operands = parse_expr_list(stream);
    require_token(token_stream_poll(stream), TOKEN_RPAREN, stream);
    return make_fun_apply(function, operands);
}

ast_ref_t * parse_ref(token_stream_t * stream) {
    ast_id_t * name = parse_cid(stream);
    require(name != NULL, "Need cid", stream);
    return make_ref(name);
}

ast_struct_ref_t * parse_struct_ref(ast_expr_t * base, token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_PERIOD, stream);

    token_t * token = token_stream_poll(stream);
    require_token(token, TOKEN_ID, stream);

    int index = 0;

    return make_struct_ref(base, index, string_dup(&token->value));
}

ast_expr_t * parse_term(token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    switch (token->type) {
        case TOKEN_FUN:
            return &parse_fun(stream)->super;
        case TOKEN_LPAREN:
            return &parse_block(stream)->super;
        case TOKEN_STRING:
            return &parse_str(stream)->super;
        case TOKEN_DOUBLE:
            return &parse_double(stream)->super;
        case TOKEN_LONG:
            return &parse_long(stream)->super;
        case TOKEN_ID:
            return &parse_ref(stream)->super;
        default:
            return NULL;
    }
}

ast_expr_t * parse_primary(token_stream_t * stream) {
    ast_expr_t * left = NULL;
    while (true) {
        ast_expr_t * new_left = NULL;
        if (left == NULL) {
            new_left = parse_term(stream);
        } else {
            token_t * token = token_stream_peek(stream);
            switch (token->type) {
                case TOKEN_LPAREN:
                    new_left = &parse_fun_apply(left, stream)->super;
                    break;
                case TOKEN_PERIOD:
                    new_left = &parse_struct_ref(left, stream)->super;
                    break;
                default:
                    break;
            }
        }
        if (new_left == NULL) {
            return left;
        } else {
            left = new_left;
        }
    }
}

typedef struct {
    ast_expr_t * left;
    operator_def_t * op;
} frame_t;

static inline operator_def_t * parse_operator_def(operator_table_t * table, token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token->type != TOKEN_ID) {
        return NULL;
    }
    operator_def_t * def = operator_table_get(table, &token->value);
    if (def != NULL) {
        token_stream_poll(stream);
        return def;
    }
    return NULL;
}

static inline bool operator_le(operator_def_t *left, operator_def_t *right) {
    if (left->precedence != right->precedence) {
        return left->precedence <= right->precedence;
    }
    return left->left2right;
}

static inline ast_expr_t * build_unary_apply(operator_def_t * op, ast_expr_t * operand) {
    if (op == NULL) {
        return operand;
    } else {
        return &make_fun_apply(&make_ref(op->var)->super, make_expr_list(operand, NULL))->super;
    }
}

static frame_t parse_unary_rec(token_stream_t *stream, operator_def_t * last_prefix) {
    ast_expr_t *left;
    operator_def_t *postfix;
    module_t *current_module = parser_context_current_module(context);
    operator_def_t *prefix = parse_operator_def(&current_module->prefix_table, stream);
    if (prefix == NULL) {
        left = parse_primary(stream);
        if (left == NULL) {
            require(last_prefix == NULL, "Need expr", stream);
            frame_t current;
            current.left = NULL;
            current.op = NULL;
            return current;
        }
        postfix = parse_operator_def(&current_module->postfix_table, stream);
    } else {
        frame_t current = parse_unary_rec(stream, prefix);
        left = current.left;
        postfix = current.op;
    }

    while (true) {
        if (postfix == NULL || (last_prefix != NULL && operator_le(last_prefix, postfix))) {
            frame_t current;
            current.left = build_unary_apply(last_prefix, left);
            current.op = postfix;
            return current;
        } else {
            left = build_unary_apply(postfix, left);
            postfix = parse_operator_def(&current_module->postfix_table, stream);
        }
    }
}

static inline ast_expr_t * parse_unary(token_stream_t *stream) {
    frame_t ret = parse_unary_rec(stream, NULL);
    return ret.left;
}

static inline ast_expr_t * build_binary_apply(operator_def_t * op, ast_expr_t * left, ast_expr_t * right) {
    if (op == NULL) {
        ensure(left == NULL);
        return right;
    } else {
        return &make_fun_apply(&make_ref(op->var)->super,
                               make_expr_list(left, make_expr_list(right, NULL)))->super;
    }
}

static frame_t parse_binary_rec(token_stream_t *stream, ast_expr_t * last_left, operator_def_t * last_op) {
    ast_expr_t * left = parse_unary(stream);
    if (left == NULL) {
        require(last_op == NULL, "Need expr", stream);
        frame_t current;
        current.left = NULL;
        current.op = NULL;
        return current;
    }
    operator_def_t * op = parse_operator_def(&parser_context_current_module(context)->binary_table, stream);

    while (true) {
        if (op == NULL || (last_op != NULL && operator_le(last_op, op))) {
            frame_t current;
            current.left = build_binary_apply(last_op, last_left, left);
            current.op = op;
            return current;
        } else {
            frame_t current = parse_binary_rec(stream, left, op);
            left = current.left;
            op = current.op;
        }
    }
}

static inline ast_expr_t * parse_binary(token_stream_t *stream) {
    frame_t ret = parse_binary_rec(stream, NULL, NULL);
    return ret.left;
}

ast_expr_t * parse_expr(token_stream_t * stream) {
    return parse_binary(stream);
}

ast_statement_t * parser_parse(parser_t * parser, token_stream_t * stream) {
    return parse_statement(stream);
}
