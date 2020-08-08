#include<string>
#include<iostream>
#include<sstream>

#include "../include/lexer.hpp"



int main () {
    Lexer lexer;
    lexer.readSourceFile("test.txt");
    lexer.lexString();
    
    lexer.tokenDump();
}
