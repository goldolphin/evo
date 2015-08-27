#define _GNU_SOURCE

#include <stdio.h>
#include <stdbool.h>
#include <utils/memory.h>
#include <lexer/lexer.h>
#include <utils/sbuilder.h>

/**
 * @author caofuxiang
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
    char *buf = new_array(char, buf_len);
    lexer_t lexer;
    lexer_init(&lexer);
    // ensure(match(&lexer, "import IO\n"));
    while (true) {
        if (fgets(buf, buf_len, f) == NULL) break;
        // printf("Input: %s\n", buf);
        ensure(match(&lexer, buf));
    }
    return 0;
}
