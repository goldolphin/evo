/**
 * @author goldolphin
 *         2015-09-08 16:09:09.
 */

#include <utils/string.h>
#include <lexer/token.h>
#include <utils/pair.h>
#include "infix_parser.h"
#include "utils.h"
#include "parser.h"
#include "ast.h"

#define INFIX_DEF_MAP_INITIAL_SIZE 128

typedef struct {
    string_t name;
    bool left2right;
    int precedence;
} infix_def_t;

static infix_def_t * make_infix_def(string_t * name, bool left2right, int precedence) {
    infix_def_t * def = new_data(infix_def_t);
    uint8_t * s = new_array(uint8_t, name->len);
    memcpy(s, name->value, (size_t) name->len);
    string_init(&def->name, s, name->len);
    def->left2right = left2right;
    def->precedence = precedence;
    return def;
}

static inline infix_def_t * parse_infix_def(infix_parser_t * parser, token_stream_t * stream) {
    token_t * token = token_stream_peek(stream);
    if (token->type != TOKEN_ID) {
        return NULL;
    }
    pair_t kv;
    if (hashmap_get(&parser->infix_def_map, &token->value, &kv)) {
        token_stream_poll(stream);
        return kv.value;
    }
    return NULL;
}

static inline bool infix_le(infix_def_t *left, infix_def_t *right) {
    if (left->precedence != right->precedence) {
        return left->precedence <= right->precedence;
    }
    return left->left2right;
}

ast_id_t * make_id(string_t * value);

ast_fun_apply_t * make_infix_apply(ast_id_t * infix, ast_expr_t * left, ast_expr_t * right);

ast_expr_t * parse_expr_rec(parser_t * parser, ast_expr_t * left, token_stream_t * stream);

typedef struct {
    ast_expr_t * left;
    infix_def_t * op;
} frame_t;

static ast_fun_apply_t *parse_infix_rec(parser_t *parser, token_stream_t *stream, frame_t *top) {
    frame_t new_top;
    new_top.left = parse_expr_rec(parser, NULL, stream);
    require(new_top.left != NULL, "Need expr", stream);
    new_top.op = parse_infix_def(&parser->infix_parser, stream);

    while (true) {
        if (new_top.op == NULL) {
            ast_fun_apply_t *result = make_infix_apply(make_id(&top->op->name), top->left, new_top.left);
            top->left = &result->super;
            top->op = NULL;
            return result;
        } else {
            if (infix_le(top->op, new_top.op)) {
                top->left = &make_infix_apply(make_id(&top->op->name), top->left, new_top.left)->super;
                top->op = new_top.op;
                return NULL;
            }
            new_top.left = new_top.left;
            new_top.op = new_top.op;
            parse_infix_rec(parser, stream, &new_top);
        }
    }
}

void infix_parser_init(infix_parser_t * parser) {
    hashmap_init1(&parser->infix_def_map, INFIX_DEF_MAP_INITIAL_SIZE, string_hash_func, string_equal_func);
}

void infix_parser_destroy(infix_parser_t * parser) {
    hashmap_destroy(&parser->infix_def_map);
}

bool infix_parser_add(infix_parser_t * parser, string_t * name, bool left2right, int precedence) {
    if (hashmap_contains(&parser->infix_def_map, name)) {
        return false;
    }
    infix_def_t *def = make_infix_def(name, left2right, precedence);
    ensure(hashmap_put(&parser->infix_def_map, &def->name, def));
    return true;
}

ast_fun_apply_t * parse_infix(parser_t * parser, token_stream_t * stream, ast_expr_t * left) {
    require(left != NULL, "Need expr", stream);
    frame_t top;
    top.left = left;
    top.op = parse_infix_def(&parser->infix_parser, stream);
    if (top.op == NULL) {
        return NULL;
    }
    while (true) {
        ast_fun_apply_t *result = parse_infix_rec(parser, stream, &top);
        if (result != NULL) {
            return result;
        }
    }
}
