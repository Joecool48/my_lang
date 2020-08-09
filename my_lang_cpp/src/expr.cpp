#include "../include/expr.hpp"

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

Expr::Expr() : root(nullptr), currentToken(0) {}

void Expr::generateAST(vector<Token> & tokens) {
    this->tokens = tokens;
     
}

ExprNode *Expr::expression() {
    return equality();
}

ExprNode *Expr::equality() {
    ExprNode  *left = comparison();

    while (match(TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL)) {
        Token op = previous();
        ExprNode *right = comparison();
        
        left = new BinaryNode(left, op, right);
    }

    return left;
}

ExprNode *Expr::comparison() {
    ExprNode *left = addition();
    
    while (match(TokenType::LESS, TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS_EQUAL)) {
        Token op = previous();
        ExprNode *right = addition();
        
        left = new BinaryNode(left, op, right); 
    }

    return left;
}

ExprNode *Expr::addition() {
    ExprNode *left = multiplication();

    while (match(TokenType::PLUS, TokenType::MINUS)) {
        Token op = previous();
        ExprNode *right = multiplication();
        left = new BinaryNode(left, op, right);
    }

    return left;
}

ExprNode *Expr::multiplication() {
    ExprNode *left = unary();
    
    while (match(TokenType::STAR, TokenType::SLASH)) {
        Token op = previous();
        ExprNode *right = unary();
        left = new BinaryNode(left, op, right);
    }

    return left;
}

ExprNode *Expr::unary() {
    if (match(TokenType::BANG, TokenType::MINUS)) {
        Token op = previous();
        ExprNode *right = unary();
        return new UnaryNode(right, op);
    }

    return primary();
}

ExprNode *Expr::primary() {
    if (match(TokenType::FALSE)) return new LiteralNode(previous());
    if (match(TokenType::TRUE)) return new LiteralNode(previous());
    if (match(TokenType::VOID)) return new LiteralNode(previous()); 

    if (match(TokenType::NUMBER_DECIMAL, TokenType::NUMBER_FLOAT, TokenType::NUMBER_HEX, TokenType::IDENTIFIER, TokenType::STRING))
        return new LiteralNode(previous());

    if (match(TokenType::LEFT_PAREN)) {
        ExprNode *left = expression();

    }

}
