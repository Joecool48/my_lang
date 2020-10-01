#ifndef TYPE_HPP
#define TYPE_HPP

#include<string>
#include"error.hpp"

using namespace std;

class Variable {
    Type *type;
    string name;
    uint64_t lineDeclared;
    uint64_t colDeclared;
    Variable(Type *type, string name, uint64_t line, uint64_t col);
    // Qualifiers TODO added later here
};

class Parameter {
    Type *type;
    string name;
};

class Function {
    Type *ret_type;
    string name;
    // Qualifiers TODO added later here
    vector<Parameter> parameters;
};

class Type {
private:
    string name;
    // map for ease of access by name
    unordered_map<string, Variable> variables;
    unordered_map<string, Function> functions;
public:
    Type(string name);
    string getName();
    Type* getMemberByName(string name);
};

#endif /* TYPE_HPP */
