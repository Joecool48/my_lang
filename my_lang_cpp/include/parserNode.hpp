#include"parser.hpp"

#ifndef PARSERNODE_H
#define PARSERNODE_H

enum class ParserNodeType {
    IF, FOR, WHILE
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

#endif /* PARSERNODE_H */
