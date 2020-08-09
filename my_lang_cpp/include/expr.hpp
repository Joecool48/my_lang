#include "token.hpp"
#include <vector>

class ExprNode {
    public:
    ExprNode *left;
    Token op;
    ExprNode *right;
    int i;
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
    LiteralNode(Token literal);
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
    
    bool match(TokenType...);
    Token previous();
    Token peek();
    Token advance(); 
    public:
    Expr();
    void generateAST(vector<Token> & tokens);
    

};

