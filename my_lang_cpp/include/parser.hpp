#ifndef PARSER_H
#define PARSER_H

#include<vector>
#include<string>
#include<iostream>
#include"token.hpp"
#include"error.hpp"
#include<cstdarg>
#include<stack>

using namespace std;

class Parser {
    public:
    Parser(ErrorHandler *e);
    void addTokens(const vector<Token> & tokens);
    void parse();
    private:
    vector<Token> tokens;
    uint64_t currentToken;
    ErrorHandler *eHandler;
    bool isAtEnd(uint64_t dist = 0);     
    Token peek(uint64_t dist = 0);
    Token advance();
    Token previous();
    uint64_t getCurrentLineNum();
    uint64_t getCurrentColNum();
    int64_t findMatchingRightParen(int64_t pos);
    vector<Token> spliceExpression(uint64_t start, uint64_t end);
    void dumpTokens(const vector<Token> & t);
    bool match(int num, ...);
};

#endif
