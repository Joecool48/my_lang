#ifndef PARSER_H
#define PARSER_H

#include<vector>
#include<string>
#include<iostream>
#include"token.hpp"

using namespace std;

class Parser {
    public:
    Parser();
    void addTokens(const vector<Token> & tokens);
    void parse();
    private:
    vector<Token> tokens;
    uint64_t currentToken;
    


};

#endif