#ifndef __CONNECTION_MANAGER_H__
#define __CONNECTION_MANAGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

// Forward declaration of ConnectionManager
typedef struct ConnectionManager {
    int serverListeningPort;
    char serverIPAddress[15];
} ConnectionManager;

// Function prototypes for ConnectionManager
ConnectionManager* ConnectionManager_new(int port);
void ConnectionManager_delete(ConnectionManager* self);

#endif