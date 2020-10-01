#include "../include/parser.hpp"

Parser::Parser(ErrorHandler *e) {
    this->currentToken = 0;
    this->eHandler = e;
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
                    currentPos = parseHelper(currentPos, node->body);
                }
            }
        }
        else if (match(currentPos, 1, TokenType::WHILE)) {
            if (peek(currentPos).tokenType != TokenType::LEFT_PAREN) {
                eHandler->reportError(Error(ErrorType::ExpectedLeftParenException, "while", getCurrentLineNum(currentPos), getCurrentColNum(currentPos)));
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
                    currentPos = parseHelper(currentPos, node->body);
                }
            }
        }
        // can be a function call, start of an assignment, or something else
        // TODO fix to check for type
        else if (match(currentPos, 1, TokenType::IDENTIFIER)) {
            // save the identifier before assignment
            Token prev = previous(currentPos);
            if (match(currentPos, 1, TokenType::EQUAL)) {
                // starting with the next token add all of them up till end of expression
                vector<Token> toks;
                // get the tokens for the expression
                uint64_t newCurrentToken = checkExpression(currentPos, toks);
                Expr *expr = new Expr(toks, this->eHandler);
                AssignNode * node = new AssignNode(prev, expr);
                nodes.push_back(node);

            }
        }
    }
    return currentPos;
}

// A simple function to detect if it is the end of the file, or a newline is found next
bool Parser::isNextNewline(uint64_t current) {
    if (isAtEnd(current + 1) || peek(current).lineNum !=  peek(current + 1).lineNum)
       return true;
    return false;
}

bool Parser::isKeyword(TokenType t) {
    switch(t) {
        case TokenType::CLASS:
        case TokenType::ELSE:
        case TokenType::IF:
        case TokenType::FUN:
        case TokenType::FOR:
        case TokenType::WHILE:
        case TokenType::RETURN:
        case TokenType::VOID:
        case TokenType::INT:
        case TokenType::FLOAT:
        case TokenType::CINT:
        case TokenType::CFLOAT:
           return true;
    }
    return false;
}

bool Parser::isBinaryOperator(TokenType t) {
    switch(t) {
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::STAR:
        case TokenType::SLASH:
        case TokenType::PERCENT:
        case TokenType::BITWISE_AND:
        case TokenType::BITWISE_OR:
        case TokenType::BITWISE_XOR:
        case TokenType::AND:
        case TokenType::OR:
        case TokenType::XOR:
            return true;
    }
    return false;
}

bool Parser::isLiteral(TokenType t) {
    switch(t) {
        case TokenType::STRING:
        case TokenType::NUMBER_DECIMAL:
        case TokenType::NUMBER_FLOAT:
        case TokenType::NUMBER_HEX:
        case TokenType::IDENTIFIER:
            return true;
    }
    return false;
}

// gives the ending token of the expression and compiles them all into a vector
// Simply ensures the expression is valid assuming the types are correct.
// DOES NOT TYPECHECK or make sure the operator is valid for a given type
uint64_t Parser::checkExpression(uint64_t startToken, vector<Token> & toks) {
    uint64_t current = startToken;
    while (!isAtEnd(current)) {
        if (isNextNewline(current)) {
            // check to see if the current token is a valid end of line, and the next one is a valid start
            TokenType t_curr = peek(current).tokenType;
            TokenType t_next = peek(current + 1).tokenType;
            // incomplete expression, but let the expr class handle that exception
            // unlear here whether to include the next token in the exception
            if (isBinaryOperator(t_curr) && !isLiteral(t_next)) {
                toks.push_back(peek(current));
                advance(current);
                break;
            }
            else if (isKeyword(t_next)) {
                toks.push_back(peek(current));
                advance(current);
                break;
            }
            else if (!isBinaryOperator(t_curr) && !isBinaryOperator(t_next)) {
                toks.push_back(peek(current));
                advance(current);
                break;
            }
        }
        toks.push_back(peek(current));
        advance(current);
    }
    return current;
}

void Parser::parse() {
    this->currentToken = 0;
    // called to assemble each function and class including main
    parseHelper(this->currentToken, this->parserNodes);
}
