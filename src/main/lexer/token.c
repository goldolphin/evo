/**
 * @author goldolphin
 *         2015-08-27 10:00:00.
 */

#include <utils/memory.h>
#include "token.h"

const char *token_type_name(token_type_t type) {
    switch (type) {
#define TOKEN_DEF(a, b) case a: return #a;
#include "token_define.def"
#undef TOKEN_DEF
        default: return NULL;
    }
}

const char *token_type_value(token_type_t type) {
    switch (type) {
#define TOKEN_DEF(a, b) case a: return b;
#include "token_define.def"
#undef TOKEN_DEF
        default: return NULL;
    }
}

void token_init(token_t * token, token_type_t type, uint8_t *value, int value_len) {
    token->type = type;
    token->value = value;
    token->value_len = value_len;
}

bool sbuilder_token(sbuilder_t * builder, token_t * token) {
    sbuilder_format(builder, "`%s, ", token_type_name(token->type));
    sbuilder_nstr(builder, (const char *) token->value, token->value_len);
    return sbuilder_str(builder, "`");
}
