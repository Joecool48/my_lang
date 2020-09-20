#include"../include/parserNode.hpp"

ParserNodeType ParserNode::getNodeType() {
    return this->type;
}

IfNode::IfNode(Expr * conditional, vector<ParserNode*> body) {
    this->type = ParserNodeType::IF;
    
    this->conditional = conditional;
    this->body = body; 
}

IfNode::IfNode(Expr * conditional) {
    this->type = ParserNodeType::IF;
    this->conditional = conditional;
}

WhileNode::WhileNode(Expr * conditional) {
    this->type = ParserNodeType::WHILE;
    this->conditional = conditional;
}

WhileNode::WhileNode(Expr * conditional, vector<ParserNode*> body) {
    this->type = ParserNodeType::WHILE;

    this->conditional = conditional;
    this->body = body;
}

AssignNode::AssignNode(Token identifier, Expr * expr) {
    this->type = ParserNodeType::ASSIGN;
    this->identifier = identifier;
    this->expr = expr;
}
