#include "../include/parser.hpp"

Parser::Parser() {
    this->currentToken = 0;    
}

void Parser::addTokens(const vector<Token> & tokens) {
    this->tokens = tokens;
    this->currentToken = 0;
}

bool Parser::isAtEnd(uint64_t dist = 0) {
    return (currentToken + dist >= tokens.size());
}

Token Parser::peek(uint64_t dist = 0) {
    if (isAtEnd(dist)) return Token();
    return Tokens[currentToken + dist]; 
}

Token Parser::previous() {
    return Tokens[currentToken - 1];
}

Token Parser::advance() {
    if (!isAtEnd())
        currentToken++;
    return previous();
}



void Parser::parse() {
    this->currentToken = 0;
    
    while (!isAtEnd()) {
        
    }
}


