#ifndef ERROR_H
#define ERROR_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<errno.h>

uint8_t error_occurred;

void syserr(const int8_t * location, int32_t errnum);

void lexerr(const int8_t * msg, int32_t lineNum, int32_t colNum);

void reporterr(uint32_t lineNum, uint32_t colNum, const int8_t * location, const int8_t * message);

void init_error();

#endif
