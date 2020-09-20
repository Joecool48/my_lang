#include"parser.hpp"

#ifndef PARSERNODE_H
#define PARSERNODE_H

enum class ParserNodeType {
    IF, FOR, WHILE, ASSIGN
};

class ParserNode {
    public: ParserNodeType type;
    uint64_t lineNum;
    uint64_t colNum;
    ParserNodeType getNodeType();  
};

class IfNode : public ParserNode {
    public:
    Expr * conditional;
    vector<ParserNode*> body;
    IfNode(Expr * conditional, vector<ParserNode*> body); 
    IfNode(Expr * conditional);
};

class WhileNode : public ParserNode {
    public:
    Expr * conditional;
    vector<ParserNode*> body;
    WhileNode(Expr * conditional, vector<ParserNode*> body);
    WhileNode(Expr * conditional);
};

class AssignNode : public ParserNode {
    public:
    Token identifier;
    Expr * expr;
    AssignNode(Token identifier, Expr * expr);
};

#endif /* PARSERNODE_H */
