#ifndef PRE_PARSER_HPP
#define PRE_PARSER_HPP

#include<vector>
#include"token.hpp"
#include"error.hpp"
#include"coreParser.hpp"
#include"type.hpp"
#include<stack>

using namespace std;

class PreParser : public CoreParser {
public:
    Qualifiers qualifiers;
    ErrorHandler eHandler;
    PreParser(ErrorHandler * e);
    PreParser(ErrorHandler * e, const vector<Token> & tokens);
    unordered_map<string, Type*> completeTypes;
    unordered_map<string, Type*> incompleteTypes;
    void preParse();
    void init();
    bool matchPrimative(); // determine if current token is a primative
    Type* trackType(const Token & type);
    void gatherMembers(Type *newType);
};




#endif
