/**
 * @author goldolphin
 *         2015-08-28 16:35:35.
 */

#include <lexer/token.h>
#include <parser/module/module.h>
#include "parser.h"
#include "utils.h"

#define SYMBOL_TABLE_INITIAL_CAPACITY 4096

// Utilities

static inline var_def_t * parser_get_var(const char * func_name, parser_context_t * context, string_t * name, token_stream_t * stream) {
    var_def_t *def = module_get_var(parser_context_current_module(context), name);
    if (def == NULL) {
        SBUILDER(builder, 1024);
        sbuilder_str(&builder, "Undefined name: ");
        sbuilder_string(&builder, name);
        parser_require(func_name, false, builder.buf, stream);
    }
    return def;
}

static inline void parser_define_var(const char * func_name, parser_context_t * context, string_t * name, token_stream_t * stream) {
    if (!module_define_var(parser_context_current_module(context), name, NULL)) {
        SBUILDER(builder, 1024);
        sbuilder_str(&builder, "Duplicate definition of name: ");
        sbuilder_string(&builder, name);
        parser_require(func_name, false, builder.buf, stream);
    }
}
#define get_var(context, name, stream) parser_get_var(__FUNCTION__, context, name, stream)

#define define_var(context, name, stream) parser_define_var(__FUNCTION__, context, name, stream)

void parser_init(parser_t * parser) {
}

void parser_destroy(parser_t * parser) {
}

// Basic
ast_var_declare_t * make_var_declare(string_t * name) {
    ast_var_declare_t * var_declare = new_data(ast_var_declare_t);
    var_declare->name = name;
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
ast_import_t * make_import(string_t * module_name) {
    ast_import_t * import = new_data(ast_import_t);
    import->super.category = AST_IMPORT;
    import->module_name = module_name;
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

ast_fun_t * make_fun(int param_num, ast_var_declare_list_t * params, ast_expr_t * body) {
    ast_fun_t * fun = new_data(ast_fun_t);
    expr_init(&fun->super, AST_FUN);
    fun->param_num = param_num;
    fun->params = params;
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

ast_ref_t * make_ref(int level, int index, string_t * name) {
    ast_ref_t * ref = new_data(ast_ref_t);
    expr_init(&ref->super, AST_REF);
    ref->level = level;
    ref->index = index;
    ref->name = string_dup(name);
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

bool parse_id(token_stream_t * stream, sbuilder_t * builder) {
    token_t * token = token_stream_peek(stream);
    if (token->type != TOKEN_ID) {
        return false;
    }
    sbuilder_string(builder, &token->value);
    token_stream_poll(stream);
    return true;
}

bool parse_cid(token_stream_t * stream, sbuilder_t * builder) {
    if (!parse_id(stream, builder)) return false;
    token_t * token = token_stream_peek(stream);
    if (token->type == TOKEN_PERIOD) {
        token_stream_poll(stream);
        return parse_cid(stream, builder);
    } else {
        return true;
    }
}

#define BUF(buf) SBUILDER(buf, 1024)
#define BUF2STR(buf, str) string_t str; string_init(&str, (uint8_t *)buf.buf, (int)sbuilder_len(&buf))

ast_expr_t * parse_expr(parser_context_t * context, token_stream_t * stream);

ast_import_t * parse_import(parser_context_t * context, token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_IMPORT, stream);
    BUF(buf);
    require(parse_cid(stream, &buf), "Need cid!", stream);
    BUF2STR(buf, name);
    module_t *to_import = parser_context_get_module(context, &name);
    module_t *current = parser_context_current_module(context);
    module_import_module(current, to_import);
    return make_import(string_dup(&name));
}

type_t * parse_type(token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token->type != TOKEN_ID) {
        return NULL;
    }
    token_stream_poll(stream);
    return NULL;
}

ast_var_declare_t * parse_var_declare(token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token->type != TOKEN_ID) return NULL;
    token_stream_poll(stream);
    string_t * name = string_dup(&token->value);

    token = token_stream_peek(stream);
    if (token->type == TOKEN_COLON) {
        token_stream_poll(stream);
        parse_type(stream);
    }
    return make_var_declare(name);
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

ast_struct_t * parse_struct(parser_context_t * context, token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_STRUCT, stream);

    token_t * token = token_stream_poll(stream);
    require_token(token, TOKEN_ID, stream);
    string_t * name = string_dup(&token->value);

    require_token(token_stream_poll(stream), TOKEN_LPAREN, stream);
    ast_var_declare_list_t * members = parse_var_declare_list(stream);
    require_token(token_stream_poll(stream), TOKEN_RPAREN, stream);

    define_var(context, name, stream);
    define_var(context, string_concat2(name, STRING("Null")), stream);

    for (ast_var_declare_list_t * m = members; m != NULL; m = m->next) {
        define_var(context, string_concat3(name, STRING("_get_"), m->var->name), stream);
        define_var(context, string_concat3(name, STRING("_set_"), m->var->name), stream);
    }
    return make_struct(members);
}

ast_let_t * parse_let(parser_context_t * context, token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_LET, stream);

    ast_var_declare_t * var = parse_var_declare(stream);
    require(var != NULL, "Need variable", stream);

    require_id(token_stream_poll(stream), "=", stream);
    define_var(context, var->name, stream);

    ast_expr_t * expr = parse_expr(context, stream);
    require(expr != NULL, "Need expr", stream);
    return make_let(var, expr);
}


