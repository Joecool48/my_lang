#ifndef VECTOR_H
#define VECTOR_H

#include<stdio.h>
#include<stdlib.h>
#include<stdint.h>
#include<string.h>
#include"tokens.h"

typedef token_t elem_t;

typedef struct vector_t {
    size_t capacity;
    size_t size;
    elem_t * arr;
} vector_t;


vector_t * vector_create();

void vector_push(vector_t * vec, elem_t c);

elem_t vector_pop(vector_t * vec);

elem_t vector_get(vector_t * vec, uint32_t idx);

void vector_free(vector_t * vec);

int vector_size(vector_t * vec);

#endif
