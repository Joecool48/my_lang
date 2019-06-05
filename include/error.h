#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdint.h>
#include<errno.h>

uint8_t error_occurred;

void syserr(int8_t * location, int32_t errnum);

void lexerror(int8_t * msg, int32_t lineNum, int32_t colNum);

void report_error(uint32_t lineNum, uint32_t colNum, int8_t * location, int8_t * message);

void init_error();
