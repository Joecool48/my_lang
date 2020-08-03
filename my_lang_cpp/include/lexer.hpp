#include <string>
#include <iostream>
#include <vector>
#include <fstream>
#include <sstream>

#include "token.hpp"

using namespace std;

class Lexer {
    string lexText;
    string sourceFile;
    vector<Token> tokens; 
    uint64_t currentPos;
    uint64_t currentLine;
    uint64_t currentCol;
    
    // for string management
    char startQuote;
    uint64_t startLine;
    uint64_t startCol;
    uint64_t endLine;
    uint64_t endCol;
    stringstream assembledStr;;
    bool isStringStart();
    void setStringStart();
    void tokenizeString();
    
    void readSourceFile(string fileName);
    void lexString();
    vector<Token> getTokens();
    bool isNewline();
    char advance();
    char advance(uint64_t amount);
    char peek();
    char peek(uint64_t amount);
    char getCurrentChar();
    bool endOfFile();
    void reset();
    bool isValidIdentifierChar(char c);
    bool checkPastFileLen(uint64_t amount);
};
