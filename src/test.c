#include"scan.h"

void dumpTokens() {
    int i;
    for (i = 0; i < vector_size(tokens); i++) {
        token_t tok = vector_get(tokens, i);
        switch (tok.token) {
            case LEFT_PAREN:
                printf("(");
                break;
            case RIGHT_PAREN:
                printf(")");
                break;
            case COMMA:
                printf(",");
                break;
            case DOT:
                printf(".");
                break;
            case SEMICOLON:
                printf(";");
                break;
            case COLON:
                printf(":");
                break;
            case LEFT_CURLY:
                printf("{");
                break;
            case RIGHT_CURLY:
                printf("}");
                break;
            case LEFT_BRACE:
                printf("[");
                break;
            case RIGHT_BRACE:
                printf("]");
                break;
            case STAR:
                printf("*");
                break;
            case SLASH:
                printf("/");
                break;
            case PLUS:
                printf("+");
                break;
            case MINUS:
                printf("-");
                break;
            case PERCENT:
                printf("%%");
                break;
            case BITWISE_AND:
                printf("&");
                break;
            case BITWISE_OR:
                printf("|");
                break;
            case BITWISE_NOT:
                printf("~");
                break;
            case PLUS_EQUAL:
                printf("+=");
                break;
            case MINUS_EQUAL:
                printf("-=");
                break;
            case STAR_EQUAL:
                printf("*=");
                break;
            case SLASH_EQUAL:
                printf("/=");
                break;
            case PERCENT_EQUAL:
                printf("%%=");
                break;
            case BITWISE_AND_EQUAL:
                printf("&=");
                break;
            case BITWISE_OR_EQUAL:
                printf("|=");
                break;
            case BITWISE_XOR_EQUAL:
                printf("^=");
                break;
            case BITWISE_XOR:
                printf("^");
                break;
            case BANG:
                printf("!");
                break;
            case BANG_EQUAL:
                printf("!=");
                break;
            case EQUAL:
                printf("=");
                break;
            case EQUAL_EQUAL:
                printf("==");
                break;
            case GREATER:
                printf(">");
                break;
            case GREATER_EQUAL:
                printf(">=");
                break;
            case LESS:
                printf("<");
                break;
            case LESS_EQUAL:
                printf("<=");
                break;

            // now keywords
            case AND:
                printf("and");
                break;
            case CLASS:
                printf("class");
                break;
            case ELSE:
                printf("else");
                break;
            case FALSE:
                printf("false");
                break;
            case FUN:
                printf("fun");
                break;
            case FOR:
                printf("for");
                break;
            case IF:
                printf("if");
                break;
            case VOID:
                printf("void");
                break;
            case OR:
                printf("or");
                break;
            case RETURN:
                printf("return");
                break;
            case THIS:
                printf("this");
                break;
            case TRUE:
                printf("true");
                break;
            case INT:
                printf("int");
                break;
            case FLOAT:
                printf("float");
                break;
            case CINT:
                printf("cint");
                break;
            case CFLOAT:
                printf("cfloat");
                break;
            case WHILE:
                printf("while");
                break;
            case NOT:
                printf("not");
                break;
            case XOR:
                printf("xor");
                break;
            case END_FILE:
                printf("<EOF>");
                break;
            case NEWLINE:
                printf("<NEWLINE>");
                break;

            case IDENTIFIER:
            case NUMBER:
            case STRING:
                printf("%s", tok.lexme); // all other ones should have lexme strs
                break;
            default:
                printf("Token shouldnt exist...");
                break;
        }
        printf("\n");
    }
}

int main() {
    scanFile("test.lang");
    printf("Num tokens: %d\n", vector_size(tokens));
    dumpTokens();
}
