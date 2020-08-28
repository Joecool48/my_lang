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
        AND, CLASS, ELSE, IF, FALSE, TRUE, FUN, FOR, VOID, OR, RETURN, THIS, INT, FLOAT, CINT, CFLOAT, 
        WHILE, NOT, XOR,
        STRING, IDENTIFIER, NUMBER_FLOAT, NUMBER_DECIMAL, NUMBER_HEX,
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
    friend ostream& operator<<(ostream & stream, const Token & tok) {
        stringstream str;
        switch(tok.tokenType) {
            case TokenType::LEFT_PAREN:
                str << "(";
                break;
            case TokenType::RIGHT_PAREN:
                str << ")";
                break;
            case TokenType::COMMA:
                str << ",";
                break;
            case TokenType::DOT:
                str << ".";
                break;
            case TokenType::SEMICOLON:
                str << ";";
                break;
            case TokenType::COLON:
                str << ":";
                break;
            case TokenType::LEFT_CURLY:
                str << "{";
                break;
            case TokenType::RIGHT_CURLY:
                str << "}";
                break;
            case TokenType::LEFT_BRACE:
                str << "[";
                break;
            case TokenType::RIGHT_BRACE:
                str << "]";
                break;
            case TokenType::STAR:
                str << "*";
                break;
            case TokenType::SLASH:
                str << "/";
                break;
            case TokenType::PLUS:
                str << "+";
                break;
            case TokenType::MINUS:
                str << "-";
                break;
            case TokenType::PERCENT:
                str << "%";
                break;
            case TokenType::BITWISE_AND:
                str << "&";
                break;
            case TokenType::BITWISE_OR:
                str << "|";
                break;
            case TokenType::BITWISE_NOT:
                str << "~";
                break;
            case TokenType::BITWISE_XOR:
                str << "^";
                break;
            case TokenType::BANG:
                str << "!";
                break;
            case TokenType::BANG_EQUAL:
                str << "!=";
                break;
            case TokenType::EQUAL:
                str << "=";
                break;
            case TokenType::EQUAL_EQUAL:
                str << "==";
                break;
            case TokenType::GREATER:
                str << ">";
                break;
            case TokenType::GREATER_EQUAL:
                str << ">=";
                break;
            case TokenType::LESS:
                str << "<";
                break;
            case TokenType::LESS_EQUAL:
                str << "<=";
                break;
            case TokenType::AND:
                str << "and";
                break;
            case TokenType::CLASS:
                str << "class";
                break;
            case TokenType::ELSE:
                str << "else";
                break;
            case TokenType::IF:
                str << "if";
                break;
            case TokenType::FALSE:
                str << "false";
                break;
            case TokenType::TRUE:
                str << "true";
                break;
            case TokenType::FUN:
                str << "fun";
                break;
            case TokenType::FOR:
                str << "for";
                break;
            case TokenType::VOID:
                str << "void";
                break;
            case TokenType::OR:
                str << "or";
                break;
            case TokenType::RETURN:
                str << "return";
                break;
            case TokenType::THIS:
                str << "this";
                break;
            case TokenType::INT:
                str << "int";
                break;
            case TokenType::FLOAT:
                str << "float";
                break;
            case TokenType::CINT:
                str << "cint";
                break;
            case TokenType::CFLOAT:
                str << "cfloat";
                break;
            case TokenType::WHILE:
                str << "while";
                break;
            case TokenType::NOT:
                str << "not";
                break;
            case TokenType::XOR:
                str << "xor";
                break;
            case TokenType::STRING:
                str << "\"" << tok.lexme << "\"";
                break;
            case TokenType::NUMBER_FLOAT:
            case TokenType::NUMBER_DECIMAL:
            case TokenType::NUMBER_HEX:
            case TokenType::IDENTIFIER:
                str << tok.lexme;
                break;
            case TokenType::NONE:
                str << "Ugh why none?";
                break;
            default:
                str << "WHAT IS THIS?! " << tok.lexme;
                break;
        }
        stream << str.str();
        return stream;

    }
};

#endif
