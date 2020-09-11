#include "../include/parser.hpp"

Parser::Parser(ErrorHandler *e) {
    this->currentToken = 0;
    this->eHandler = e;  
}

void Parser::addTokens(const vector<Token> & tokens) {
    this->tokens = tokens;
    this->currentToken = 0;
}

bool Parser::isAtEnd(uint64_t dist) {
    return (currentToken + dist >= tokens.size());
}

Token Parser::peek(uint64_t dist) {
    if (isAtEnd(dist)) return Token();
    return this->tokens[currentToken + dist]; 
}

Token Parser::previous() {
    return tokens[currentToken - 1];
}

Token Parser::advance() {
    if (!isAtEnd())
        currentToken++;
    return previous();
}

bool Parser::match(int num, ...) {
    va_list args;
    va_start(args, num);

    int i = 0;
    while (i < num) {
        TokenType t = va_arg(args, TokenType);
        if (peek().tokenType == t) {
            advance();
            va_end(args); 
            return true;
        }
        i++;
    }
    va_end(args);
    return false;

}

uint64_t Parser::getCurrentLineNum() {
    return peek().lineNum + 1;
}

uint64_t Parser::getCurrentColNum() {
    return peek().colNum + 1;
}

int64_t Parser::findMatchingRightParen(int64_t pos) {
    stack<Token> s;
    s.push(tokens[pos]);
    int64_t i = pos; 
    i++;

    while (!s.empty()) {
        while (i < tokens.size() && (tokens[i].tokenType != TokenType::LEFT_PAREN && tokens[i].tokenType != TokenType::RIGHT_PAREN))
            i++;
        if (i >= tokens.size()) return -1; // couldnt find ending right paren
        else if (tokens[i].tokenType == TokenType::LEFT_PAREN) {
            s.push(tokens[i]);
            i++;
        }
        else if (tokens[i].tokenType == TokenType::RIGHT_PAREN) {
            s.pop();
            if (s.empty()) return i;
            else i++;
        }
        else i++;
    }
    return i;
}

vector<Token> Parser::spliceExpression(uint64_t start, uint64_t end) {
    vector<Token> exprTokens;

    for (uint64_t i = start; i <= end; i++) {
        exprTokens.push_back(tokens[i]);
    }
    
    return exprTokens;
}

void Parser::dumpTokens(const vector<Token> & t) {
    cout << "Size: " << t.size() << endl;
    for (auto it = t.begin(); it != t.end(); ++it) {
        cout << *it << " "; 
    }
    cout << endl;
}

void Parser::analyzeLine() {
     
}



ParserNode* Parser::checkIf(ParserNode * node) {
    // change out this match need one that doesnt just use global currentToken value
    if (match(1, TokenType::IF)) {
            if (peek().tokenType != TokenType::LEFT_PAREN) {
                eHandler->reportError(Error(ErrorType::ExpectedLeftParenException, "if", getCurrentLineNum(), getCurrentColNum())); 
                // end here cause fatal error?
            } 
            else {
                int64_t endParen = findMatchingRightParen(currentToken);
                if (endParen < 0) {
                    eHandler->reportError(Error(ErrorType::MissingParenException, getCurrentLineNum(), getCurrentColNum()));
                }
                else {
                    vector<Token> exprTokens = spliceExpression(currentToken, endParen);
                    dumpTokens(exprTokens); // TEST TODO change later
                    Expr * expr = new Expr(exprTokens, eHandler);
                    expr->dumpAST(); 
                    return;
                } 
            }
        }  
}

// returns the position in the tokens array that it ended in
uint64_t Parser::parseHelper(vector<ParserNode*> & nodes) {
    uint64_t currentPos = currentToken;
    
    // until it reaches the end of its block
    while (currentPos < tokens.size() && tokens[currentPos].tokenType != TokenType::RIGHT_CURLY) {
          
    }  
}

void Parser::parse() {
    this->currentToken = 0;
    
    while (!isAtEnd()) {
        if (match(1, TokenType::IF)) {
            if (peek().tokenType != TokenType::LEFT_PAREN) {
                eHandler->reportError(Error(ErrorType::ExpectedLeftParenException, "if", getCurrentLineNum(), getCurrentColNum())); 
                // end here cause fatal error?
            } 
            else {
                int64_t endParen = findMatchingRightParen(currentToken);
                if (endParen < 0) {
                    eHandler->reportError(Error(ErrorType::MissingParenException, getCurrentLineNum(), getCurrentColNum()));
                }
                else {
                    vector<Token> exprTokens = spliceExpression(currentToken, endParen);
                    dumpTokens(exprTokens); // TEST TODO change later
                    Expr * expr = new Expr(exprTokens, eHandler);
                    expr->dumpAST(); 
                    return;
                } 
            }
        }   
    }
}


