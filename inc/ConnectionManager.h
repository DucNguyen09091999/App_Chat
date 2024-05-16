#ifndef __CONNECTION_MANAGER_H__
#define __CONNECTION_MANAGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

typedef struct tcp_socket {
    int id;
    int sd;
    int port;
    char ipAddress;
} tcp_socket_t;

// Forward declaration of ConnectionManager
typedef struct ConnectionManager {
    int serverListeningPort;
    char serverIPAddress[15];
} ConnectionManager;

struct ConnectionManagerOpsType {
    int (*pCntMngOnStart)(void);
};

struct ConnectionManagerOpsType;

// Function prototypes
int CntOnStart(void);
extern const struct ConnectionManagerOpsType ConnectionManager_Operations;

// Function prototypes for ConnectionManager
ConnectionManager* ConnectionManager_new(int port);
void ConnectionManager_delete(ConnectionManager* self);

#endif