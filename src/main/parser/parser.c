/**
 * @author goldolphin
 *         2015-08-28 16:35:35.
 */

#include "parser.h"
#include "utils.h"

static inline string_t * string_dup(string_t * from) {
    uint8_t * s = new_array(uint8_t, from->len);
    memcpy(s, from->value, (size_t) from->len);
    string_t * to = new_data(string_t);
    string_init(to, s, from->len);
    return to;
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

ast_let_t * parse_let(token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_LET, stream);

    ast_var_declare_t * var = parse_var_declare(stream);
    require(var != NULL, "Need id", stream);

    require_id(token_stream_poll(stream), "=", stream);

    ast_expr_t * expr = parse_expr(stream);
    require(expr != NULL, "Need expr", stream);
    return make_let(var, expr);
}


ast_statement_t * parse_statement(token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token == TOKEN_END) {
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

ast_fun_t * parse_fun(token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_FUN, stream);

    require_token(token_stream_poll(stream), TOKEN_LPAREN, stream);
    ast_var_declare_list_t * params = parse_var_declare_list(stream);
    require_token(token_stream_poll(stream), TOKEN_RPAREN, stream);

    ast_cid_t * return_type = NULL;
    if (token_stream_peek(stream)->type == TOKEN_COLON) {
        token_stream_poll(stream);
        return_type = parse_cid(stream);
    }

    ast_expr_t * body = parse_expr(stream);
    require(body != NULL, "Need body", stream);

    return make_fun(params, return_type, body);
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
        require(ast_is_expr(statement->type), "Need expr", stream);
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
    if (token->type != TOKEN_NUMBER) {
        return NULL;
    }
    token_stream_poll(stream);
    char str[token->value.len+1];
    memcpy(str, token->value.value, (size_t) token->value.len);
    str[token->value.len] = '\0';
    return make_double(atof(str));
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

static ast_expr_t * parse_expr_rec(ast_expr_t * left, token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token == TOKEN_END) {
        return NULL;
    }
    ast_expr_t * new_left = NULL;
    if (left == NULL) {
        switch (token->type) {
            case TOKEN_FUN:
                new_left = &parse_fun(stream)->super;
                break;
            case TOKEN_LPAREN:
                new_left = &parse_block(stream)->super;
                break;
            case TOKEN_STRING:
                new_left = &parse_str(stream)->super;
                break;
            case TOKEN_NUMBER:
                new_left = &parse_double(stream)->super;
                break;
            case TOKEN_ID:
                new_left = &parse_ref(NULL, stream)->super;
            default:
                break;
        }
    } else {
        switch (token->type) {
            case TOKEN_LPAREN:
                new_left = &parse_fun_apply(left, stream)->super;
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
        return parse_expr_rec(new_left, stream);
    }
}

ast_expr_t * parse_expr(token_stream_t * stream) {
    return parse_expr_rec(NULL, stream);
}
