#include"../include/preParser.hpp"

PreParser::PreParser() {
    currentToken = 0;
    init();
}

PreParser::PreParser(ErrorHandler * e, const vector<Token> & tokens) {
    this->tokens = tokens;
    this->currentToken = 0;
    this->eHandler = e;
    init();
}

void PreParser::addTokens(ErrorHandler * e, const vector<Token> & tokens) {
    this->tokens = tokens;
    this->currentToken = 0;
    this->eHandler = e;
    init();
}

bool PreParser::matchPrimative() {
    return match(5, TokenType::FLOAT, TokenType::INT, TokenType::CHAR, TokenType::BOOL, TokenType:STRING);
}

void init() {
    // add all the primitives to the type list
    this->completeTypes["int"] = new Type("int");
    this->completeTypes["float"] = new Type("float");
    this->completeTypes["char"] = new Type("char");
    this->completeTypes["bool"] = new Type("bool");
    this->completeTypes["str"] = new Type("str");
}

// returns the already declared type if the type is complete, otherwise nullptr
Type* PreParser::trackType(const Token & type) {
    if (type.tokenType == TokenType::IDENTIFIER) {
        // if not already in the map, then add it
        if (completeTypes.find(type.lexme) != completeTypes.end())
            return completeTypes[type.lexme];
        else
            incompleteTypes[type.lexme] = new Type(type.lexme);
            return incompleteTypes[types.lexme];
    }
    return nullptr;
}

bool PreParser::alreadyDeclared(Type * type, string name) {
    if (type->variables.find(name) != type->variables.end()) {
        this->eHandler->reportError(Error(
            ErrorType::AlreadyDeclaredException,
            name + " was already declared as a variable",
            type->variables[name].lineDeclared,
            type->variables[name].colDeclared
        ));
        return true;
    }
    else if (type->functions.find(name) != type->functions.end()) {
        this->eHandler->reportError(Error(
            ErrorType::AlreadyDeclaredException,
            name + " was already declared as a function",
            type->functions[name].lineDeclared,
            type->functions[name].colDeclared
        ));
        return true;
    }
    return false;
}

void PreParser::gatherMembers(Type *createdType) {
    // all members are either declarations, or function declarations
    // anything else is a reject and not allowed
    // declarations are defined as <qualifiers> <type> <identifier>
    // or as <qualifiers> <type> <identifier> <=> <value>

    // functions are defined as <qualifiers> <fun>
    // <return type (not needed, but good idea to have)>
    // <identifier> <(> <parameters seperated by ,> <)>
    stack<bool> curlyStack; // push bool on when "{" and off when "}"

    // qualifiers will be added later TODO

    while(!isAtEnd() && !curlyStack.empty()) {
        // should start by assuming the type is declared somewhere.
        // if we find out later it isnt, then we can throw an exception
        if (matchPrimative() || match(1, TokenType::IDENTIFIER)) {
            // assume identifier is a type
            Token newType = previous();
            Type* type = trackType(newType);
            if (!match(1, TokenType::IDENTIFIER)) {
                this->eHandler->reportError(Error(
                    ErrorType::ExpectedIdentifierException,
                    newType.lexme,
                    getCurrentLineNum(),
                    getCurrentColNum()
                ));
            }
            string name = previous().lexme;
            // assignment can happen later when we add constant and static qualifiers
            // we have a valid variable
            // check for double declaration which is not allowed
            if (!alreadyDeclared(type, name))
                createdType->variables[name] = Variable(newType, name, newType.lineNum, newType.colNum);
        }
        // must be a function
        else if (match(1, TokenType::FUN)) {
            // if there is only 1 identifier next, then it must be the name
            Type* retType = nullptr;
            if (matchPrimative()) {
                retType = this->completeTypes[previous().toString()];
                if (!match(1, TokenType::IDENTIFIER)) {

                }
            }
        }
    }
}

void PreParser::preParse() {
    // look for the keywords fun and class to check out all the declared functions
    // and types in the file
    while (!isAtEnd()) {
        // class detection without current support for inheritence
        if (match(1, TokenType::CLASS)) {
            if (!match(1, TokenType::IDENTIFIER)) {
                this->eHandler->reportError(
                    Error(ErrorType::ExpectedIdentifierException,
                        "keyword class", getCurrentLineNum(), getCurrentColNum()));
            }
            string newName = previous()->lexme;
            // would add inheritence here TODO
            if (!match(1, TokenType::LEFT_CURLY)) {
                this->eHandler->reportError(
                    Error(ErrorType::ExpectedCurlyException,
                        newName, getCurrentLineNum(), getCurrentColNum());
                );
            }
            Type *newType = new Type(newName);
            gatherMembers(newType);
        }
    }
}
