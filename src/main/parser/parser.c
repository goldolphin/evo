/**
 * @author goldolphin
 *         2015-08-28 16:35:35.
 */

#include <lexer/token.h>
#include "parser.h"
#include "utils.h"

// Basic
ast_id_t * make_id(string_t * value) {
    ast_id_t * id = new_data(ast_id_t);
    id->name = string_dup(value);
    return id;
}

ast_cid_t * make_cid(ast_id_t * id, ast_cid_t * parent) {
    ast_cid_t * cid = new_data(ast_cid_t);
    cid->id = id;
    cid->parent = parent;
    return cid;
}

ast_var_declare_t * make_var_declare(ast_id_t * id, ast_cid_t * type) {
    ast_var_declare_t * var_declare = new_data(ast_var_declare_t);
    var_declare->id = id;
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
ast_import_t * make_import(ast_cid_t * module) {
    ast_import_t * import = new_data(ast_import_t);
    import->super.type = AST_IMPORT;
    import->module = module;
    return import;
}

ast_struct_t * make_struct(ast_id_t * id, ast_var_declare_list_t * members, ast_cid_t * parent) {
    ast_struct_t * s = new_data(ast_struct_t);
    s->super.type = AST_STRUCT;
    s->id = id;
    s->members = members;
    s->parent = parent;
    return s;
}

ast_let_t * make_let(ast_var_declare_t * var, ast_expr_t * expr) {
    ast_let_t * let = new_data(ast_let_t);
    let->super.type = AST_LET;
    let->var = var;
    let->expr = expr;
    return let;
}

// Expr
ast_fun_t * make_fun(ast_var_declare_list_t * params, ast_cid_t * return_type, ast_expr_t * body) {
    ast_fun_t * fun = new_data(ast_fun_t);
    fun->super.super.type = AST_FUN;
    fun->params = params;
    fun->return_type = return_type;
    fun->body = body;
    return fun;
}

ast_block_t * make_block(ast_statement_list_t * statements, ast_expr_t * the_last) {
    ast_block_t * block = new_data(ast_block_t);
    block->super.super.type = AST_BLOCK;
    block->statements = statements;
    block->the_last = the_last;
    return block;
}

ast_str_t * make_str(string_t * value) {
    ast_str_t * str = new_data(ast_str_t);
    str->super.super.type = AST_STR;
    str->value = string_dup(value);
    return str;
}

ast_double_t * make_double(double value) {
    ast_double_t * d = new_data(ast_double_t);
    d->super.super.type = AST_DOUBLE;
    d->value = value;
    return d;
}

ast_long_t * make_long(long value) {
    ast_long_t * d = new_data(ast_long_t);
    d->super.super.type = AST_LONG;
    d->value = value;
    return d;
}

ast_fun_apply_t * make_fun_apply(ast_expr_t * function, ast_expr_list_t * operands) {
    ast_fun_apply_t * fun_apply = new_data(ast_fun_apply_t);
    fun_apply->super.super.type = AST_FUN_APPLY;
    fun_apply->function = function;
    fun_apply->operands = operands;
    return fun_apply;
}

ast_ref_t * make_ref(ast_expr_t * base, ast_cid_t * cid) {
    ast_ref_t * ref = new_data(ast_ref_t);
    ref->super.super.type = AST_REF;
    ref->base = base;
    ref->cid = cid;
    return ref;
}

// Parsers

ast_id_t * parse_id(token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token->type != TOKEN_ID) {
        return NULL;
    }
    token_stream_poll(stream);
    return make_id(&token->value);
}

ast_cid_t * parse_cid(token_stream_t * stream) {
    ast_id_t * id = parse_id(stream);
    if (id == NULL) return NULL;
    token_t * token = token_stream_peek(stream);
    if (token->type == TOKEN_PERIOD) {
        token_stream_poll(stream);
        return make_cid(id, parse_cid(stream));
    } else {
        return make_cid(id, NULL);
    }
}

ast_import_t * parse_import(token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_IMPORT, stream);

    ast_cid_t * module = parse_cid(stream);
    require(module != NULL, "Need cid!", stream);
    return make_import(module);
}

ast_var_declare_t * parse_var_declare(token_stream_t * stream) {
    ast_id_t * id = parse_id(stream);
    if (id == NULL) return NULL;
    token_t * token = token_stream_peek(stream);
    ast_cid_t * type = NULL;
    if (token->type == TOKEN_COLON) {
        token_stream_poll(stream);
        type = parse_cid(stream);
        require(type != NULL, "Need cid!", stream);
    }
    return make_var_declare(id, type);
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

    ast_id_t * id = parse_id(stream);
    require(id != NULL, "Need id!", stream);

    require_token(token_stream_poll(stream), TOKEN_LPAREN, stream);
    ast_var_declare_list_t * members = parse_var_declare_list(stream);
    require_token(token_stream_poll(stream), TOKEN_RPAREN, stream);

    ast_cid_t * parent = NULL;
    if (token_stream_peek(stream)->type == TOKEN_EXTENDS) {
        token_stream_poll(stream);
        parent = parse_cid(stream);
    }

    return make_struct(id, members, parent);
}

