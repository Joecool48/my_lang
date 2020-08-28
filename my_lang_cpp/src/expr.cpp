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

LiteralNode::LiteralNode(Token literal) {
    this->op = literal;
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

bool Expr::match(int num, ...) {
    va_list args;
    va_start(args, num);

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
    va_end(args);
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
    if (currentToken >= tokens.size())
        return Token();
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
    return logicalOr();
}

ExprNode *Expr::logicalOr() {
    ExprNode *left = logicalXor();

    while (match(1, TokenType::OR)) {
        cout << "Found logical or" << endl;
        Token op = previous();
        ExprNode *right = logicalXor();

        left = new BinaryNode(left, op, right);
    }

    return left;
}

ExprNode *Expr::logicalXor() {
    ExprNode *left = logicalAnd();

    while (match(1, TokenType::XOR)) {
        Token op = previous();
        ExprNode *right = logicalAnd();

        left = new BinaryNode(left, op, right);
    }

    return left;
}

ExprNode *Expr::logicalAnd() {
    ExprNode *left = bitwiseOr();

    while (match(1, TokenType::AND)) {
        Token op = previous();
        ExprNode *right = bitwiseOr();

        left = new BinaryNode(left, op, right);
    }

    return left;
}

ExprNode *Expr::bitwiseOr() {
    ExprNode *left = bitwiseXor();

    while (match(1, TokenType::BITWISE_OR)) {
        Token op = previous();
        ExprNode *right = bitwiseXor();

        left = new BinaryNode(left, op, right);
    }

    return left;
}

ExprNode *Expr::bitwiseXor() {
    ExprNode *left = bitwiseAnd();

    while (match(1, TokenType::BITWISE_XOR)) {
        Token op = previous();
        ExprNode *right = bitwiseAnd();

        left = new BinaryNode(left, op, right);
    }

    return left;
}

ExprNode *Expr::bitwiseAnd() {
    ExprNode *left = equality();

    while (match(1, TokenType::BITWISE_AND)) {
        Token op = previous();
        ExprNode *right = equality();

        left = new BinaryNode(left, op, right);
    }

    return left;
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
    if (match(3, TokenType::BANG, TokenType::MINUS, TokenType::BITWISE_NOT)) {
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

    if (match(5, TokenType::NUMBER_FLOAT, TokenType::NUMBER_DECIMAL, TokenType::NUMBER_HEX, TokenType::IDENTIFIER, TokenType::STRING))
        return new LiteralNode(previous());

    if (match(1, TokenType::LEFT_PAREN)) {
        ExprNode *left = expression();
        consume(TokenType::RIGHT_PAREN, peek(), ErrorType::MissingParenException); // TODO
        return new GroupingNode(left);
    }
    cout << peek() << endl;
    // Should never get here
    return nullptr;
}

void Expr::dumpASTHelper(ExprNode * node) {
    if (node == nullptr) return;
    if (node->left != nullptr)
        dumpASTHelper(node->left);
    cout << node->op << endl;
    if (node->right != nullptr)
        dumpASTHelper(node->right);
}

void Expr::levelDumpAST() {
    queue<ExprNode*> q;

    q.push(root);

    while (!q.empty()) {
        q.push(nullptr);
        ExprNode *current = q.front(); 
        while(current != nullptr) {
            cout << q.front()->op << " ";
            if (q.front()->left != nullptr) q.push(q.front()->left);
            if (q.front()->right != nullptr) q.push(q.front()->right);
            q.pop();
            current = q.front();
        }
        q.pop(); // pop the null
        cout << endl;
    }
}

bool Expr::isBinaryOperator(const Token tok) {
    switch (tok.tokenType) {
        case TokenType::STAR:
        case TokenType::SLASH:
        case TokenType::PLUS:
        case TokenType::MINUS:
        case TokenType::PERCENT:
        case TokenType::BITWISE_AND:
        case TokenType::BITWISE_OR:
        case TokenType::BITWISE_XOR:
        case TokenType::BANG_EQUAL:
        case TokenType::EQUAL_EQUAL:
        case TokenType::GREATER:
        case TokenType::LESS:
        case TokenType::LESS_EQUAL:
        case TokenType::GREATER_EQUAL:
        case TokenType::AND:
        case TokenType::OR:
        case TokenType::XOR:
            return true;
        default:
            return false;
    }
}

bool Expr::isLiteral(const Token tok) {
    switch (tok.tokenType) {
        case TokenType::STRING:
        case TokenType::NUMBER_DECIMAL:
        case TokenType::NUMBER_FLOAT:
        case TokenType::NUMBER_HEX:
        case TokenType::FALSE:
        case TokenType::TRUE:
        case TokenType::VOID:
            return true;
        default:
            return false;
    }
    // Note: void is also an optionally specified return type for functions. It also is a value that a object
    // reference can take
}

// Determines if an operation between two child nodes can take place, and creates a new node representing the result
// If the operation is impossible, then throw an exception
Token Expr::operate(const Token & op, const Token & left, const Token & right) {
    string newLexme;

    if (left.tokenType == TokenType::NUMBER_DECIMAL && right.tokenType == TokenType::NUMBER_DECIMAL) {
        // TODO may need to ensure that the lexme is properly converted
        long leftNum = atol(left.lexme.c_str());
        long rightNum = atol(right.lexme.c_str());

        switch (op.tokenType) {
            case TokenType::PLUS:
                newLexme = to_string((long)(leftNum + rightNum));
                break;
            case TokenType::MINUS:
                newLexme = to_string((long)(leftNum - rightNum));
                break;
            case TokenType::STAR:
                newLexme = to_string((long)(leftNum * rightNum));
                break;
            case TokenType::SLASH:
                newLexme = to_string((long)(leftNum / rightNum));
                break;
                // unsure if the % should behave differently here
            case TokenType::PERCENT:
                newLexme = to_string((long)(leftNum % rightNum));
                break;
            case TokenType::BITWISE_AND:
                newLexme = to_string((long)(leftNum & rightNum));
                break;
            case TokenType::BITWISE_OR:
                newLexme = to_string((long)(leftNum | rightNum));
                break;
            case TokenType::BITWISE_XOR:
                newLexme = to_string((long)(leftNum ^ rightNum));
                break;
            case TokenType::AND:
                newLexme = to_string((long)(leftNum && rightNum));
                break;
            case TokenType::OR:
                newLexme = to_string((long)(leftNum || rightNum));
                break;
            case TokenType::XOR:
                newLexme = to_string((long)((leftNum != 0 && rightNum == 0) || (leftNum == 0 && rightNum != 0) ? 1 : 0));
                break;
            case TokenType::EQUAL_EQUAL:
                newLexme = to_string((long)(leftNum == rightNum));
                break;
            case TokenType::BANG_EQUAL:
                newLexme = to_string((long)(leftNum != rightNum));
                break;
            case TokenType::LESS:
                newLexme = to_string((long)(leftNum < rightNum));
                break;
            case TokenType::LESS_EQUAL:
                newLexme = to_string((long)(leftNum <= rightNum));
                break;
            case TokenType::GREATER:
                newLexme = to_string((long)(leftNum > rightNum));
                break;
            case TokenType::GREATER_EQUAL:
                newLexme = to_string((long)(leftNum >= rightNum));
                break;
            default:
                // TODO insert real test exception here
                cout << "Uh oh this operator shouldnt be here" << endl;
                return Token();
        }
        return Token(TokenType::NUMBER_DECIMAL, left.lineNum, left.colNum, newLexme);
    }
    // implicit conversion from int to float
    else if ((left.tokenType == TokenType::NUMBER_FLOAT && right.tokenType == TokenType::NUMBER_FLOAT) ||
            (left.tokenType == TokenType::NUMBER_DECIMAL && right.tokenType == TokenType::NUMBER_FLOAT) ||
            (left.tokenType == TokenType::NUMBER_FLOAT && right.tokenType == TokenType::NUMBER_DECIMAL)) {
        double leftNum = stod(left.lexme.c_str());
        double rightNum = stod(right.lexme.c_str());

        switch (op.tokenType) {
            case TokenType::PLUS:
                newLexme = to_string((double)(leftNum + rightNum));
                break;
            case TokenType::MINUS:
                newLexme = to_string((double)(leftNum - rightNum));
                break;
            case TokenType::STAR:
                newLexme = to_string((double)(leftNum * rightNum));
                break;
            case TokenType::SLASH:
                newLexme = to_string((double)(leftNum / rightNum));
                break;
            case TokenType::AND:
                newLexme = to_string((long)(leftNum && rightNum));
                break;
            case TokenType::OR:
                newLexme = to_string((long)(leftNum || rightNum));
                break;
            case TokenType::XOR:
                newLexme = to_string((long)((leftNum != 0 && rightNum == 0) || (leftNum == 0 && rightNum != 0) ? 1 : 0));
                break;
            case TokenType::EQUAL_EQUAL:
                newLexme = to_string((long)(leftNum == rightNum));
                break;
            case TokenType::BANG_EQUAL:
                newLexme = to_string((long)(leftNum != rightNum));
                break;
            case TokenType::LESS:
                newLexme = to_string((long)(leftNum < rightNum));
                break;
            case TokenType::LESS_EQUAL:
                newLexme = to_string((long)(leftNum <= rightNum));
                break;
            case TokenType::GREATER:
                newLexme = to_string((long)(leftNum > rightNum));
                break;
            case TokenType::GREATER_EQUAL:
                newLexme = to_string((long)(leftNum >= rightNum));
                break;
            default:
                // TODO insert real test exception here
                cout << "Uh oh this operator shouldnt be here" << endl;
                return Token();
        }
        return Token(TokenType::NUMBER_FLOAT, left.lineNum, left.colNum, newLexme);
    }
    else if (left.tokenType == TokenType::STRING && right.tokenType == TokenType::STRING) {
        int c = left.lexme.compare(right.lexme);

        switch (op.tokenType) {
            case TokenType::PLUS:
                // TODO unclear how to calculate the end length. Should it be right most end, or ignored
                return Token(TokenType::STRING, left.lineNum, left.colNum, right.endLineNum, right.endColNum, 
                        left.lexme + right.lexme);
            case TokenType::EQUAL_EQUAL:
                return Token(TokenType::NUMBER_DECIMAL, left.lineNum, left.colNum, to_string((long)(left.lexme == right.lexme)));
            case TokenType::BANG_EQUAL:
                return Token(TokenType::NUMBER_DECIMAL, left.lineNum, left.colNum, to_string((long)(left.lexme != right.lexme)));
            case TokenType::LESS:
                return Token(TokenType::NUMBER_DECIMAL, left.lineNum, left.colNum, to_string((long)(c < 0)));
            case TokenType::LESS_EQUAL:
                return Token(TokenType::NUMBER_DECIMAL, left.lineNum, left.colNum, to_string((long)(c <= 0)));
            case TokenType::GREATER:
                return Token(TokenType::NUMBER_DECIMAL, left.lineNum, left.colNum, to_string((long)(c > 0)));
            case TokenType::GREATER_EQUAL:
                return Token(TokenType::NUMBER_DECIMAL, left.lineNum, left.colNum, to_string((long)(c >= 0)));
            default:
                cout << "Uh oh this operator shouldnt be here" << endl;
                return Token();
        }
    }
    else if ((left.tokenType == TokenType::STRING && right.tokenType == TokenType::NUMBER_DECIMAL) || (left.tokenType == TokenType::NUMBER_DECIMAL && right.tokenType == TokenType::STRING)) {
        stringstream str;
        long num; 
        switch (op.tokenType) {
            case TokenType::STAR:
                num = left.tokenType == TokenType::NUMBER_DECIMAL ? atol(left.lexme.c_str()) : atol(right.lexme.c_str());
                if (num < 0) {
                    cout << "Cant have a negative number" << endl;
                }
                else {
                    string s = left.tokenType == TokenType::STRING ? left.lexme : right.lexme;
                    for (long i = 0; i < num; i++) {
                        str << s;
                    }
                }
                return Token(TokenType::STRING, left.lineNum, left.colNum, str.str());
            default:
                cout << "Uh oh this operator shouldnt be here" << endl;
                return Token();
        }

    }
    return Token();
}

ExprNode* Expr::condense(ExprNode *node) {
    if (node->left != nullptr && node->right != nullptr && isLiteral(node->left->op) && isLiteral(node->right->op)) {
        Token newTok = operate(node->op, node->left->op, node->right->op);
        ExprNode *newNode = new LiteralNode(newTok);
        delete node->left;
        delete node->right;
        delete node;
        return newNode; 
    }
    return nullptr;
}

void Expr::dumpAST() {
    dumpASTHelper(root);  
}

ExprNode* Expr::pruneASTHelper(ExprNode *node) {
    if (node == nullptr) return nullptr;
    if (node->left != nullptr)
        node->left = pruneASTHelper(node->left);
    if (node->right != nullptr)
        node->right = pruneASTHelper(node->right);

    if (isBinaryOperator(node->op) && node->left != nullptr && node->right != nullptr) {
        ExprNode *newNode = condense(node);
        if (newNode != nullptr) {
            return newNode; 
        }
    }

    return node;
}

void Expr::pruneAST() {
    // reduce the AST to its minimal form (i.e 6 + 6 should be reduced to a singular 12 node)
    pruneASTHelper(root); 
}

uint64_t Expr::getLineNum() {
    return peek().lineNum + 1;
}

uint64_t Expr::getColNum() {
    return peek().colNum + 1;
}
