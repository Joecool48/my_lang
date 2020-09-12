#ifndef PARSER_H
#define PARSER_H

#include<vector>
#include<string>
#include<iostream>
#include"token.hpp"
#include"error.hpp"
#include"expr.hpp"
#include"parserNode.hpp"
#include<cstdarg>
#include<stack>

using namespace std;

class Parser {
    public:
    Parser(ErrorHandler *e);
    void addTokens(const vector<Token> & tokens);
    void parse();
    private:
    vector<ParserNode *> parserNodes;
    vector<Token> tokens;
    uint64_t currentToken;
    ErrorHandler *eHandler;

    uint64_t parseHelper(uint64_t currentPos, vector<ParserNode*> & nodes);

    bool isAtEnd();     
    Token peek();
    Token advance();
    Token previous();
    uint64_t getCurrentLineNum();
    uint64_t getCurrentColNum();
    int64_t findMatchingRightParen(int64_t pos);
    vector<Token> spliceExpression(uint64_t start, uint64_t end);
    void dumpTokens(const vector<Token> & t);
    bool match(int num, ...);
    
    // members for recursive parsing
    bool match(uint64_t & currentPos, int num, ...);
    Token peek(uint64_t & currentPos);
    bool isAtEnd(uint64_t & currentPos);
    Token advance(uint64_t & currentPos);
    Token previous(uint64_t & currentPos);
    uint64_t getCurrentLineNum(uint64_t & currentPos);
    uint64_t getCurrentColNum(uint64_t & currentPos);
};

#endif
