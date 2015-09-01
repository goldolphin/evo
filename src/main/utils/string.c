/**
 * @author goldolphin
 *         2015-09-01 19:58:58.
 */

#include "string.h"

void string_init(string_t *str, uint8_t *value, int len) {
    str->value = value;
    str->len = len;
}

bool sbuilder_string(sbuilder_t * builder, string_t * str) {
    return sbuilder_nstr(builder, (const char *) str->value, str->len);
}
