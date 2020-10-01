#ifndef CORE_PARSER_H
#define CORE_PARSER_H

#include<string>
#include<iostream>
#include "token.hpp"
#include "expr.hpp"

using namespace std;

class CoreParser {
public:
    vector<Token> tokens;
    uint64_t currentToken;
    
    void addTokens(const vector<Token> & tokens);
    bool isAtEnd();
    Token peek();
    Token previous();
    Token advance();
    bool match(int num, ...);
    bool check(uint64_t currentPos, int num, ...);
    uint64_t getCurrentLineNum(uint64_t & currentPos);
    uint64_t getCurrentColNum(uint64_t & currentPos);
    uint64_t getCurrentLineNum();
    uint64_t getCurrentColNum();
    Token previous(uint64_t currentPos);
    Token advance(uint64_t & currentPos);
    bool isAtEnd(uint64_t currentPos);
    Token peek(uint64_t currentPos);
    bool match(uint64_t & currentPos, int num, ...);
};

#endif /* CORE_PARSER_H */
