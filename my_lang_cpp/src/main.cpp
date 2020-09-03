#include<string>
#include<iostream>
#include<sstream>

#include "../include/lexer.hpp"
#include "../include/expr.hpp"


int main () {
    Lexer lexer;
    lexer.readSourceFile("test.txt");
    lexer.lexString();
    
    lexer.tokenDump();
    Expr expr;
    vector<Token> tokens = lexer.getTokens();
    cout << "Starting parsing" << endl;
    expr.generateAST(tokens);
    cout << "Dumping tree unpruned" << endl;
    expr.levelDumpAST();
    cout << "Pruning AST" << endl;
    expr.pruneAST();
    expr.levelDumpAST();
}
