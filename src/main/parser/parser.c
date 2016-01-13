/**
 * @author goldolphin
 *         2015-08-28 16:35:35.
 */

#include <lexer/token.h>
#include "parser.h"
#include "utils.h"
#include "operator_table.h"
#include "var_table.h"
#include "ast.h"
#include "type/type.h"
#include "type_table.h"
#include "symbol_table.h"
#include "type/struct.h"

#define SYMBOL_TABLE_INITIAL_CAPACITY 4096

// Utilities

static inline var_def_t * parser_get_var(const char * func_name, parser_t * parser, string_t * name, token_stream_t * stream) {
    var_def_t *def = var_table_get(&parser->var_table, name);
    if (def == NULL) {
        SBUILDER(builder, 1024);
        sbuilder_str(&builder, "Undefined name: ");
        sbuilder_string(&builder, name);
        parser_require(func_name, false, builder.buf, stream);
    }
    return def;
}

static inline void parser_define_var(const char * func_name, parser_t * parser, string_t * name, type_t * type, token_stream_t * stream) {
    if (!var_table_add(&parser->var_table, name, type)) {
        SBUILDER(builder, 1024);
        sbuilder_str(&builder, "Duplicate definition of name: ");
        sbuilder_string(&builder, name);
        parser_require(func_name, false, builder.buf, stream);
    }
}
#define get_var(parser, name, stream) parser_get_var(__FUNCTION__, parser, name, stream)

#define define_var(parser, name, type, stream) parser_define_var(__FUNCTION__, parser, name, type, stream)

static inline type_def_t * parser_get_type(const char * func_name, parser_t * parser, string_t * name, token_stream_t * stream) {
    type_def_t *type = type_table_get(&parser->type_table, name);
    if (type == NULL) {
        SBUILDER(builder, 1024);
        sbuilder_str(&builder, "Undefined type: ");
        sbuilder_string(&builder, name);
        parser_require(func_name, false, builder.buf, stream);
    }
    return type;
}

static inline void parser_declare_type(const char * func_name, parser_t * parser, string_t * name, token_stream_t * stream) {
    type_table_declare(&parser->type_table, name);
}

static inline void parser_define_type(const char * func_name, parser_t * parser, type_t * type, token_stream_t * stream) {
    if (!type_table_define(&parser->type_table, type->name, type)) {
        SBUILDER(builder, 1024);
        sbuilder_str(&builder, "Duplicate definition of type: ");
        sbuilder_string(&builder, type->name);
        parser_require(func_name, false, builder.buf, stream);
    }
}

#define get_type(parser, name, stream) parser_get_type(__FUNCTION__, parser, name, stream)

#define declare_type(parser, name, stream) parser_declare_type(__FUNCTION__, parser, name, stream)

#define define_type(parser, type, stream) parser_define_type(__FUNCTION__, parser, type, stream)

static inline void add_op(parser_t * parser, operator_table_t * table, const char * op, int precedence, const char * var) {
    string_t var_name;
    string_init(&var_name, (uint8_t *) var, (int) strlen(var));
    var_def_t * var_def = get_var(parser, &var_name, NULL);

    string_t op_name;
    string_init(&op_name, (uint8_t *) op, (int) strlen(op));
    operator_table_add(table, &op_name, true, precedence, var_def);
}

void parser_add_prefix(parser_t * parser, const char * op, int precedence, const char * var) {
    add_op(parser, &parser->prefix_table, op, precedence, var);
}

void parser_add_postfix(parser_t * parser, const char * op, int precedence, const char * var) {
    add_op(parser, &parser->postfix_table, op, precedence, var);
}

void parser_add_binary(parser_t * parser, const char * op, bool left2right, int precedence, const char * var) {
    add_op(parser, &parser->binary_table, op, precedence, var);
}

void parser_add_var(parser_t * parser, const char * var, type_t * type) {
    string_t var_name;
    string_init(&var_name, (uint8_t *) var, (int) strlen(var));
    define_var(parser, &var_name, type, NULL);
}

