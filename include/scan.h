#include<stdio.h>
#include<stdlib.h>
#include<errno.h>
#include"vector.h"
#include"error.h"

#include "token.h"

const uint32_t MAX_TOKEN_SIZE = 100;
const int8_t EOF = -1;

typedef int error_t;

FILE * sourceFile;
int8_t * sourceText;
ssize_t fileLen;
vector_t * tokens;

uint32_t lineNum;
uint32_t colNum;
uint32_t currentPos;
uint32_t currentIdentifierLen; // for keeping track of name string lengths
bool isNumber; // lets the current scanner know if it is parsing a number or not

void initScan();
void loadSourceFile(int8_t * filename);
void scanTokensInFile();
void scanToken(int8_t token);
void lexString();
int8_t * parseString(int8_t * str);
bool isCharacterThatNeedsEscape(int8_t c);
uint32_t find(int8_t c, uint32_t start);
bool isValidChar(int8_t c);
bool isValidNonIdentifier(int8_t c);
void skipComment();
void addToken(uint32_t token);
void addTokenIdentifierFromLen(uint32_t len);
void addTokenToList(token_t tok);
int8_t advance(int32_t amount);
int8_t peek(int32_t amount);
int32_t getFileLen(FILE * stream);
void loadSourceFile(uint8_t * filename);
bool isValidIdentifierChar(int8_t token);
void readFile(FILE * stream, int8_t * buf, size_t len);
bool isKeyword(int8_t * word, size_t len);
bool match(int8_t * word1, int8_t * word2, size_t len);
