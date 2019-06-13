#ifndef TOKENS_H
#define TOKENS_H


typedef int8_t bool;

#define true 1
#define false 0

enum token {
    // single tokens
    LEFT_PAREN = 0, RIGHT_PAREN, COMMA, DOT, SEMICOLON, COLON, LEFT_CURLY, RIGHT_CURLY,
    LEFT_BRACE, RIGHT_BRACE, STAR, SLASH, PLUS, MINUS, PERCENT, BITWISE_AND, BITWISE_OR, BITWISE_NOT,
    PLUS_EQUAL, MINUS_EQUAL, STAR_EQUAL, SLASH_EQUAL, PERCENT_EQUAL,
    BITWISE_AND_EQUAL, BITWISE_OR_EQUAL, BITWISE_XOR_EQUAL, BITWISE_XOR,
    // colliding token chars
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // types
    IDENTIFIER, STRING, NUMBER,

    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, VOID, OR,
    RETURN, THIS, TRUE, INT, FLOAT, CINT, CFLOAT, WHILE, NOT, XOR,
    END_FILE, NEWLINE

};

typedef struct token_t {
    enum token token;
    int8_t * lexme; // only populated if it is an identifier, string, or number so as not to waste memory
    // literal
    uint32_t lineNum;
    uint32_t colNum;
} token_t;

#endif
