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
    return (currentPos + 1) >= lexText.size();
}

void Lexer::reset() {
    sourceFile.clear();
    tokens.clear();
    currentPos = 0;
    currentLine = 0;
    currentCol = 0;
}

vector<Token> Lexer::getTokens() {
    return this->tokens;
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
    stringstream assembledStr;

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
            assembledStr << peek();
            advance();
        }
    }
    if (!stringEnd) {
        // TODO ERROR HERE FOR NOT FINDING END QUOTE 
    }
    // assemble the string and add it as a token
    Token tok(TokenType::STRING, startLine, startCol, endLine, endCol, assembledStr.str());
    tokens.push_back(tok);
     
}

string Lexer::retrieveIdentifier() {
    stringstream s;
    while (isValidIdentifierChar(peek()) && !endOfFile()) {
        s << peek();
        advance();
    }
    return s.str();
}

// checks to see if the string is a keyword
// if so, then it adds it to token list and returns true
bool Lexer::isKeyword(const string & str) {
    TokenType keywordType = TokenType::NONE;
    
    if (str == "and")
        keywordType = TokenType::AND; 
    else if (str == "class")
        keywordType = TokenType::CLASS;
    else if (str == "else") 
        keywordType = TokenType::ELSE;
    else if (str == "if")
        keywordType = TokenType::IF;
    else if (str == "false")
        keywordType = TokenType::FALSE;
    else if (str == "true")
        keywordType = TokenType::TRUE;
    else if (str == "fun")
        keywordType = TokenType::FUN;
    else if (str == "for")
        keywordType = TokenType::FOR;
    else if (str == "void")
        keywordType = TokenType::VOID;
    else if (str == "or")
        keywordType = TokenType::OR;
    else if (str == "return")
        keywordType = TokenType::RETURN;
    else if (str == "this")
        keywordType = TokenType::THIS;
    else if (str == "int")
        keywordType = TokenType::INT;
    else if (str == "float")
        keywordType = TokenType::FLOAT;
    else if (str == "cint")
        keywordType = TokenType::CINT;
    else if (str == "cfloat")
        keywordType = TokenType::CFLOAT;

    if (keywordType != TokenType::NONE) {
        Token tok(keywordType, startLine, startCol);
        tokens.push_back(tok);
        return true;
    }

    return false;
}

bool Lexer::isValidIdentifierStartChar(char c) {
    // first char cant be a number
    return (c >=  'a' && c <= 'z') || (c  >= 'A' && c <= 'Z') || c == '_';
}

void Lexer::setIdentifierStart() {
    startLine = currentLine;
    startCol = currentCol;
}

bool Lexer::isNumber(char c) {
    return (c >='0' && c <= '9');
}

bool Lexer::isAlphaOrUnderbar(char c) {
    return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
}

// TODO add hexidecimal support and possibly octal later
string Lexer::retrieveNumber() {
    stringstream str;
    seenDecimal = false;
    while ((isNumber(peek()) || (peek() == '.'))  && !endOfFile()) {
        if (peek() == '.') {
            if (seenDecimal) {
                // TODO error here for too many decimal points 
            }
            else {
                seenDecimal = true;
            }
        }
        str << peek();
        advance(); 
    }

    if (!endOfFile() && isAlphaOrUnderbar(peek())) {
        // TODO error should be space between number and keyword/identifier
    }

    return str.str();
}

void Lexer::setNumberStart() {
    startLine = currentLine;
    startCol = currentCol;
}

bool Lexer::match(const string & searchStr) {
    for (uint64_t i = 0; i < searchStr.size(); i++) {
        if (endOfFile() || peek(i) != searchStr[i])
            return false;
    }
    return true;
}

