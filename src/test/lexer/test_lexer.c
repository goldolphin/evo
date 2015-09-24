#include <stdio.h>
#include <utils/memory.h>
#include <lexer/lexer.h>
#include <lexer/file_char_stream.h>
#include <lexer/token.h>
#include <lexer/basic_char_stream.h>

/**
 * @author goldolphin
 *         2015-08-26 09:29:29.
 */

void print_token(token_t * token) {
    SBUILDER(builder, 1024);
    sbuilder_token(&builder, token);
    printf("%s\n", builder.buf);
}

void test_str(const char * str) {
    basic_char_stream_t stream;
    basic_char_stream_init(&stream, (uint8_t *) str, (int) strlen(str));
    lexer_t lexer;
    lexer_init(&lexer);
    lexer_context_t ctx;
    lexer_init_context(&lexer, &ctx);
    while (true) {
        token_t * token = lexer_poll(&lexer, &ctx, &stream.super);
        ensure(token != NULL);
        if (token == NULL || token->type == TOKEN_END) break;
        print_token(token);
    }
}

void test_file(const char * filename) {
    file_char_stream_t stream;
    file_char_stream_init(&stream, filename, 4096);
    lexer_t lexer;
    lexer_init(&lexer);
    lexer_context_t ctx;
    lexer_init_context(&lexer, &ctx);
    while (true) {
        token_t * token = lexer_poll(&lexer, &ctx, &stream.super);
        ensure(token != NULL);
        if (token == NULL || token->type == TOKEN_END) break;
        print_token(token);
    }
}

int main() {
    test_str("1- - -1");
    test_str("掌上 生活=");
    test_file("src/test/evo/list.evo");
    return 0;
}
