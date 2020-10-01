#ifndef TOKEN_HPP
#define TOKEN_HPP

#include <string>
#include <iostream>
#include <sstream>

using namespace std;

enum class TokenType {
        LEFT_PAREN, RIGHT_PAREN, COMMA, DOT, SEMICOLON, COLON, LEFT_CURLY, RIGHT_CURLY,
        LEFT_BRACE, RIGHT_BRACE, STAR, SLASH, PLUS, MINUS, PERCENT, BITWISE_AND, BITWISE_OR, BITWISE_NOT, BITWISE_XOR,
        BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL,
        AND, CLASS, ELSE, IF, FALSE, TRUE, FUN, FOR, VOID, OR, RETURN, THIS, INT, FLOAT, CHAR, BOOL, STR,
        WHILE, NOT, XOR,
        STRING, IDENTIFIER, NUMBER_FLOAT, NUMBER_DECIMAL
        // Denotes a unfilled type
        NONE
    };

// string names
//
// symbol operators
const string MULT_OPERATOR = "*";
const string DIV_OPERATOR = "/";
const string ADD_OPERATOR = "+";
const string SUB_OPERATOR = "-";
const string MOD_OPERATOR = "%";
const string BITWISE_AND_OPERATOR = "&";
const string BITWISE_OR_OPERATOR = "|";
const string BITWISE_NOT_OPERATOR = "~";
const string BITWISE_XOR_OPERATOR = "^";

// other symbols
const string LEFT_PAREN = "(";
const string RIGHT_PAREN = ")";
const string COMMA = ",";
const string SEMICOLON = ";";
const string COLON = ":";
const string LEFT_CURLY = "{";
const string RIGHT_CURLY = "}";
const string LEFT_BRACE = "[";
const string RIGHT_BRACE = "]";
const string BANG = "!";
const string BANG_EQUAL = "!=";
const string EQUAL = "=";
const string EQUAL_EQUAL = "==";
const string GREATER = ">";
const string GREATER_EQUAL = ">=";
const string LESS = "<";
const string LESS_EQUAL = "<=";

// add equal operators later

class Token {
    public:
    TokenType tokenType;
    uint64_t lineNum;
    uint64_t colNum;
    uint64_t endLineNum;
    uint64_t endColNum;
    string lexme;
    Token();
    Token(TokenType tokenType, uint64_t lineNum, uint64_t colNum);
    Token(TokenType tokenType, uint64_t lineNum, uint64_t colNum, string lexme);
    Token(TokenType tokenType, uint64_t lineNum, uint64_t colNum, uint64_t endLineNum, uint64_t endColNum, string lexme);
    string toString();
};

#endif
