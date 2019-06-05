#include"vector.h"

int main() {
    vector_t * vec = vector_create();
    vector_push(vec, 'c');
    vector_push(vec, 'h');
    vector_push(vec, 'd');
    vector_push(vec, 'l');
    for (uint32_t i = 0; i < 4; i++) {
        printf("%c\n", vector_get(vec, i));
    }
    for(uint32_t i = 0; i < 4; i++) {
        printf("%c\n", vector_pop(vec));
    }
}
