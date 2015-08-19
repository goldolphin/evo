/**
 * 
 * @author goldolphin
 *         2015-05-02 12:49
 */
#include "token.h"
#include "../memory/memory.h"

token_info_t token_end;

void token_init(token_t * token, token_type type, char_t * value) {
    token->type = type;
    token->value = value;
}

void token_init_predefined(token_t * token, char_t * value) {
    return token_init(token, PREDEFINED, value);
}
