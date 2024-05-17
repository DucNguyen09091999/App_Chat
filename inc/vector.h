#ifndef __APP_VECTOR_H__
#define __APP_VECTOR_H__
#include <stddef.h>
#include "ConnectionManager.h"
//typedef struct tcp_socket_t;
typedef struct {
    tcp_socket_t* data;
    size_t size;
    size_t capacity;
} Vector;

void vector_init(Vector* vector, size_t initial_capacity);
void vector_add(Vector* vector, tcp_socket_t element);
void vector_free(Vector* vector);
extern Vector activeConnections;
#endif