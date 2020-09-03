#ifndef EXPR_HPP
#define EXPR_HPP

#include "token.hpp"
#include "error.hpp"
#include <vector>
#include <cstdarg>
#include <queue>

class ExprNode {
    public:
    ExprNode *left;
    Token op;
    ExprNode *right;
    ExprNode *nestedExpression;
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
    explicit LiteralNode(Token literal);
};

class GroupingNode : public ExprNode {
    public:
    explicit GroupingNode(ExprNode *nestedExpression);
};

class Expr {
    private:
    ExprNode *root;
    vector<Token> tokens;
    uint64_t currentToken;

    ExprNode *expression();
    ExprNode *logicalOr();
    ExprNode *logicalXor();
    ExprNode *logicalAnd();
    ExprNode *bitwiseOr();
    ExprNode *bitwiseXor();
    ExprNode *bitwiseAnd();
    ExprNode *equality();
    ExprNode *comparison();
    ExprNode *addition();
    ExprNode *multiplication();
    ExprNode *unary();
    ExprNode *primary();
    
    bool match(int num, ...);
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

    void dumpASTHelper(ExprNode * node);    
    ExprNode* pruneASTHelper(ExprNode * node);
    bool isBinaryOperator(const Token tok);
    bool isLiteral(const Token tok); 
    bool isGrouping(ExprNode * node);

    ExprNode* condense(ExprNode *node);
    Token operate(const Token & op, const Token & left, const Token & right);
    
    public:
    Expr();
    void generateAST(vector<Token> & tokens);
    void pruneAST();
    void dumpAST();
    void levelDumpAST();
};

#endif
