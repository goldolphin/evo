#include <stdio.h>
#include <utils/memory.h>
#include <lexer/lexer.h>
#include <lexer/file_char_stream.h>

/**
 * @author goldolphin
 *         2015-08-26 09:29:29.
 */

void print_token(token_t * token) {
    SBUILDER(builder, 1024);
    sbuilder_token(&builder, token);
    printf("%s\n", builder.buf);
}

int main() {
    file_char_stream_t stream;
    file_char_stream_init(&stream, "src/test/evo/list.evo", 4096);
    lexer_t lexer;
    lexer_init(&lexer);
    lexer_context_t ctx;
    lexer_init_context(&lexer, &ctx);
    while (true) {
        token_t * token = lexer_poll(&lexer, &ctx, &stream.super);
        ensure(token != NULL);
        if (token == TOKEN_END) break;
        print_token(token);
    }
    return 0;
}
