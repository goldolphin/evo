/**
 * @author goldolphin
 *         2015-09-08 16:09:09.
 */

#include <utils/string.h>
#include "infix_parser.h"
#include "ast.h"
#include "utils.h"

infix_def_t * get_infix_def(token_t * token) {
    return NULL;
}

ast_fun_apply_t * infix_parser_parse(ast_expr_t * left, token_stream_t * stream) {
    require(left != NULL, "Need expr", stream);
    infix_def_t * infix_def = get_infix_def(token_stream_peek(stream));
    if (infix_def == NULL) {
        return NULL;
    }
    token_stream_poll(stream);

    return NULL;
}