ast_statement_t * parse_statement(parser_context_t * context, token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token->type == TOKEN_END) {
        return NULL;
    }
    SBUILDER(builder, 1024);

    switch (token->type) {
        case TOKEN_IMPORT:
            return &parse_import(context, stream)->super;
        case TOKEN_STRUCT:
            return &parse_struct(context, stream)->super;
        case TOKEN_LET:
            return &parse_let(context, stream)->super;
        default:
            break;
    }
    ast_expr_t * expr = parse_expr(context, stream);
    if (expr == NULL) return NULL;
    return &expr->super;
}

ast_fun_t * parse_fun(parser_context_t * context, token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_FUN, stream);

    require_token(token_stream_poll(stream), TOKEN_LPAREN, stream);
    ast_var_declare_list_t * params = parse_var_declare_list(stream);
    require_token(token_stream_poll(stream), TOKEN_RPAREN, stream);

    parser_context_enter_scope(context);
    // Add parameter into scope.
    int param_num = 0;
    for (ast_var_declare_list_t * head = params; head != NULL; head = head->next) {
        define_var(context, head->var->name, stream);
        ++ param_num;
    }

    if (token_stream_peek(stream)->type == TOKEN_COLON) {
        token_stream_poll(stream);
        parse_type(stream);
    }

    ast_expr_t * body = parse_expr(context, stream);
    parser_context_exit_scope(context);

    require(body != NULL, "Need body", stream);
    return make_fun(param_num, params, body);
}

static bool parse_block_rec(parser_context_t * context, token_stream_t * stream, ast_statement_list_t ** p_statements, ast_expr_t ** p_the_last) {
    ast_statement_t * statement = parse_statement(context, stream);
    if (statement == NULL) {
        return false;
    }
    ast_statement_list_t * statements;
    ast_expr_t * the_last;
    if (parse_block_rec(context, stream, &statements, &the_last)) {
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

ast_block_t * parse_block(parser_context_t * context, token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_LPAREN, stream);
    ast_statement_list_t * statements;
    ast_expr_t * the_last;
    require(parse_block_rec(context, stream, &statements, &the_last), "Need block", stream);
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

ast_expr_list_t * parse_expr_list(parser_context_t * context, token_stream_t * stream) {
    ast_expr_t * expr = parse_expr(context, stream);
    if (expr == NULL) return NULL;
    token_t * token = token_stream_peek(stream);
    if (token->type == TOKEN_COMMA) {
        token_stream_poll(stream);
        return make_expr_list(expr, parse_expr_list(context, stream));
    } else {
        return make_expr_list(expr, NULL);
    }
}

ast_fun_apply_t * parse_fun_apply(parser_context_t * context, ast_expr_t * function, token_stream_t * stream) {
    require(function != NULL, "Need function", stream);
    require_token(token_stream_poll(stream), TOKEN_LPAREN, stream);
    ast_expr_list_t * operands = parse_expr_list(context, stream);
    require_token(token_stream_poll(stream), TOKEN_RPAREN, stream);
    return make_fun_apply(function, operands);
}

ast_ref_t * parse_ref(parser_context_t * context, token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token->type != TOKEN_ID) {
        return NULL;
    }
    var_def_t *def = get_var(context, &token->value, stream);
    token_stream_poll(stream);
    return make_ref(def->super.level, def->super.index, def->super.name);
}

ast_struct_ref_t * parse_struct_ref(ast_expr_t * base, token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_PERIOD, stream);

    token_t * token = token_stream_poll(stream);
    require_token(token, TOKEN_ID, stream);

    int index = 0;

    return make_struct_ref(base, index, string_dup(&token->value));
}

