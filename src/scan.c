#include"scan.h"

void readFile(FILE * stream, int8_t * buf, size_t len) {
    if (len != fread(buf, sizeof(int8_t), len, stream)) {
        syserror(__FUNCTION__, errno);
    }
    buf[fileLen] = '\0'; // add the null terminator
}

bool isValidIdentifierChar(int8_t token) {
    return  ((token >= 'a' && token <= 'z') || (token >= 'A' && token <= 'Z') || token == '_');
}

void loadSourceFile(uint8_t * filename) {
    if (sourceFile == NULL)
        syserror(__FUNCTION__, errno);

    FILE * sourceFile = fopen(filename, "r");

    fileLen = getFileLen(sourceFile);
    sourceText = (int8_t *) malloc(sizeof(int8_t) * (fileLen + 1)); // allocate memory for source text

    readFile(sourceFile, sourceText, fileLen);
}

int32_t getFileLen(FILE * stream) {
    if (NULL == stream) return -1;
    ssize_t size;
    fseek(stream, 0L, SEEK_END);
    size = ftell(stream);
    fseek(stream, 0, SEEK_SET);
    return size;
}

// must get the letter we will get next, so subtract 1 since at pos 0 it is a new char
int8_t peek(int32_t amount) {
    if (currentPos + amount >= fileLen) return EOF;
    return sourceText[currentPos + amount];
}

bool match(int8_t * word1, int8_t * word2, size_t len) {
    for (uint32_t i = 0; i < len; i++) {
        if (word1[i] != word2[i]) return false;
    }
    return true;
}

bool isKeyword(int8_t * word, size_t len) {
    token_t tok;
    uint32_t startCol = colNum;
    uint32_t startLine = lineNum;
    switch (len) {
        // check for keywords of length 1
        case 0:
            printf("Shouldnt have keywords length 0\n");
            exit(EXIT_FAILURE);
        case 1:
            return false;
            break;
        case 2:
            if (match(word, "or", len))
                tok.token = OR;
            else if (match(word, "if", len))
                tok.token = IF;
            else
                return false; // no token keyword with that length
            advance(len);
            break;
            break;
        case 3:
            if (match(word, "and", len))
                tok.token = AND;
            else if (match(word, "for", len))
                tok.token = FOR;
            else if (match(word, "fun", len))
                tok.token = FUN;
            else if (match(word, "int", len))
                tok.token = INT;
            else if (match(word, "not", len))
                tok.token = NOT;
            else
                return false;
            advance(len);
            break;
            break;
        case 4:
            if (match(word, "else", len))
                tok.token = ELSE;
            else if (match(word, "void", len))
                tok.token = VOID;
            else if (match(word, "this", len))
                tok.token = THIS;
            else if (match(word, "true", len))
                tok.token = TRUE;
            else if (match(word, "cint", len))
                tok.token = CINT;
            else
                return false;
            advance(len);
            break;
        case 5:
            if (match(word, "class", len))
                tok.token = CLASS;
            else if (match(word, "float", len))
                tok.token = FLOAT;
            else if (match(word, "while", len))
                tok.token = WHILE;
            else
                return false;
            advance(len);
            break;
        case 6:
            if (match(word, "return", len))
                tok.token = RETURN;
            else if (match(word, "cfloat", len))
                tok.token = CFLOAT;
            else
                return false;
            advance(len);
            break;
        // otherwise it is a keyword, and we fill out the rest of the struct
        tok.lexme = NULL;
        tok.lineNum = startCol;
        tok.colNum = startLine;
        vector_add(tokens, tok);
    }
    return true;
}

// advance amount and return char at that location. If none, then return EOF. If newline then update
// line and col counters, and if not then update just position and col counter
int8_t advance(int32_t amount) {
    if (sourceText[currentPos] == '\n') {
        lineNum++;
        colNum = 0;
    }
    else {
        colNum += amount; // shouldnt overflow since calling function should check token by token
    }
    currentPos += amount;
    if (currentPos >= filenameLen) return EOF;
    return sourceText[currentPos];
}

// puts the token in the vector of tokens
void addTokenToList(token_t tok) {
    vector_push(tokens, tok);
}

// function that goes back len spaces and inserts the string into the token list
void addTokenIdentifierFromLen(uint32_t len) {
    int8_t * str = (int8_t *) malloc(sizeof(int8_t) * (len + 1));
    int8_t * strPtr = sourceText + currentPos - len;

    memcpy(str, strPtr, len);

    str[len] = '\0';
    // create token and insert it into list
    token_t tok;
    tok.token = IDENTIFIER;
    tok.lexme = str;
    tok.lineNum = lineNum;
    tok.colNum = colNum - len; // find the start of the variable
    addTokenToList(tok);
}

