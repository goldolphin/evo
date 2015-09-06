/**
 * @author goldolphin
 *         2015-08-28 16:35:35.
 */

#include <lexer/token.h>
#include "token_stream.h"
#include "ast.h"

static inline void require(bool condition, const char * message) {
    if (!condition) {
        fprintf(stderr, "%s\n", message);
        ensure(false);
    }
}

static inline void require_token(token_t * token, token_type_t type) {
    if (token == NULL || token->type != type) {
        SBUILDER(builder, 1024);
        sbuilder_format(&builder, "Need token %s, but given ", token_type_name(type));
        sbuilder_token(&builder, token);
        require(false, builder.buf);
    }
}

static inline void require_id(token_t * token, const char * value) {
    if (token == NULL || token->type != TOKEN_ID || !string_equals_c(&token->value, value)) {
        SBUILDER(builder, 1024);
        sbuilder_format(&builder, "Need id %s, but given ", value);
        sbuilder_token(&builder, token);
        require(false, builder.buf);
    }
}

static inline string_t * string_dup(string_t * from) {
    uint8_t * s = new_array(uint8_t, from->len);
    memcpy(s, from->value, (size_t) from->len);
    string_t * to = new_data(string_t);
    string_init(to, s, from->len);
    return to;
}

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

ast_import_t * make_import(ast_cid_t * module) {
    ast_import_t * import = new_data(ast_import_t);
    import->super.type = AST_IMPORT;
    import->module = module;
    return import;
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

ast_struct_t * make_struct(ast_id_t * id, ast_var_declare_list_t * members, ast_cid_t * parent) {
    ast_struct_t * s = new_data(ast_struct_t);
    s->id = id;
    s->members = members;
    s->parent = parent;
    return s;
}

ast_let_t * make_let(ast_var_declare_t * var, ast_expr_t * expr) {
    ast_let_t * let = new_data(ast_let_t);
    let->var = var;
    let->expr = expr;
    return let;
}

ast_fun_t * make_fun(ast_var_declare_list_t * params, ast_cid_t * return_type, ast_expr_t * body) {
    ast_fun_t * fun = new_data(ast_fun_t);
    fun->params = params;
    fun->return_type = return_type;
    fun->body = body;
    return fun;
}

ast_statement_list_t * make_statement_list(ast_statement_t * statement, ast_statement_list_t * next) {
    ast_statement_list_t * list = new_data(ast_statement_list_t);
    list->statement = statement;
    list->next = next;
    return list;
}

ast_block_t * make_block(ast_statement_list_t * statements, ast_expr_t * the_last) {
    ast_block_t * block = new_data(ast_block_t);
    block->statements = statements;
    block->the_last = the_last;
    return block;
}

ast_str_t * make_str(string_t * value) {
    ast_str_t * str = new_data(ast_str_t);
    str->value = string_dup(value);
    return str;
}

ast_double_t * make_double(double value) {
    ast_double_t * d = new_data(ast_double_t);
    d->value = value;
    return d;
}

// Parsers

ast_statement_t * parse_statement(token_stream_t * stream);

ast_expr_t * parse_expr(token_stream_t * stream);

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
    require_token(token_stream_poll(stream), TOKEN_IMPORT);

    ast_cid_t * module = parse_cid(stream);
    require(module != NULL, "Need cid!");
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
        require(type != NULL, "Need cid!");
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
    require_token(token_stream_poll(stream), TOKEN_STRUCT);

    ast_id_t * id = parse_id(stream);
    require(id != NULL, "Need id!");

    require_token(token_stream_poll(stream), TOKEN_LPAREN);
    ast_var_declare_list_t * members = parse_var_declare_list(stream);
    require_token(token_stream_poll(stream), TOKEN_RPAREN);

    ast_cid_t * parent = NULL;
    if (token_stream_peek(stream)->type == TOKEN_EXTENDS) {
        token_stream_poll(stream);
        parent = parse_cid(stream);
    }

    return make_struct(id, members, parent);
}

ast_let_t * parse_let(token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_LET);

    ast_var_declare_t * var = parse_var_declare(stream);
    require(var != NULL, "Need id");

    require_id(token_stream_poll(stream), "=");

    ast_expr_t * expr = parse_expr(stream);
    require(expr != NULL, "Need expr");
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
    require_token(token_stream_poll(stream), TOKEN_FUN);

    require_token(token_stream_poll(stream), TOKEN_LPAREN);
    ast_var_declare_list_t * params = parse_var_declare_list(stream);
    require_token(token_stream_poll(stream), TOKEN_RPAREN);

    ast_cid_t * return_type = NULL;
    if (token_stream_peek(stream)->type == TOKEN_COLON) {
        return_type = parse_cid(stream);
    }

    ast_expr_t * body = parse_expr(stream);
    require(body != NULL, "Need body");

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
        require(ast_is_expr(statement->type), "Need expr");
        *p_statements = NULL;
        *p_the_last = container_of(statement, ast_expr_t, super);
        return true;
    }
}

ast_block_t * parse_block(token_stream_t * stream) {
    require_token(token_stream_poll(stream), TOKEN_LPAREN);
    ast_statement_list_t * statements;
    ast_expr_t * the_last;
    require(parse_block_rec(stream, &statements, &the_last), "Need block");
    require_token(token_stream_poll(stream), TOKEN_RPAREN);
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
    if (token->type != TOKEN_ID) {
        return NULL;
    }
    token_stream_poll(stream);
    return make_double(&token->value);
}

ast_expr_t * parse_expr(token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token == TOKEN_END) {
        return NULL;
    }

    switch (token->type) {
        case TOKEN_FUN:
            return &parse_fun(stream)->super;
        case TOKEN_LPAREN:
            return &parse_block(stream)->super;
        case TOKEN_STRING:
            return &parse_str(stream)->super;
        case TOKEN_NUMBER:
            return &parse_double(stream)->super;
        default:
            break;
    }

}


void parser_parse(token_stream_t *stream) {
}