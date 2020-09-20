#ifndef PRE_PARSER_HPP
#define PRE_PARSER_HPP

#include<vector>
#include"token.hpp"
#include"error.hpp"

using namespace std;

class PreParser {
    PreParser();
    void addTokens(const vector<Token> & tokens);

};




#endif