bool Lexer::isOperator() {
    TokenType type = TokenType::NONE;

    startLine = currentLine;
    startCol = currentCol;
    if (match(LESS_EQUAL)) {
        type = TokenType::LESS_EQUAL;
        advance(LESS_EQUAL.size());
    }
    else if (match(GREATER_EQUAL)) {
        type = TokenType::GREATER_EQUAL;
        advance(GREATER_EQUAL.size());
    }
    else if (match(EQUAL_EQUAL)) {
        type = TokenType::EQUAL_EQUAL;
        advance(EQUAL_EQUAL.size());
    }
    else if (match(BANG_EQUAL)) {
        type = TokenType::BANG_EQUAL;
        advance(BANG_EQUAL.size());
    }
    else if (match(ADD_OPERATOR)) {
        type = TokenType::PLUS;
        advance(ADD_OPERATOR.size()); 
    }
    else if (match(SUB_OPERATOR)) {
        type = TokenType::MINUS;
        advance(SUB_OPERATOR.size());
    }
    else if (match(MULT_OPERATOR)) {
        type = TokenType::STAR;
        advance(MULT_OPERATOR.size()); 
    }
    else if (match(DIV_OPERATOR)) {
        type = TokenType::SLASH;
        advance(DIV_OPERATOR.size());
    }
    else if (match(MOD_OPERATOR)) {
        type = TokenType::PERCENT;
        advance(MOD_OPERATOR.size());
    }
    else if (match(BITWISE_AND_OPERATOR)) {
        type = TokenType::BITWISE_AND;
        advance(BITWISE_AND_OPERATOR.size());
    }
    else if (match(BITWISE_OR_OPERATOR)) {
        type = TokenType::BITWISE_OR;
        advance(BITWISE_OR_OPERATOR.size()); 
    }
    else if (match(BITWISE_NOT_OPERATOR)) {
        type = TokenType::BITWISE_NOT;
        advance(BITWISE_NOT_OPERATOR.size());
    }
    else if (match(BITWISE_XOR_OPERATOR)) {
        type = TokenType::BITWISE_XOR;
        advance(BITWISE_XOR_OPERATOR.size());
    }
    else if (match(LEFT_PAREN)) {
        type = TokenType::LEFT_PAREN;
        advance(LEFT_PAREN.size());
    }
    else if (match(RIGHT_PAREN)) {
        type = TokenType::RIGHT_PAREN;
        advance(RIGHT_PAREN.size());
    }
    else if (match(COMMA)) {
        type = TokenType::COMMA;
        advance(COMMA.size());
    }
    else if (match(SEMICOLON)) {
        type = TokenType::SEMICOLON;
        advance(SEMICOLON.size());
    }
    else if (match(COLON)) {
        type = TokenType::COLON;
        advance(COLON.size());
    }
    else if (match(LEFT_CURLY)) {
        type = TokenType::LEFT_CURLY;
        advance(LEFT_CURLY.size());
    }
    else if (match(RIGHT_CURLY)) {
        type = TokenType::RIGHT_CURLY;
        advance(RIGHT_CURLY.size());
    } 
    else if (match(LEFT_BRACE)) {
        type = TokenType::LEFT_BRACE;
        advance(LEFT_BRACE.size());
    }
    else if (match(RIGHT_BRACE)) {
        type = TokenType::RIGHT_BRACE;
        advance(RIGHT_BRACE.size());
    }
    else if (match(BANG)) {
        type = TokenType::BANG; 
        advance(BANG.size());
    }
    else if (match(EQUAL)) {
        type = TokenType::EQUAL;
        advance(EQUAL.size());
    }
    else if (match(GREATER)) {
        type = TokenType::GREATER;
        advance(GREATER.size());
    } 
    else if (match(LESS)) {
        type = TokenType::LESS;
        advance(LESS.size());
    }
    
    if (type != TokenType::NONE) {
        Token tok(type, startLine, startCol);
        tokens.push_back(tok);
        return true; 
    }
    
    return false;
}

