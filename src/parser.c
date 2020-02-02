#include "parser.h"

void initparsing(vector_t * toks) {
    // simply copy the pointer
    tokens = toks; 
    currentToken = 0;
}

expr_t * allocateExpr(expr_t * left, token_t op, expr_t * right) {
    expr_t * newExpr = (expr_t *) malloc(sizeof(expr_t));
    if (!newExpr) {
        perror("malloc");
        exit(EXIT_FAILURE);
    }
    newExpr->left = left;
    newExpr->right = right;
    newExpr->op = op;
    return newExpr;
}

token_t peek() {
    return vector_get(tokens, currentToken);
}

token_t advance(int32_t amount) {
    if (vector_size(tokens) > currentToken + amount)
        currentToken += amount;
    return vector_get(tokens, currentToken - amount);
}

bool check(enum token tok) {
    if (vector_size(tokens) == currentToken) return false;
    return peek().token == tok;
}

bool match(int num, ...) {
    int i;
    va_list list;
    va_start(list, num);

    for (i = 0; i <= num; i++) {
        enum token tok = va_arg(list, enum token);

        if (check(tok)) {
            advance(1);
            return true;
        }
    }
    return false;
}

token_t previous() {
    return vector_get(tokens, currentToken - 1);
}

expr_t* expression() {
    return equality();
} 

expr_t* equality() {
    expr_t * expr = comparison();
    while (match(2, BANG_EQUAL, EQUAL_EQUAL)) {
        token_t oper = previous();
        expr_t * right = comparison();
        expr = allocateExpr(expr, oper, right);
    }
    return expr;
}

expr_t* comparison() {
    expr_t * expr = addition();
    
    while (match(4, GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
        token_t oper = previous();
        expr_t * right = addition();
        expr = allocateExpr(expr, oper, right); 
    }
    return expr;
}

expr_t* addition() {
    expr_t * expr = multiplication();

    while (match(2, PLUS, MINUS)) {
        token_t oper = previous();
        expr_t * right = multiplication();
        expr = allocateExpr(expr, oper, right);  
    }
    return expr;
}

expr_t* multiplication() {
    expr_t * expr = unary();

    while (match(3, STAR, SLASH, PERCENT)) {
        token_t oper = previous();
        expr_t * right = unary();
        expr = allocateExpr(expr, oper, right);
    }
    return expr;
}

// TODO unary and other parts of tree

bool isUnary(expr_t * expr) {
    return expr->right == NULL;
}