void parser_add_type(parser_t * parser, type_t * type) {
    define_type(parser, type, NULL);
}

static inline void parser_enter_scope(parser_t * parser) {
    var_table_enter(&parser->var_table);
    operator_table_enter(&parser->prefix_table);
    operator_table_enter(&parser->postfix_table);
    operator_table_enter(&parser->binary_table);
}

static inline void parser_exit_scope(parser_t * parser) {
    var_table_exit(&parser->var_table);
    operator_table_exit(&parser->prefix_table);
    operator_table_exit(&parser->postfix_table);
    operator_table_exit(&parser->binary_table);
}

void parser_init(parser_t * parser) {
    var_table_init(&parser->var_table, SYMBOL_TABLE_INITIAL_CAPACITY);
    type_table_init(&parser->type_table, SYMBOL_TABLE_INITIAL_CAPACITY);
    operator_table_init(&parser->prefix_table, SYMBOL_TABLE_INITIAL_CAPACITY);
    operator_table_init(&parser->postfix_table, SYMBOL_TABLE_INITIAL_CAPACITY);
    operator_table_init(&parser->binary_table, SYMBOL_TABLE_INITIAL_CAPACITY);

    parser_add_var(parser, "__ne", FUN_TYPE);
    parser_add_binary(parser, "!=", true, 5, "__ne");

    parser_add_var(parser, "__eq", FUN_TYPE);
    parser_add_binary(parser, "==", true, 5, "__eq");

    parser_add_var(parser, "__or", FUN_TYPE);
    parser_add_binary(parser, "||", true, 5, "__or");

    parser_add_var(parser, "__lt", FUN_TYPE);
    parser_add_binary(parser, "<", true, 5, "__lt");

    parser_add_var(parser, "__ge", FUN_TYPE);
    parser_add_binary(parser, ">=", true, 5, "__ge");

    parser_add_var(parser, "__re", FUN_TYPE);
    parser_add_binary(parser, "%", true, 5, "__re");

    parser_add_var(parser, "__add", FUN_TYPE);
    parser_add_binary(parser, "+", true, 5, "__add");

    parser_add_var(parser, "__sub", FUN_TYPE);
    parser_add_binary(parser, "-", true, 5, "__sub");

    parser_add_var(parser, "__mul", FUN_TYPE);
    parser_add_binary(parser, "*", true, 4, "__mul");

    parser_add_var(parser, "__neg", FUN_TYPE);
    parser_add_prefix(parser, "-", 2, "__neg");

    parser_add_var(parser, "printf", FUN_TYPE);
    parser_add_var(parser, "printfn", FUN_TYPE);
    parser_add_var(parser, "if", FUN_TYPE);
    parser_add_var(parser, "rand", FUN_TYPE);

    parser_add_type(parser, UNIT_TYPE);
    parser_add_type(parser, BOOL_TYPE);
    parser_add_type(parser, INT_TYPE);
    parser_add_type(parser, LONG_TYPE);
    parser_add_type(parser, DOUBLE_TYPE);
    parser_add_type(parser, FUN_TYPE);
    parser_add_type(parser, STRING_TYPE);
}

void parser_destroy(parser_t * parser) {
    var_table_destroy(&parser->var_table);
    type_table_destroy(&parser->type_table);
    operator_table_destroy(&parser->prefix_table);
    operator_table_destroy(&parser->postfix_table);
    operator_table_destroy(&parser->binary_table);
}

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

ast_var_declare_t * make_var_declare(string_t * name, type_def_t * type_def) {
    ast_var_declare_t * var_declare = new_data(ast_var_declare_t);
    var_declare->name = name;
    var_declare->type.index = type_def->super.index;
    var_declare->type.name = type_def->super.name;
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
    import->super.category = AST_IMPORT;
    import->module = module;
    return import;
}

type_struct_t * make_type_struct(string_t * name, ast_var_declare_list_t * members, type_struct_t * parent) {
    type_struct_t * s = new_data(type_struct_t);
    s->super.category = TC_STRUCT;
    s->super.name = string_dup(name);
    s->members = members;
    s->parent = parent;
    return s;
}

