#include <string>
#include <iostream>

using namespace std;

enum class TokenType {
        LEFT_PAREN, RIGHT_PAREN, COMMA, DOT, SEMICOLON, COLON, LEFT_CURLY, RIGHT_CURLY,
        LEFT_BRACE, RIGHT_BRACE, STAR, SLASH, PLUS, MINUS, PERCENT, BITWISE_AND, BITWISE_OR, BITWISE_NOT, BITWISE_XOR,
        BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
        AND, CLASS, ELSE, IF, FALSE, FUN, FOR, VOID, OR, RETURN, THIS, TRUE, INT, FLOAT, CINT, CFLOAT, 
        WHILE, NOT, XOR,
        STRING,
        // Denotes a unfilled type
        NONE
    };

class Token {
    public:
    TokenType tokenType;
    uint64_t lineNum;
    uint64_t colNum;
    Token();
    Token(TokenType tokenType, uint64_t lineNum, uint64_t colNum);
};

class StringToken : Token {
    public:
    string lexme;
    uint64_t endLineNum;
    uint64_t endColNum;
    StringToken(uint64_t lineNum, uint64_t colNum, uint64_t endLineNum, uint64_t endColNum, string lexme);
};
