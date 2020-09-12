#include "../include/parser.hpp"

Parser::Parser(ErrorHandler *e) {
    this->currentToken = 0;
    this->eHandler = e;  
}

void Parser::addTokens(const vector<Token> & tokens) {
    this->tokens = tokens;
    this->currentToken = 0;
}

bool Parser::isAtEnd() {
    return isAtEnd(this->currentToken);
}

Token Parser::peek() {
    return peek(this->currentToken);
}

Token Parser::previous() {
    return previous(this->currentToken); 
}

Token Parser::advance() {
    return advance(this->currentToken);
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

uint64_t Parser::getCurrentLineNum(uint64_t & currentPos) {
    return peek(currentPos).lineNum + 1;
}

uint64_t Parser::getCurrentColNum(uint64_t & currentPos) {
    return peek(currentPos).colNum + 1;
}

uint64_t Parser::getCurrentLineNum() {
    return getCurrentLineNum(this->currentToken);
}

uint64_t Parser::getCurrentColNum() {
    return getCurrentColNum(this->currentToken);
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

Token Parser::previous(uint64_t & currentPos) {
    return tokens[currentPos - 1];
}

Token Parser::advance(uint64_t & currentPos) {
    if (!isAtEnd(currentPos))
        currentPos++;
    return previous(currentPos);
}

bool Parser::isAtEnd(uint64_t & currentPos) {
    return (currentPos >= tokens.size());
}

Token Parser::peek(uint64_t & currentPos) {
    if (isAtEnd(currentPos)) return Token();
    return this->tokens[currentPos]; 
}

bool Parser::match(uint64_t & currentPos, int num, ...) {
    va_list args;
    va_start(args, num);

    int i = 0;
    while (i < num) {
        TokenType t = va_arg(args, TokenType);
        if (peek(currentPos).tokenType == t) {
            advance(currentPos);
            va_end(args); 
            return true;
        }
        i++;
    }
    va_end(args);
    return false;
}


// returns the position in the tokens array that it ended in
uint64_t Parser::parseHelper(uint64_t currentPos, vector<ParserNode*> & nodes) {
    
    // check while not at end and not at end of scope
    while (!isAtEnd(currentPos) && peek(currentPos).tokenType != TokenType::RIGHT_CURLY) {
        if (match(currentPos, 1, TokenType::IF)) {
            if (peek(currentPos).tokenType != TokenType::LEFT_PAREN) {
                eHandler->reportError(Error(ErrorType::ExpectedLeftParenException, "if", getCurrentLineNum(currentPos), getCurrentColNum(currentPos))); 
                // end here cause fatal error?
            } 
            else {
                int64_t endParen = findMatchingRightParen(currentPos);
                if (endParen < 0) {
                    eHandler->reportError(Error(ErrorType::MissingParenException, getCurrentLineNum(currentPos), getCurrentColNum(currentPos)));
                }
                else {
                    vector<Token> exprTokens = spliceExpression(currentPos, endParen);
                    Expr * expr = new Expr(exprTokens, eHandler);
                    IfNode * node = new IfNode(expr);
                    nodes.push_back(node);
                    return parseHelper(currentPos, node->body);
                } 
            }
        }
        else if (match(currentPos, 1, TokenType::WHILE)) {
            if (peek(currentPos).tokenType != TokenType::LEFT_PAREN) {
                eHandler->reportError(Error(ErrorType::ExpectedLeftParenException, "for", getCurrentLineNum(currentPos), getCurrentColNum(currentPos))); 
            }
            else {
                int64_t endParen = findMatchingRightParen(currentPos);
                if (endParen < 0) {
                    eHandler->reportError(Error(ErrorType::MissingParenException, getCurrentLineNum(currentPos), getCurrentColNum(currentPos)));
                }
                else {
                    vector<Token> exprTokens = spliceExpression(currentPos, endParen);
                    Expr * expr = new Expr(exprTokens, eHandler);
                    WhileNode * node = new WhileNode(expr);
                    nodes.push_back(node);
                    return parseHelper(currentPos, node->body);
                }
            }
        }
        // can be a function call, start of an assignment, or something else
        else if (match(currentPos, 1, TokenType::IDENTIFIER)) {
            if (match(currentPos, 1, TokenType::EQUAL)) {
                // TODO add assignment detecting code here
            }  
        }   
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


