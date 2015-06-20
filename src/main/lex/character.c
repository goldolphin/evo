/**
 * 
 * @author goldolphin
 *         2015-05-02 12:47
 */
#include "character.h"
#include "../common/bitmap.h"
#include "../memory/memory.h"

typedef struct {
    bitmap_t * invisible;
    bitmap_t * non_id;
} chardef_t;

#define MAKE(bitmap) (chardef->bitmap = make_bitmap(CHAR_COUNT))
#define IS_IN(bitmap, c) bitmap_isset(chardef->bitmap, c)
#define ADD(bitmap, c) bitmap_set(chardef->bitmap, c)
#define ADD_ALL(bitmap, src) bitmap_union(chardef->bitmap, chardef->src)
#define ADD_STR(bitmap, s) for (char_t * p = s; *p != '\0'; ++p) ADD(bitmap, *p)

chardef_t * make_chardef() {
    chardef_t * chardef = new_data(chardef_t);

    MAKE(invisible);
    for (int c = 0; c < 32; ++c) {
        ADD(invisible, c);
    }
    ADD(invisible, 127);

    MAKE(non_id);
    ADD_ALL(non_id, invisible);
    ADD_STR(non_id, "'\"()\\,.:;[]{}`");
}

bool is_identifier_char(chardef_t * chardef, char_t c) {
    return !IS_IN(non_id, c);
}

bool is_digit(chardef_t * chardef, char_t c) {
    return c >= '0' && c <= '9';
}
