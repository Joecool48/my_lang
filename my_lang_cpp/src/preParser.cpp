#include"../include/preParser.hpp"

PreParser::PreParser() {
    currentToken = 0;
}

PreParser::PreParser(const vector<Token> & tokens) {
    this->tokens = tokens;
    this->currentToken = 0;
}

void PreParser::addTokens(const vector<Token> & tokens) {
    this->tokens = tokens;
    this->currentToken = 0;
}


