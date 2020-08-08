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
