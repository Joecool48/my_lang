#ifndef PARSER_H
#define PARSER_H

#include"vector.h"
#include<stdarg.h>

vector_t * tokens;
int currentToken;

void initParsing(vector_t * toks);


// Expression class. If right is null then it is unary
typedef struct expr {
    expr *  left;
    token_t op;
    expr * right;
} expr_t;

expr_t * allocateExpr(expr_t * left, token_t op, expr_t * right);

expr_t * expression();
expr_t * equaltiy();
expr_t * comparison();
expr_t * addition();
expr_t * multiplication();

token_t peek();

token_t advance(int32_t amount);

token_t previous();

bool check(uint32_t token);

bool isUnary(expr_t * expr);

bool match(int num, ...);

#endif
