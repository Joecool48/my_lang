#include "../include/token.hpp"

// constructor for everything else
Token::Token(TokenType tokenType, uint64_t lineNum, uint64_t colNum) {
    this->tokenType = tokenType;
    this->lineNum = lineNum;
    this->colNum = colNum;
}

Token::Token() {
    this->tokenType = TokenType::NONE;
    this->lineNum = 0;
    this->colNum = 0;
}

// constructor for identifiers and numbers
Token::Token(TokenType tokenType, uint64_t lineNum, uint64_t colNum, string lexme) {
    this->tokenType = tokenType;
    this->lineNum = lineNum;
    this->colNum = colNum;
    this->lexme = lexme;
}

// constructor for strings
Token::Token(TokenType tokenType, uint64_t lineNum, uint64_t colNum, uint64_t endLineNum, uint64_t endColNum, string lexme) {
    this->tokenType = tokenType;
    this->lineNum = lineNum;
    this->colNum = colNum;
    this->endLineNum = endLineNum;
    this->endColNum = endColNum;
    this->lexme = lexme;
}

string stringifyToken(TokenType type) {
    stringstream str;
            switch(type) {
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
}
