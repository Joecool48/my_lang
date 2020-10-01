#include "../include/coreParser.hpp"

void CoreParser::addTokens(const vector<Token> & tokens) {
    this->tokens = tokens;
    this->currentToken = 0;
}

bool CoreParser::isAtEnd() {
    return isAtEnd(this->currentToken);
}

Token CoreParser::peek() {
    return peek(this->currentToken);
}

Token CoreParser::previous() {
    return previous(this->currentToken);
}

Token CoreParser::advance() {
    return advance(this->currentToken);
}

bool CoreParser::match(int num, ...) {
    va_list args;
    va_start(args, num);

    int i = 0;
    while (i < num) {
        TokenType t = va_arg(args, TokenType);
        if (peek().tokenType == t) {
            advance();
            va_end(args);
            return true;
        }
        i++;
    }
    va_end(args);
    return false;
}

// simple function to check whether one of the given tokens is at the current position
bool CoreParser::check(uint64_t currentPos, int num, ...) {
    va_list args;
    va_start(args, num);

    int i = 0;
    while (i < num) {
        TokenType t = va_arg(args, TokenType);
        if (peek(currentPos).tokenType == t) {
            va_end(args);
            return true;
        }
        i++;
    }
    va_end(args);
    return false;
}

uint64_t CoreParser::getCurrentLineNum(uint64_t & currentPos) {
    return peek(currentPos).lineNum + 1;
}

uint64_t CoreParser::getCurrentColNum(uint64_t & currentPos) {
    return peek(currentPos).colNum + 1;
}

uint64_t CoreParser::getCurrentLineNum() {
    return getCurrentLineNum(this->currentToken);
}

uint64_t CoreParser::getCurrentColNum() {
    return getCurrentColNum(this->currentToken);
}

Token CoreParser::previous(uint64_t currentPos) {
    return tokens[currentPos - 1];
}

Token CoreParser::advance(uint64_t & currentPos) {
    if (!isAtEnd(currentPos))
        currentPos++;
    return previous(currentPos);
}

bool CoreParser::isAtEnd(uint64_t currentPos) {
    return (currentPos >= tokens.size());
}

Token CoreParser::peek(uint64_t currentPos) {
    if (isAtEnd(currentPos)) return Token();
    return this->tokens[currentPos];
}

bool CoreParser::match(uint64_t & currentPos, int num, ...) {
    va_list args;
    va_start(args, num);

    int i = 0;
    while (i < num) {
        TokenType t = va_arg(args, TokenType);
        if (peek(currentPos).tokenType == t) {
            advance(currentPos);
            va_end(args);
            return true;
        }
        i++;
    }
    va_end(args);
    return false;
}
