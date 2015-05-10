/**
 * 
 * @author goldolphin
 *         2015-05-02 12:49
 */
#include "token.h"
#include "../memory/memory.h"

token_info_t token_end;

token_t * make_token(token_type type, char_t * value) {
    token_t * t = new_data(token_t);
    t->type = type;
    t->value = value;
    return t;
}

token_t * predefine_token(char_t * value) {
    return make_token(PREDEFINED, value);
}
