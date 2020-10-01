#include "../include/token.hpp"

// constructor for everything else
Token::Token(TokenType tokenType, uint64_t lineNum, uint64_t colNum) {
    this->tokenType = tokenType;
    this->lineNum = lineNum;
    this->colNum = colNum;
}

Token::Token() {
    this->tokenType = TokenType::NONE;
    this->lineNum = 0;
    this->colNum = 0;
}

// constructor for identifiers and numbers
Token::Token(TokenType tokenType, uint64_t lineNum, uint64_t colNum, string lexme) {
    this->tokenType = tokenType;
    this->lineNum = lineNum;
    this->colNum = colNum;
    this->lexme = lexme;
}

// constructor for strings
Token::Token(TokenType tokenType, uint64_t lineNum, uint64_t colNum, uint64_t endLineNum, uint64_t endColNum, string lexme) {
    this->tokenType = tokenType;
    this->lineNum = lineNum;
    this->colNum = colNum;
    this->endLineNum = endLineNum;
    this->endColNum = endColNum;
    this->lexme = lexme;
}

string Token::toString() {
    stringstream str;
    switch(this->tokenType) {
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
        case TokenType::CHAR:
            str << "char";
            break;
        case TokenType::BOOL:
            str << "bool";
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
            str << "\"" << tok.lexme << "\"";
            break;
        case TokenType::STR:
            str << "string";
            break;
        case TokenType::NUMBER_FLOAT:
        case TokenType::NUMBER_DECIMAL:
        case TokenType::IDENTIFIER:
            str << tok.lexme;
            break;
        case TokenType::NONE:
            str << "Ugh why none?";
            break;
    }
    return str.str();
}