ast_struct_t * make_struct(type_struct_t * type) {
    ast_struct_t * s = new_data(ast_struct_t);
    s->super.category = AST_STRUCT;
    s->type = type;
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
void expr_init(ast_expr_t * expr, ast_category_t category, ast_type_t * type) {
    expr->super.category = category;
    expr->type.index = type->index;
    expr->type.name = type->name;
}

void expr_init1(ast_expr_t * expr, ast_category_t category, type_def_t * type_def) {
    expr->super.category = category;
    expr->type.index = type_def->super.index;
    expr->type.name = type_def->super.name;
}

ast_fun_t * make_fun(parser_t * parser, int param_num, ast_var_declare_list_t * params, ast_cid_t * return_type, ast_expr_t * body) {
    ast_fun_t * fun = new_data(ast_fun_t);

    expr_init1(&fun->super, AST_FUN, get_type(parser, FUN_TYPE->name, NULL));
    fun->param_num = param_num;
    fun->params = params;
    fun->return_type = return_type;
    fun->body = body;
    return fun;
}

ast_block_t * make_block(ast_statement_list_t * statements, ast_expr_t * the_last) {
    ast_block_t * block = new_data(ast_block_t);
    expr_init(&block->super, AST_BLOCK, &the_last->type);
    block->statements = statements;
    block->the_last = the_last;
    return block;
}

ast_str_t * make_str(parser_t * parser, string_t * value) {
    ast_str_t * str = new_data(ast_str_t);
    expr_init1(&str->super, AST_STR, get_type(parser, STRING_TYPE->name, NULL));
    str->value = string_dup(value);
    return str;
}

ast_double_t * make_double(parser_t * parser, double value) {
    ast_double_t * d = new_data(ast_double_t);
    expr_init1(&d->super, AST_DOUBLE, get_type(parser, DOUBLE_TYPE->name, NULL));
    d->value = value;
    return d;
}

ast_long_t * make_long(parser_t * parser, long value) {
    ast_long_t * d = new_data(ast_long_t);
    expr_init1(&d->super, AST_LONG, get_type(parser, LONG_TYPE->name, NULL));
    d->value = value;
    return d;
}

ast_fun_apply_t * make_fun_apply(ast_expr_t * function, ast_expr_list_t * operands) {
    ast_fun_apply_t * fun_apply = new_data(ast_fun_apply_t);
    expr_init(&fun_apply->super, AST_FUN_APPLY, NULL);
    fun_apply->function = function;
    fun_apply->operands = operands;
    return fun_apply;
}

ast_ref_t * make_ref(int level, int index, string_t * name, type_t * type) {
    ast_ref_t * ref = new_data(ast_ref_t);
    expr_init(&ref->super, AST_REF, type);
    ref->level = level;
    ref->index = index;
    ref->name = string_dup(name);
    return ref;
}

ast_struct_ref_t * make_struct_ref(ast_expr_t * base, int index, string_t * name, type_t * type) {
    ast_struct_ref_t * ref = new_data(ast_struct_ref_t);
    expr_init(&ref->super, AST_STRUCT_REF, type);
    ref->base = base;
    ref->index = index;
    ref->name = name;
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

type_t * parse_type(parser_t * parser, token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token->type != TOKEN_ID) {
        return NULL;
    }
    type_t * type = get_type(parser, &token->value, stream);
    token_stream_poll(stream);
    return type;
}

var_declare_t * parse_var_declare(parser_t * parser, token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token->type != TOKEN_ID) return NULL;
    token_stream_poll(stream);
    string_t * name = string_dup(&token->value);

    type_t * type = NULL;
    token = token_stream_peek(stream);
    if (token->type == TOKEN_COLON) {
        token_stream_poll(stream);
        type = parse_type(parser, stream);
    }
    return make_var_declare(name, type);
}

