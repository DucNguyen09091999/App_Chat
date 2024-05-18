#include "vector.h"

const struct VectorOpsType Vector_Operations = {
    .pInit = vector_init,
    .pAdd = vector_add,
    .pFree = vector_free,
    .pBegin = vector_begin,
    .pEnd = vector_end,
    .pSize = vector_size,
    .pCurrent = vector_current,
    .pEmpty = vector_empty,
    .pRemove = vector_remove
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


tcp_socket_t vector_begin(Vector* vector)
{
    if (vector->size > 0) {
        return vector->data[0];
    } else {
        fprintf(stderr, "Vector is empty.\n");
        // You can decide how to handle this case; for example, return a default value or exit
        exit(EXIT_FAILURE);
    }   
}

tcp_socket_t vector_end(Vector* vector) {
    if (vector->size > 0) {
        return vector->data[vector->size - 1];
    } else {
        fprintf(stderr, "Vector is empty.\n");
        // You can decide how to handle this case; for example, return a default value or exit
        exit(EXIT_FAILURE);
    }
}

int vector_size(Vector* vector)
{    
    if (vector->size > 0) {
        return vector->size;
    } else {
        fprintf(stderr, "Vector is empty.\n");
        // You can decide how to handle this case; for example, return a default value or exit
        exit(EXIT_FAILURE);
    }

}

tcp_socket_t vector_current(Vector* vector, int index)
{
    if (vector->size > 0) {
        return vector->data[index];
    } else {
        fprintf(stderr, "Vector is empty.\n");
        // You can decide how to handle this case; for example, return a default value or exit
        exit(EXIT_FAILURE);
    }  
}


int vector_empty(Vector* vector)
{    
    if (vector->size > 0) {
        return FAILURE;
    } else {
        //fprintf(stderr, "Vector is empty.\n");
        // You can decide how to handle this case; for example, return a default value or exit
        return SUCCESS;
    }

}

void vector_remove(Vector* vector, size_t index)
{
    if (index >= vector->size) 
    {
        printf("Index out of bounds\n");
        return;
    }
    // Move elements after the index to the left
    memmove(&vector->data[index], &vector->data[index + 1], (vector->size - index - 1) * sizeof(tcp_socket_t));
    vector->size--;
}