ast_let_t * parse_let(parser_t * parser, token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_LET, stream);

    ast_var_declare_t * var = parse_var_declare(stream);
    require(var != NULL, "Need id", stream);

    require_id(token_stream_poll(stream), "=", stream);

    ast_expr_t * expr = parse_expr(parser, stream);
    require(expr != NULL, "Need expr", stream);
    return make_let(var, expr);
}


ast_statement_t * parse_statement(parser_t * parser, token_stream_t * stream) {
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
            return &parse_let(parser, stream)->super;
        default:
            break;
    }
    ast_expr_t * expr = parse_expr(parser, stream);
    if (expr == NULL) return NULL;
    return &expr->super;
}

ast_fun_t * parse_fun(parser_t * parser, token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_FUN, stream);

    require_token(token_stream_poll(stream), TOKEN_LPAREN, stream);
    ast_var_declare_list_t * params = parse_var_declare_list(stream);
    require_token(token_stream_poll(stream), TOKEN_RPAREN, stream);

    ast_cid_t * return_type = NULL;
    if (token_stream_peek(stream)->type == TOKEN_COLON) {
        token_stream_poll(stream);
        return_type = parse_cid(stream);
    }

    ast_expr_t * body = parse_expr(parser, stream);
    require(body != NULL, "Need body", stream);

    return make_fun(params, return_type, body);
}

static bool parse_block_rec(parser_t * parser, token_stream_t * stream, ast_statement_list_t ** p_statements, ast_expr_t ** p_the_last) {
    ast_statement_t * statement = parse_statement(parser, stream);
    if (statement == NULL) {
        return false;
    }
    ast_statement_list_t * statements;
    ast_expr_t * the_last;
    if (parse_block_rec(parser, stream, &statements, &the_last)) {
        *p_statements = make_statement_list(statement, statements);
        *p_the_last = the_last;
        return true;
    } else {
        require(ast_is_expr(statement->type), "Need expr", stream);
        *p_statements = NULL;
        *p_the_last = container_of(statement, ast_expr_t, super);
        return true;
    }
}

ast_block_t * parse_block(parser_t * parser, token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_LPAREN, stream);
    ast_statement_list_t * statements;
    ast_expr_t * the_last;
    require(parse_block_rec(parser, stream, &statements, &the_last), "Need block", stream);
    require_token(token_stream_poll(stream), TOKEN_RPAREN, stream);
    return make_block(statements, the_last);
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

ast_expr_list_t * parse_expr_list(parser_t * parser, token_stream_t * stream) {
    ast_expr_t * expr = parse_expr(parser, stream);
    if (expr == NULL) return NULL;
    token_t * token = token_stream_peek(stream);
    if (token->type == TOKEN_COMMA) {
        token_stream_poll(stream);
        return make_expr_list(expr, parse_expr_list(parser, stream));
    } else {
        return make_expr_list(expr, NULL);
    }
}

ast_fun_apply_t * parse_fun_apply(parser_t * parser, ast_expr_t * function, token_stream_t * stream) {
    require(function != NULL, "Need function", stream);
    require_token(token_stream_poll(stream), TOKEN_LPAREN, stream);
    ast_expr_list_t * operands = parse_expr_list(parser, stream);
    require_token(token_stream_poll(stream), TOKEN_RPAREN, stream);
    return make_fun_apply(function, operands);
}

ast_ref_t * parse_ref(ast_expr_t * base, token_stream_t * stream) {
    if (base != NULL) {
        require_token(token_stream_poll(stream), TOKEN_PERIOD, stream);
    }
    ast_cid_t * cid = parse_cid(stream);
    if (cid == NULL) {
        require(base == NULL, "Need cid", stream);
        return NULL;
    }
    return make_ref(base, cid);
}

ast_expr_t * parse_term(parser_t * parser, token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    switch (token->type) {
        case TOKEN_FUN:
            return &parse_fun(parser, stream)->super;
        case TOKEN_LPAREN:
            return &parse_block(parser, stream)->super;
        case TOKEN_STRING:
            return &parse_str(stream)->super;
        case TOKEN_DOUBLE:
            return &parse_double(stream)->super;
        case TOKEN_LONG:
            return &parse_long(stream)->super;
        case TOKEN_ID:
            return &parse_ref(NULL, stream)->super;
        default:
            return NULL;
    }
}

