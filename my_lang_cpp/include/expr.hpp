#ifndef EXPR_HPP
#define EXPR_HPP

#include "token.hpp"
#include "error.hpp"
#include <vector>
#include <cstdarg>


class ExprNode {
    public:
    ExprNode *left;
    Token op;
    ExprNode *right;
    ExprNode();
};

class BinaryNode : public ExprNode {
    public:
    BinaryNode(ExprNode *left, Token & op, ExprNode *right);
};

class UnaryNode : public ExprNode {
    public:
    UnaryNode(ExprNode *left, Token & op); 
};

class LiteralNode : public ExprNode {
    public:
    Token literal;
    explicit LiteralNode(Token literal);
};

class GroupingNode : public ExprNode {
    public:
    ExprNode *nestedExpression;
    explicit GroupingNode(ExprNode *nestedExpression);
};

class Expr {
    private:
    ExprNode *root;
    vector<Token> tokens;
    uint64_t currentToken;

    ExprNode *expression();
    ExprNode *equality();
    ExprNode *comparison();
    ExprNode *addition();
    ExprNode *multiplication();
    ExprNode *unary();
    ExprNode *primary();
    
    bool match(uint64_t num, TokenType types...);
    Token previous();
    Token peek();
    Token advance();
    bool check(TokenType type);
    Token consume(TokenType type, const Token & tok, ErrorType errorType);

    uint64_t getLineNum();
    uint64_t getColNum();
   
    uint64_t getLineNum(const Token & tok);
    uint64_t getColNum(const Token & tok); 

    bool isAtEnd();

    public:
    Expr();
    void generateAST(vector<Token> & tokens);
    

};

#endif
