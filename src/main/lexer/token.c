/**
 * @author goldolphin
 *         2015-08-27 10:00:00.
 */

#include <utils/memory.h>
#include "token.h"

token_t token_end;

token_t * TOKEN_END = &token_end;

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
    string_init(&token->value, value, value_len);
}

bool sbuilder_token(sbuilder_t * builder, token_t * token) {
    sbuilder_format(builder, "`%s, ", token_type_name(token->type));
    sbuilder_string(builder, &token->value);
    return sbuilder_str(builder, "`");
}
