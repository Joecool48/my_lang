#ifndef PRE_PARSER_HPP
#define PRE_PARSER_HPP

#include<vector>
#include"token.hpp"
#include"error.hpp"

using namespace std;

class PreParser {
    PreParser();
    PreParser(const vector<Token> & tokens);
    vector<Token> tokens;
    uint64_t currentToken;
    void addTokens(const vector<Token> & tokens);
    
};




#endif
