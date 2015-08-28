#define _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <utils/memory.h>
#include <lexer/lexer.h>
#include <lexer/character.h>

/**
 * @author goldolphin
 *         2015-08-26 09:29:29.
 */

void callback(token_t * token, void * extra) {
    SBUILDER(builder, 1024);
    sbuilder_token(&builder, token);
    printf("%s\n", builder.buf);
}

bool match(lexer_t* lexer, const char * line) {
    return lexer_match(lexer, (uint8_t *) line, (int) strlen(line), callback, NULL);
}

int main() {
    int buf_len = 1024;
    FILE * f = fopen("src/test/evo/list.evo", "r");
    uint8_t *buf = new_array(uint8_t , buf_len);
    lexer_t lexer;
    lexer_init(&lexer);
    // ensure(match(&lexer, "import IO\n"));
    while (true) {
        int read = lexer_read_line(buf, buf_len, f);
        if (read < 0) break;
        ensure(is_linebreak(buf[read-1]));
        ensure(lexer_match(&lexer, buf, read, callback, NULL));
    }
    return 0;
}
