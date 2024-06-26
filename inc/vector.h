#ifndef __APP_VECTOR_H__
#define __APP_VECTOR_H__
#include <stddef.h>
#include "ConnectionManager.h"
#include "vector.h"

//typedef struct tcp_socket_t;
typedef struct {
    tcp_socket_t* data;
    size_t size;
    size_t capacity;
} Vector;

struct VectorOpsType {
    void (*pInit)(Vector* vector, size_t initial_capacity);
    void (*pAdd)(Vector* vector, tcp_socket_t element);
    void (*pFree)(Vector* vector);
    tcp_socket_t (*pBegin)(Vector* vector);
    tcp_socket_t (*pEnd)(Vector* vector);
    tcp_socket_t (*pCurrent)(Vector* vector, int index);
    int (*pSize)(Vector* vector);
    int (*pEmpty)(Vector* vector);
    void (*pRemove)(Vector* vector, size_t index);
};

struct VectorOpsType;


// Function prototypes
void vector_init(Vector* vector, size_t initial_capacity);
void vector_add(Vector* vector, tcp_socket_t element);
void vector_free(Vector* vector);
tcp_socket_t vector_begin(Vector* vector);
tcp_socket_t vector_end(Vector* vector);
int vector_empty(Vector* vector);
int vector_size(Vector* vector);
tcp_socket_t vector_current(Vector* vector, int index);
void vector_remove(Vector* vector, size_t index);
extern Vector activeConnections;
extern const struct VectorOpsType Vector_Operations;

#endif