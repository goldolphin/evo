#include <stdio.h>
#include <utils/memory.h>
#include <lexer/lexer.h>
#include <lexer/file_char_stream.h>

/**
 * @author goldolphin
 *         2015-08-26 09:29:29.
 */

void callback(token_t * token, void * extra) {
    SBUILDER(builder, 1024);
    sbuilder_token(&builder, token);
    printf("%s\n", builder.buf);
}

int main() {
    file_char_stream_t stream;
    file_char_stream_init(&stream, "src/test/evo/list.evo", 4096);
    lexer_t lexer;
    lexer_init(&lexer);
    ensure(lexer_match(&lexer, &stream.super, callback, NULL));
    return 0;
}