ast_expr_t * parse_primary(parser_t * parser, token_stream_t * stream) {
    ast_expr_t * left = NULL;
    while (true) {
        ast_expr_t * new_left = NULL;
        if (left == NULL) {
            new_left = parse_term(parser, stream);
        } else {
            token_t * token = token_stream_peek(stream);
            switch (token->type) {
                case TOKEN_LPAREN:
                    new_left = &parse_fun_apply(parser, left, stream)->super;
                    break;
                case TOKEN_PERIOD:
                    new_left = &parse_ref(left, stream)->super;
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
        return &make_fun_apply(&make_ref(NULL, make_cid(make_id(&op->name), NULL))->super, make_expr_list(operand, NULL))->super;
    }
}

static frame_t parse_unary_rec(parser_t *parser, token_stream_t *stream, operator_def_t * last_prefix) {
    ast_expr_t *left;
    operator_def_t *postfix;

    operator_def_t *prefix = parse_operator_def(&parser->prefix_table, stream);
    if (prefix == NULL) {
        left = parse_primary(parser, stream);
        if (left == NULL) {
            require(last_prefix == NULL, "Need expr", stream);
            frame_t current;
            current.left = NULL;
            current.op = NULL;
            return current;
        }
        postfix = parse_operator_def(&parser->postfix_table, stream);
    } else {
        frame_t current = parse_unary_rec(parser, stream, prefix);
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
            postfix = parse_operator_def(&parser->postfix_table, stream);
        }
    }
}

static inline ast_expr_t * parse_unary(parser_t *parser, token_stream_t *stream) {
    frame_t ret = parse_unary_rec(parser, stream, NULL);
    return ret.left;
}

static inline ast_expr_t * build_binary_apply(operator_def_t * op, ast_expr_t * left, ast_expr_t * right) {
    if (op == NULL) {
        ensure(left == NULL);
        return right;
    } else {
        return &make_fun_apply(&make_ref(NULL, make_cid(make_id(&op->name), NULL))->super,
                               make_expr_list(left, make_expr_list(right, NULL)))->super;
    }
}

static frame_t parse_binary_rec(parser_t *parser, token_stream_t *stream, ast_expr_t * last_left, operator_def_t * last_op) {
    ast_expr_t * left = parse_unary(parser, stream);
    if (left == NULL) {
        require(last_op == NULL, "Need expr", stream);
        frame_t current;
        current.left = NULL;
        current.op = NULL;
        return current;
    }
    operator_def_t * op = parse_operator_def(&parser->binary_table, stream);

    while (true) {
        if (op == NULL || (last_op != NULL && operator_le(last_op, op))) {
            frame_t current;
            current.left = build_binary_apply(last_op, last_left, left);
            current.op = op;
            return current;
        } else {
            frame_t current = parse_binary_rec(parser, stream, left, op);
            left = current.left;
            op = current.op;
        }
    }
}

static inline ast_expr_t * parse_binary(parser_t *parser, token_stream_t *stream) {
    frame_t ret = parse_binary_rec(parser, stream, NULL, NULL);
    return ret.left;
}

ast_expr_t * parse_expr(parser_t * parser, token_stream_t * stream) {
    return parse_binary(parser, stream);
}

void parser_add_prefix(parser_t * parser, const char * name, int precedence) {
    string_t s;
    string_init(&s, (uint8_t *) name, (int) strlen(name));
    operator_table_add(&parser->prefix_table, &s, true, precedence);
}

void parser_add_postfix(parser_t * parser, const char * name, int precedence) {
    string_t s;
    string_init(&s, (uint8_t *) name, (int) strlen(name));
    operator_table_add(&parser->postfix_table, &s, false, precedence);
}

void parser_add_binary(parser_t * parser, const char * name, bool left2right, int precedence) {
    string_t s;
    string_init(&s, (uint8_t *) name, (int) strlen(name));
    operator_table_add(&parser->binary_table, &s, left2right, precedence);
}

void parser_init(parser_t * parser) {
    operator_table_init(&parser->prefix_table);
    operator_table_init(&parser->postfix_table);
    operator_table_init(&parser->binary_table);

    parser_add_binary(parser, "!=", true, 5);
    parser_add_binary(parser, "==", true, 5);
    parser_add_binary(parser, "||", true, 5);
    parser_add_binary(parser, "<", true, 5);
    parser_add_binary(parser, ">=", true, 5);
    parser_add_binary(parser, "%", true, 5);
    parser_add_binary(parser, "+", true, 5);
    parser_add_binary(parser, "-", true, 5);
    parser_add_binary(parser, "*", true, 4);
    parser_add_prefix(parser, "-", 2);
}

void parser_destroy(parser_t * parser) {
    operator_table_destroy(&parser->prefix_table);
    operator_table_destroy(&parser->postfix_table);
    operator_table_destroy(&parser->binary_table);
}
