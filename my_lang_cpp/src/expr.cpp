#include "../include/expr.hpp"

ExprNode::ExprNode() : left(nullptr), right(nullptr) {}

BinaryNode::BinaryNode(ExprNode *left, Token & op, ExprNode *right) {
    this->left = left;
    this->op = op;
    this->right = right;
}

UnaryNode::UnaryNode(ExprNode *left, Token & op) {
    this->left = left;
    this->op = op;
    this->right = nullptr;
}

LiteralNode::LiteralNode(Token literal) : literal(literal) {
    this->left = nullptr;
    this->right = nullptr;
}

GroupingNode::GroupingNode(ExprNode *nestedExpression) : nestedExpression(nestedExpression) {}

Expr::Expr() : root(nullptr), currentToken(0) {}

void Expr::generateAST(vector<Token> & tokens) {
    this->tokens = tokens;
    this->currentToken = 0; 
    this->root = expression();
}

bool Expr::match(uint64_t num, TokenType types...) {
    va_list args;
    va_start(args, types);

    uint64_t i = 0;
    while (i < num) {
        TokenType t = va_arg(args, TokenType);
        if (peek().tokenType == t) {
            advance();
            va_end(args); 
            return true;
        }
        i++;
    }
    return false;
     
}

uint64_t Expr::getLineNum(const Token & tok) {
    return tok.lineNum + 1;
}

uint64_t Expr::getColNum(const Token & tok) {
    return tok.colNum + 1;
}

bool Expr::isAtEnd() {
    return currentToken >= tokens.size();
}

Token Expr::previous() {
    return tokens[currentToken - 1];
}

Token Expr::peek() {
    return tokens[currentToken];
}

Token Expr::advance() {
    if (!isAtEnd())
        currentToken++;
    return previous();
}

bool Expr::check(TokenType type) {
    if (isAtEnd()) return false;

    return peek().tokenType == type;
}

Token Expr::consume(TokenType tokenType, const Token & tok, ErrorType errorType) {
    if (check(tokenType)) return advance();

    throw Error(errorType, getLineNum(tok), getColNum(tok));
}

ExprNode *Expr::expression() {
    return equality();
}

ExprNode *Expr::equality() {
    ExprNode *left = comparison();

    while (match(2, TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
        Token op = previous();
        ExprNode *right = comparison();
        
        left = new BinaryNode(left, op, right);
    }

    return left;
}

ExprNode *Expr::comparison() {
    ExprNode *left = addition();
    
    while (match(4, TokenType::LESS, TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS_EQUAL)) {
        Token op = previous();
        ExprNode *right = addition();
        
        left = new BinaryNode(left, op, right); 
    }

    return left;
}

ExprNode *Expr::addition() {
    ExprNode *left = multiplication();

    while (match(2, TokenType::PLUS, TokenType::MINUS)) {
        Token op = previous();
        ExprNode *right = multiplication();
        left = new BinaryNode(left, op, right);
    }

    return left;
}

ExprNode *Expr::multiplication() {
    ExprNode *left = unary();
    
    while (match(2, TokenType::STAR, TokenType::SLASH)) {
        Token op = previous();
        ExprNode *right = unary();
        left = new BinaryNode(left, op, right);
    }

    return left;
}

ExprNode *Expr::unary() {
    if (match(2, TokenType::BANG, TokenType::MINUS)) {
        Token op = previous();
        ExprNode *right = unary();
        return new UnaryNode(right, op);
    }

    return primary();
}

ExprNode *Expr::primary() {
    if (match(1, TokenType::FALSE)) return new LiteralNode(previous());
    if (match(1, TokenType::TRUE)) return new LiteralNode(previous());
    if (match(1, TokenType::VOID)) return new LiteralNode(previous()); 

    if (match(5, TokenType::NUMBER_DECIMAL, TokenType::NUMBER_FLOAT, TokenType::NUMBER_HEX, TokenType::IDENTIFIER, TokenType::STRING))
        return new LiteralNode(previous());

    if (match(1, TokenType::LEFT_PAREN)) {
        ExprNode *left = expression();
        consume(TokenType::RIGHT_PAREN, peek(), ErrorType::MissingParenException); // TODO
        return new GroupingNode(left);
    }
    // Should never get here
    return nullptr;
}

uint64_t Expr::getLineNum() {
    return peek().lineNum + 1;
}

uint64_t Expr::getColNum() {
    return peek().colNum + 1;
}
