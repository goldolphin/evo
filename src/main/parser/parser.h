/**
 * @author goldolphin
 *         2015-08-28 16:35:35.
 */

#ifndef EVO_PARSER_H
#define EVO_PARSER_H

#include "ast.h"
#include "token_stream.h"

typedef struct {

} parser_t;

typedef struct {

} parser_context_t;

ast_statement_t * parse_statement(token_stream_t * stream);

ast_expr_t * parse_expr(token_stream_t * stream);

#endif //EVO_PARSER_H
