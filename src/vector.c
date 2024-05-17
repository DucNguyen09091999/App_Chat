#include "vector.h"

const struct VectorOpsType Vector_Operations = {
    .pInit = vector_init,
    .pAdd = vector_add,
    .pFree = vector_free
};

Vector activeConnections;

void vector_init(Vector* vector, size_t initial_capacity) {
    vector->data = (tcp_socket_t*)malloc(initial_capacity * sizeof(tcp_socket_t));
    if (vector->data == NULL) {
        fprintf(stderr, "Failed to allocate memory for vector.\n");
        exit(EXIT_FAILURE);
    }
    vector->size = 0;
    vector->capacity = initial_capacity;
    printf("vector init\n");
}

void vector_add(Vector* vector, tcp_socket_t element) {
    if (vector->size == vector->capacity) {
        size_t new_capacity = vector->capacity * 2;
        tcp_socket_t* new_data = (tcp_socket_t*)realloc(vector->data, new_capacity * sizeof(tcp_socket_t));
        if (new_data == NULL) {
            fprintf(stderr, "Failed to reallocate memory for vector.\n");
            exit(EXIT_FAILURE);
        }
        vector->data = new_data;
        vector->capacity = new_capacity;
    }
    vector->data[vector->size++] = element;
    printf("vector add\n");
}

void vector_free(Vector* vector) {
    free(vector->data);
    vector->data = NULL;
    vector->size = 0;
    vector->capacity = 0;
    printf("vector free\n");
}