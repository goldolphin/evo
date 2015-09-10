/**
 * @author goldolphin
 *         2015-09-08 16:09:09.
 */

#ifndef EVO_INFIX_PARSER_H
#define EVO_INFIX_PARSER_H

#include <utils/string.h>
#include <utils/hashmap.h>
#include "ast.h"
#include "token_stream.h"

typedef struct {
    hashmap_t infix_def_map;
} infix_parser_t;

struct parser_s;
typedef struct parser_s parser_t;

void infix_parser_init(infix_parser_t * parser);

void infix_parser_destroy(infix_parser_t * parser);

bool infix_parser_add(infix_parser_t * parser, string_t * name, bool left2right, int precedence);

ast_fun_apply_t * parse_infix(parser_t * parser, token_stream_t * stream, ast_expr_t * left);

#endif //EVO_INFIX_PARSER_H
