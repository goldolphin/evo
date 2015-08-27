/**
 * @author caofuxiang
 *         2015-08-27 10:00:00.
 */

#include "token.h"

void token_init(token_t * token, token_type_t type, uint8_t *value, int value_len) {
    token->type = type;
    token->value = value;
    token->value_len = value_len;
}

bool sbuilder_token(sbuilder_t * builder, token_t * token) {
    sbuilder_format(builder, "`%d, ", token->type);
    sbuilder_nstr(builder, (const char *) token->value, token->value_len);
    return sbuilder_str(builder, "`");
}
