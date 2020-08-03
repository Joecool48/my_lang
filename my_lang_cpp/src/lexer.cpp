#include "../include/lexer.hpp"

void Lexer::readSourceFile(string fileName) {
    fstream file;
    file.open(fileName);
    stringstream strStream;
    strStream << file.rdbuf();
    lexText = strStream.str();
}

bool Lexer::isNewline() {
    return lexText[currentPos] == '\n'; 
}

char Lexer::getCurrentChar() {
    return lexText[currentPos];
}

char Lexer::advance() {
    if (isNewline()) {
        currentLine++;
        currentCol = 0; 
    } else {
        currentCol++;
    }
    currentPos++;
    if (checkPastFileLen(0)) return EOF;
    return peek();
}
char Lexer::advance(uint64_t amount) {
    for (uint64_t i = 0; i < amount && !endOfFile(); i++) advance(); 
    return peek();
}

char Lexer::peek() {
    if (checkPastFileLen(1)) return EOF;
    return lexText[currentPos];
}

char Lexer::peek(uint64_t amount) {
    return lexText[currentPos + amount];
}

bool Lexer::endOfFile() {
    return currentPos + 1 >= lexText.size();
}

void Lexer::reset() {
    lexText.clear();
    sourceFile.clear();
    tokens.clear();
    currentPos = 0;
    currentLine = 0;
    currentCol = 0;
}

bool Lexer::isValidIdentifierChar(char c) {
    return ((c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_' || (c >= '0' && c <= '9'));
}

bool Lexer::checkPastFileLen(uint64_t amount) {
    return currentPos + amount >= lexText.size();
}

bool Lexer::isStringStart() {
    return peek() == '\'' || peek() == '\"';
}

void Lexer::setStringStart() {
    startQuote = peek();
    startLine = currentLine;
    startCol = currentCol;  
    advance(); 
}

void Lexer::tokenizeString() {
    bool stringEnd = false;
    while (!endOfFile()) {
        if (peek() == '\\') {
            // check for escaped characters
            switch (peek(1)) {
                case '\\':
                    assembledStr << "\\";
                    advance(2);
                    break;
                case 'n':
                    assembledStr << '\n';
                    advance(2);
                    break;
                case 't':
                    assembledStr << '\t';
                    advance(2);
                    break;
                case 'r':
                    assembledStr << '\r';
                    advance(2);
                    break;
                case '\"':
                    assembledStr << '\"';
                    advance(2);
                    break;
                case '\'':
                    assembledStr << '\'';
                    advance(2);
                    break;
            }
        }
        else if (peek() == startQuote) {
            endLine = currentLine;
            endCol = currentCol;
            // check for errors
            stringEnd = true;
            advance(1);
            break;
        } 
        else {
            assembledStr << peek(0);
            advance();
        }
    }
    if (!stringEnd) {
        // TODO ERROR HERE FOR NOT FINDING END QUOTE 
    }
    // assemble the string and add it as a token
    StringToken tok();
}

void Lexer::lexString() {
    reset();
    while (!endOfFile()) {
        if (isStringStart()) {
            setStringStart();
            tokenizeString();
        }
    } 
}