void Lexer::tokenDump() {
    stringstream str;
    for (auto it = tokens.begin(); it != tokens.end(); ++it) {
        switch(it->tokenType) {
            case TokenType::LEFT_PAREN:
                str << "(";
                break;
            case TokenType::RIGHT_PAREN:
                str << ")";
                break;
            case TokenType::COMMA:
                str << ",";
                break;
            case TokenType::DOT:
                str << ".";
                break;
            case TokenType::SEMICOLON:
                str << ";";
                break;
            case TokenType::COLON:
                str << ":";
                break;
            case TokenType::LEFT_CURLY:
                str << "{";
                break;
            case TokenType::RIGHT_CURLY:
                str << "}";
                break;
            case TokenType::LEFT_BRACE:
                str << "[";
                break;
            case TokenType::RIGHT_BRACE:
                str << "]";
                break;
            case TokenType::STAR:
                str << "*";
                break;
            case TokenType::SLASH:
                str << "/";
                break;
            case TokenType::PLUS:
                str << "+";
                break;
            case TokenType::MINUS:
                str << "-";
                break;
            case TokenType::PERCENT:
                str << "%";
                break;
            case TokenType::BITWISE_AND:
                str << "&";
                break;
            case TokenType::BITWISE_OR:
                str << "|";
                break;
            case TokenType::BITWISE_NOT:
                str << "~";
                break;
            case TokenType::BITWISE_XOR:
                str << "^";
                break;
            case TokenType::BANG:
                str << "!";
                break;
            case TokenType::BANG_EQUAL:
                str << "!=";
                break;
            case TokenType::EQUAL:
                str << "=";
                break;
            case TokenType::EQUAL_EQUAL:
                str << "==";
                break;
            case TokenType::GREATER:
                str << ">";
                break;
            case TokenType::GREATER_EQUAL:
                str << ">=";
                break;
            case TokenType::LESS:
                str << "<";
                break;
            case TokenType::LESS_EQUAL:
                str << "<=";
                break;
            case TokenType::AND:
                str << "and";
                break;
            case TokenType::CLASS:
                str << "class";
                break;
            case TokenType::ELSE:
                str << "else";
                break;
            case TokenType::IF:
                str << "if";
                break;
            case TokenType::FALSE:
                str << "false";
                break;
            case TokenType::TRUE:
                str << "true";
                break;
            case TokenType::FUN:
                str << "fun";
                break;
            case TokenType::FOR:
                str << "for";
                break;
            case TokenType::VOID:
                str << "void";
                break;
            case TokenType::OR:
                str << "or";
                break;
            case TokenType::RETURN:
                str << "return";
                break;
            case TokenType::THIS:
                str << "this";
                break;
            case TokenType::INT:
                str << "int";
                break;
            case TokenType::FLOAT:
                str << "float";
                break;
            case TokenType::CINT:
                str << "cint";
                break;
            case TokenType::CFLOAT:
                str << "cfloat";
                break;
            case TokenType::WHILE:
                str << "while";
                break;
            case TokenType::NOT:
                str << "not";
                break;
            case TokenType::XOR:
                str << "xor";
                break;
            case TokenType::STRING:
                str << "\"" << it->lexme << "\"";
                break;
            case TokenType::NUMBER_FLOAT:
            case TokenType::NUMBER_DECIMAL:
            case TokenType::NUMBER_HEX:
            case TokenType::IDENTIFIER:
                str << it->lexme;
                break;
            default:
                str << "WHAT IS THIS?!";
                break;
        }
        str << " " << it->lineNum << " " << it->colNum;
        str << endl;
    }
    cout << str.str(); 
}

void Lexer::lexString() {
    reset();
    cout << "Lexing String" << endl;
    while (!endOfFile()) {
        if (isStringStart()) {
            setStringStart();
            tokenizeString();
        }
        else if (isOperator()) {
        
        }
        else if (isNumber(peek())) {
            setNumberStart();
            string str = retrieveNumber();
            TokenType type = TokenType::NUMBER_DECIMAL;
            if (seenDecimal) 
                type = TokenType::NUMBER_FLOAT;
            
            Token tok(type, startLine, startCol, str); 
            tokens.push_back(tok);
        }
        else if (isValidIdentifierStartChar(peek())) {
            setIdentifierStart();
            string str = retrieveIdentifier();
            if (!isKeyword(str)) {
                // must be an identifier of sorts
                Token tok(TokenType::IDENTIFIER, startLine, startCol, str);
                tokens.push_back(tok);
            }
        }
        else {
            advance(1); // skip spaces and newlines
        } 
    } 
}
