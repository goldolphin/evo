/**
 * 
 * @author goldolphin
 *         2015-05-02 12:47
 */
#include "character.h"
#include "../common/bitmap.h"

typedef struct {
    bitmap_t * invisible;
    bitmap_t * non_id;
} chardef_t;

static chardef_t g_chardef;

#define MAKE(bitmap) (chardef->bitmap = make_bitmap(CHAR_COUNT))
#define IS_IN(bitmap, c) bitmap_isset(chardef->bitmap, c)
#define ADD(bitmap, c) bitmap_set(chardef->bitmap, c)
#define ADD_ALL(bitmap, src) bitmap_union(chardef->bitmap, chardef->src)
#define ADD_STR(bitmap, s) for (char_t * p = s; *p != '\0'; ++p) ADD(bitmap, *p)

static void chardef_init(chardef_t * chardef) {
    MAKE(invisible);
    for (char_t c = 0; c <= 32; ++c) {
        ADD(invisible, c);
    }
    ADD(invisible, 127);

    MAKE(non_id);
    ADD_ALL(non_id, invisible);
    ADD_STR(non_id, "'\"()\\,.:;[]{}`");
}

static void global_chardef_init() __attribute__((constructor)) {
    chardef_init(&g_chardef);
}

bool is_identifier_letter(char_t c) {
    return !bitmap_isset(g_chardef.non_id, c);
}
