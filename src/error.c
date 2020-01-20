#include"error.h"

void init_error() {
    error_occurred = 0;
}

void syserr(const int8_t * location, int32_t errnum) {
    // sys error occurred while compiling
    printf("%s: %s\n", location, strerror(errnum));
    exit(errnum);
}

void lexerr(const int8_t * msg, int32_t lineNum, int32_t colNum) {
    printf("Lex error at %d:%d - %s\n", lineNum, colNum, msg);
    error_occurred = 1;
}

void reporterr(uint32_t lineNum, uint32_t colNum, const int8_t * location, const int8_t * message) {
    printf("Error:\n[%s:%d:%d] %s\n", location, lineNum, colNum, message);
    error_occurred = 1;
}
