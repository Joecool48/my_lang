#include"vector.h"

// mallocs memory for a vector and allocates it to the struct
vector_t * vector_create() {
    vector_t * vec = (vector_t *) malloc(sizeof(vector_t));
    vec->capacity = 1;
    vec->size = 0;
    vec->arr = malloc(sizeof(elem_t *) * vec->capacity);
    return vec;
}

void vector_resize(vector_t * vec) {
    // create an array double the size
    elem_t * newArr = (elem_t *) malloc(vec->capacity * sizeof(elem_t) * 2);
    // copy everything
    memcpy(newArr, vec->arr, vec->size * sizeof(elem_t));

    // free old array and replaces
    free(vec->arr);
    vec->arr = newArr;

    vec->capacity *= 2;
}

// adds an element to the back of the vector
void vector_push(vector_t * vec, elem_t c) {
    // need to resize
    if (vec->size + 1 >= vec->capacity)
        vector_resize(vec);
    vec->arr[vec->size] = c;
    vec->size += 1;
}

elem_t vector_pop(vector_t * vec) {
    vec->size -= 1;
    return vec->arr[vec->size];
}

elem_t vector_get(vector_t * vec, uint32_t idx) {
    return vec->arr[idx];
}

int vector_size(vector_t * vec) {
    return vec->size;
}

void vector_free(vector_t * vec) {
    if (vec->arr != NULL) {
        free(vec->arr);
    }
    free(vec);
}