ast_expr_t * parse_term(parser_context_t * context, token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    switch (token->type) {
        case TOKEN_FUN:
            return &parse_fun(context, stream)->super;
        case TOKEN_LPAREN:
            return &parse_block(context, stream)->super;
        case TOKEN_STRING:
            return &parse_str(stream)->super;
        case TOKEN_DOUBLE:
            return &parse_double(stream)->super;
        case TOKEN_LONG:
            return &parse_long(stream)->super;
        case TOKEN_ID:
            return &parse_ref(context, stream)->super;
        default:
            return NULL;
    }
}

ast_expr_t * parse_primary(parser_context_t * context, token_stream_t * stream) {
    ast_expr_t * left = NULL;
    while (true) {
        ast_expr_t * new_left = NULL;
        if (left == NULL) {
            new_left = parse_term(context, stream);
        } else {
            token_t * token = token_stream_peek(stream);
            switch (token->type) {
                case TOKEN_LPAREN:
                    new_left = &parse_fun_apply(context, left, stream)->super;
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
        return &make_fun_apply(&make_ref(op->var->super.level, op->var->super.index, op->var->super.name)->super, make_expr_list(operand, NULL))->super;
    }
}

static frame_t parse_unary_rec(parser_context_t * context, token_stream_t *stream, operator_def_t * last_prefix) {
    ast_expr_t *left;
    operator_def_t *postfix;
    module_t *current_module = parser_context_current_module(context);
    operator_def_t *prefix = parse_operator_def(&current_module->prefix_table, stream);
    if (prefix == NULL) {
        left = parse_primary(context, stream);
        if (left == NULL) {
            require(last_prefix == NULL, "Need expr", stream);
            frame_t current;
            current.left = NULL;
            current.op = NULL;
            return current;
        }
        postfix = parse_operator_def(&current_module->postfix_table, stream);
    } else {
        frame_t current = parse_unary_rec(context, stream, prefix);
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

static inline ast_expr_t * parse_unary(parser_context_t * context, token_stream_t *stream) {
    frame_t ret = parse_unary_rec(context, stream, NULL);
    return ret.left;
}

static inline ast_expr_t * build_binary_apply(operator_def_t * op, ast_expr_t * left, ast_expr_t * right) {
    if (op == NULL) {
        ensure(left == NULL);
        return right;
    } else {
        return &make_fun_apply(&make_ref(op->var->super.level, op->var->super.index, op->var->super.name)->super,
                               make_expr_list(left, make_expr_list(right, NULL)))->super;
    }
}

static frame_t parse_binary_rec(parser_context_t * context, token_stream_t *stream, ast_expr_t * last_left, operator_def_t * last_op) {
    ast_expr_t * left = parse_unary(context, stream);
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
            frame_t current = parse_binary_rec(context, stream, left, op);
            left = current.left;
            op = current.op;
        }
    }
}

static inline ast_expr_t * parse_binary(parser_context_t * context, token_stream_t *stream) {
    frame_t ret = parse_binary_rec(context, stream, NULL, NULL);
    return ret.left;
}

ast_expr_t * parse_expr(parser_context_t * context, token_stream_t * stream) {
    return parse_binary(context, stream);
}

ast_statement_t * parser_parse(parser_t * parser, parser_context_t * context, token_stream_t * stream) {
    return parse_statement(context, stream);
}