var_declare_list_t * parse_var_declare_list(parser_t * parser, token_stream_t * stream) {
    var_declare_t * var_declare = parse_var_declare(parser, stream);
    if (var_declare == NULL) return NULL;
    token_t * token = token_stream_peek(stream);
    if (token->type == TOKEN_COMMA) {
        token_stream_poll(stream);
        return make_var_declare_list(var_declare, parse_var_declare_list(parser, stream));
    } else {
        return make_var_declare_list(var_declare, NULL);
    }
}

ast_struct_t * parse_struct(parser_t * parser, token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_STRUCT, stream);

    token_t * token = token_stream_poll(stream);
    require_token(token, TOKEN_ID, stream);
    string_t * name = string_dup(&token->value);
    declare_type(parser, name, stream);

    require_token(token_stream_poll(stream), TOKEN_LPAREN, stream);
    var_declare_list_t * members = parse_var_declare_list(parser, stream);
    require_token(token_stream_poll(stream), TOKEN_RPAREN, stream);

    type_struct_t * parent = NULL;
    if (token_stream_peek(stream)->type == TOKEN_EXTENDS) {
        token_stream_poll(stream);
        type_t *t = parse_type(parser, stream);
        require(t->category == TC_STRUCT, "Need struct type", stream);
        parent = container_of(t, type_struct_t, super);
    }
    define_var(parser, name, FUN_TYPE, stream);
    define_var(parser, string_concat(name, "Null"), FUN_TYPE, stream);
    type_struct_t * type = make_type_struct(name, members, parent);
    define_type(parser, &type->super, stream);
    return make_struct(type);
}

ast_let_t * parse_let(parser_t * parser, token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_LET, stream);

    var_declare_t * var = parse_var_declare(parser, stream);
    require(var != NULL, "Need variable", stream);

    require_id(token_stream_poll(stream), "=", stream);
    define_var(parser, var->name, var->type, stream);

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
            return &parse_struct(parser, stream)->super;
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
    var_declare_list_t * params = parse_var_declare_list(parser, stream);
    require_token(token_stream_poll(stream), TOKEN_RPAREN, stream);

    parser_enter_scope(parser);
    // Add parameter into scope.
    int param_num = 0;
    for (var_declare_list_t * head = params; head != NULL; head = head->next) {
        define_var(parser, head->var->name, head->var->type, stream);
        ++ param_num;
    }

    ast_cid_t * return_type = NULL;
    if (token_stream_peek(stream)->type == TOKEN_COLON) {
        token_stream_poll(stream);
        return_type = parse_cid(stream);
    }

    ast_expr_t * body = parse_expr(parser, stream);
    parser_exit_scope(parser);

    require(body != NULL, "Need body", stream);
    return make_fun(param_num, params, return_type, body);
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
        require(ast_is_expr(statement->category), "Need expr", stream);
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

ast_ref_t * parse_ref(parser_t * parser, token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token->type != TOKEN_ID) {
        return NULL;
    }
    var_def_t *def = get_var(parser, &token->value, stream);
    token_stream_poll(stream);
    return make_ref(def->super.level, def->super.index, def->super.name, def->type);
}

ast_struct_ref_t * parse_struct_ref(ast_expr_t * base, token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_PERIOD, stream);
    require(base->type->category == TC_STRUCT, "Need struct value", stream);
    type_struct_t *s = container_of(base->type, type_struct_t, super);

    token_t * token = token_stream_poll(stream);
    require_token(token, TOKEN_ID, stream);
    
    int index;
    var_declare_t *var = struct_member_index(s, &token->value, &index);
    require(var != NULL, "Struct has no such member", stream);

    return make_struct_ref(base, index, var->name, var->type);
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
            return &parse_ref(parser, stream)->super;
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
        return &make_fun_apply(&make_ref(op->var->super.level, op->var->super.index, op->var->super.name, FUN_TYPE)->super, make_expr_list(operand, NULL))->super;
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
        return &make_fun_apply(&make_ref(op->var->super.level, op->var->super.index, op->var->super.name, FUN_TYPE)->super,
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