// takes in a token type of a operator or keyword
void addToken(uint32_t token) {
    token_t tok;
    tok.token = token;
    tok.lexme = NULL;
    tok.lineNum = lineNum;
    tok.colNum = colNum;
    addTokenToList(tok);
}

// function will repeatedly advance till end of newline for single comments, and till "*/" for multi line
void skipComment() {
    // single line, so go till reach newline
    if (peek(0) == '/' && peek(1) == '/') {
        uint32_t tempPos = currentPos;
        while (peek(0) != '\n') {
            tempPos++;
            advance(1);
            if (tempPos >= fileLen)
                return; // end of file
        }
    }
    // multi line comment. Keep going till */ is reached
    else if (peek(0) == '/' && peek(1) == '*') {
        uint32_t tempPos = currentPos;
        while (peek(0) != '*' || peek(1) != '/') {
            tempPos++;
            advance(1);
            if (tempPos >= fileLen)
                return; // end of file
        }
    }
    else {
        printf("Comment screwed up. fix %s", __FUNCTION__);
    }
}

bool isValidNonIdentifier(int8_t c) {
    switch(c) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '(':
        case ')':
        case '\"':
        case '\'':
        case '[':
        case ']':
        case '{':
        case '}':
        case '&':
        case '^':
        case '|':
        case '!':
        case '=':
        case ',':
        case '.':
        case '>':
        case '<':
        case '~':
            return true;
        default:
            return false;
    }
}

bool isValidChar(int8_t c) {
    return isValidIdentifierChar(c) || isValidNonIdentifier(c);
}

// returns the index of the position of the char starting at location
uint32_t find(int8_t c, uint32_t start) {
    uint32_t idx = start;
    while (sourceText[idx] != c) {
        idx++;
        if (idx >= fileLen)
            lexerror("Ending quotation for string not found", lineNum, colNum);
    }
    return idx;
}

