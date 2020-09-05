#include<string>
#include<iostream>
#include<sstream>

#include "../include/lexer.hpp"
#include "../include/expr.hpp"
#include "../include/parser.hpp"
#include "../include/error.hpp"

int main () {
    Lexer lexer;
    lexer.readSourceFile("test.txt");
    lexer.lexString();
    
    lexer.tokenDump();
    vector<Token> tokens = lexer.getTokens();
    cout << "Starting parsing" << endl;
    
    ErrorHandler * e = new ErrorHandler();

    Parser p(e);
    
    p.addTokens(lexer.getTokens());
    p.parse(); 
}
