/**
 * @author goldolphin
 *         2015-08-28 16:35:35.
 */

#ifndef EVO_PARSER_H
#define EVO_PARSER_H

#include "ast.h"
#include "token_stream.h"
#include "infix_parser.h"

typedef struct parser_s {
    infix_parser_t infix_parser;
} parser_t;

typedef struct {

} parser_context_t;

void parser_init(parser_t * parser);

void parser_destroy(parser_t * parser);

ast_statement_t * parse_statement(parser_t * parser, token_stream_t * stream);

ast_expr_t * parse_expr(parser_t * parser, token_stream_t * stream);

#endif //EVO_PARSER_H