bool isCharacterThatNeedsEscape(int8_t c) {
    return ('r' == c || 't' == c || 'n' == c || '\'' == c || '\"' == c || '\' == c);
}

// escapes are '\r' '\t' '\n' '\'' '\"' '\n'
int8_t * parseString(int8_t * str) {
    uint32_t nextAvailablePos = 0;
    uint32_t current = 0;

    while(str[current] != '\0') {
        // need to consume both the characters in the string if it is escaped
        if (str[current] == '\' && currentIsCharacterThatNeedsEscape(str[currentPos + 1])) {
            switch (str[current + 1]) {
                case 'r':
                    str[nextAvailablePos] = '\r';
                    break;
                case 't':
                    str[nextAvailablePos] = '\t';
                    break;
                case 'n':
                    str[nextAvailablePos] = '\n';
                    break;
                case '\'':
                    str[nextAvailablePos] = '\'';
                    break;
                case '\"':
                    str[nextAvailablePos] = '\"';
                    break;
                case '\\':
                    str[nextAvailablePos] = '\\';
                    break;
                default:
                    printf("parseString: is screwed up\n");
                    exit(EXIT_FAILURE);
            }
            current += 2;
        }
        else {
            str[nextAvailablePos] = str[current];
            current += 1;
        }
        nextAvailablePos += 1;
    }
    str[nextAvailablePos] = '\0'; // add null terminator
    // then resize the string so we dont waste extra space
    str = (int8_t *) realloc(str, sizeof(int8_t) * (nextAvailablePos + 1));
    if (NULL == str) syserror(__FUNCTION__, errno);
    return str;
}

void lexString() {
    // check if first is a single or double quote and capture everything inbetween
    int8_t stopchar = peek(0);
    if (stopchar != '\'' || && stopchar != '\"') {
        printf("Function lexString was called incorrectly\n");
        exit(EXIT_FAILURE);
    }

    uint32_t startLineNum = lineNum;
    uint32_t startColNum = colNum;

    advance(1);
    uint32_t startidx = currentPos;
    uint32_t secondcharidx = find(stopchar, currentPos);
    uint32_t strLen = secondcharidx - startidx;
    // allocate the memory to store the string
    int8_t * str = (int8_t *) malloc(strLen + 1);
    memcpy(str, sourceText + startidx, strLen);
    str[strLen] = '\0';
    // set the pos to the quote + 1
    currentPos = secondcharidx + 1;

    str = parseString(str); // takes the string, and turns it into a string free of escape characters TODO

    token_t tok;
    tok.token = STRING;
    tok.lexme = str;
    tok.lineNum = startLineNum;
    tok.colNum = startColNum;
    vector_push(tokens, tok); // add the token
}

void scanToken(int8_t token) {
    // simply ignore whitespace
    if (token == '\n') {
        addToken(NEWLINE); // newlines are needed for expression parsing
        advance(1); // advance automatically advances lineNum and colNum
    }
    if (token == ' ' || token == '\t' || token == '\r') {
        // automagically adds keyword as atoken if true
        if (currentIdentifierLen != 0 && isKeyword(sourceText + currentPos, currentIdentifierLen)) {
            currentIdentifierLen = 0;
        }
        else if (currentIdentifierLen != 0) {
            addTokenIdentifierFromLen(currentIdentifierLen); // tokenize it after retrieving it
            currentIdentifierLen = 0; // reset identifier len
        }
        return;
    }
    if (isValidIdentifierChar(token)) {
        currentIdentifierLen++;
        advance(1);
        return;
    }
    else if (currentIdentifierLen != 0) {
        addTokenIdentifierFromLen(currentIdentifierLen);
        currentIdentifierLen = 0;
    }

    switch (token) {
        case '+':
            if (peek(1) == '=') {
                addToken(PLUS_EQUALS);
                advance(2);
            }
            else {
                addToken(PLUS);
                advance(1);
            }
            break;
        case '-':
            if (peek(1) == '=') {
                addToken(MINUS_EQUALS);
                advance(2);
            }
            else {
                addToken(MINUS);
                advance(1);
            }
            break;
        case '*':
            if (peek(1) == '=') {
                addToken(STAR_EQUALS);
                advance(2);
            }
            else {
                addToken(STAR);
                advance(1);
            }
            break;
        case '/':
            if (peek(1) == '=') {
                addToken(SLASH_EQUALS);
                advance(2);
            }
            // if single or multi line comment, then skip it
            else if (peek(1) == '/' || peek(1) == '*') {
                skipComment();
            }
            else {
                addToken(SLASH);
                advance(1);
            }
            break;
        case '%':
            if (peek(1) == '=') {
                addToken(PERCENT_EQUALS);
                advance(2);
            }
            else {
                addToken(PERCENT);
                advance(1);
            }
            break;
        // check curly braces and parenthesis and brackets
        case '[':
            addToken(LEFT_BRACE);
            advance(1);
            break;
        case ']':
            addToken(RIGHT_BRACE);
            advance(1);
            break;
        case '{':
            addToken(LEFT_CURLY);
            advance(1);
            break;
        case '}':
            addToken(RIGHT_CURLY);
            advance(1);
            break;
        case '(':
            addToken(LEFT_PAREN);
            advance(1);
            break;
        case ')':
            addToken(RIGHT_PAREN);
            advance(1);
            break;
        // bitwise operators
        case '&':
            if (peek(1) == '=') {
                addToken(BITWISE_AND_EQUALS);
                advance(2);
            }
            else {
                addToken(BITWISE_AND);
                advance(1);
            }
            break;
        case '|':
            if (peek(1) == '=') {
                addToken(BITWISE_OR_EQUALS);
                advance(2);
            }
            else {
                addToken(BITWISE_OR);
                advance(1);
            }
            break;
        case '~':
            addToken(BITWISE_NOT);
            advance(1);
            break;
        case '^':
            if (peek(1) == '=') {
                addToken(BITWISE_XOR_EQUALS);
                advance(2);
            }
            else {
                addToken(BITWISE_XOR);
                advance(1);
            }
            break;
        // relational operators
        case '>':
            if (peek(1) == '=') {
                addToken(GREATER_THAN_EQUAL);
                advance(2);
            }
            else {
                addToken(GREATER_THAN);
                advance(1);
            }
            break;
        case '<':
            if (peek(1) == '=') {
                addToken(LESS_THAN_EQUAL);
                advance(2);
            }
            else {
                addToken(LESS_THAN);
                advance(1);
            }
        case '=':
            if (peek(1) == '=') {
                addToken(EQUAL_EQUAL);
                advance(2);
            }
            else {
                addToken(EQUAL);
                advance(1);
            }
            break;
        case '!':
            if (peek(1) == '=') {
                addToken(BANG_EQUAL);
                advance(2);
            }
            else {
                addToken(BANG);
                advance(1);
            }
        // add string support
        case '\'':
        case '\"':
            lexString();
            break;
        case ',':
            addToken(COMMA);
            advance(1);
        default:
            lexerror("Unexpected character: ", lineNum, colNum);
    }
}

void scanTokensInFile() {
    while ((currentToken = sourceText[currentPos])) {

        if (!isValidChar(currentToken)) {
            lexerror("Unexpected character", lineNum, colNum);
        }

        scanToken(currentToken);

        if (currentPos >= fileLen) break; // finished scanning when past file bounds
    }
}

void initScan() {
    tokens = vector_create();
    lineNum = 1; // lines start at 1
    colNum = 0; // cols start at 0
    currentPos = 0;
    if (tokens == NULL)
        syserror(__FUNCTION__, errno);
}
