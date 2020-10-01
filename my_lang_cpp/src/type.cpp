#include "../include/type.hpp"

Variable::Variable(Type *type, string name, uint64_t line, uint64_t col) : type(type), name(name), lineDeclared(line), colDeclared(col) {}

string Type::getName() {
    return this->name;
}

Type::Type(string name) {
    this->name = name;
}
