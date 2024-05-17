#ifndef __CONNECTION_MANAGER_H__
#define __CONNECTION_MANAGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>

#define	TCP_NO_ERROR		    0  // no error
#define	TCP_SOCKET_ERROR	    1  // invalid socket
#define	TCP_ADDRESS_ERROR	    2  // invalid port and/or IP address
#define	TCP_SOCKOP_ERROR	    3  // socket operator (socket, listen, bind, accept,...) error
#define TCP_CONNECTION_CLOSED	4  // send/receive indicate connection is closed
#define	TCP_MEMORY_ERROR	    5  // mem alloc error

#define TIMEOUT         100 // in seconds
#define MAX_PENDING     10  // max connections

#define FAILURE         -1  // return status
#define SUCCESS         0   // return status

typedef struct tcp_socket {
    int id;
    int sd;
    int port;
    char ipAddress[16];
} tcp_socket_t;

// Forward declaration of ConnectionManager
typedef struct ConnectionManager {
    int serverListeningPort;
    char serverIPAddress[15];
    pthread_t threadID; 
} ConnectionManager;

struct ConnectionManagerOpsType {
    int (*pCntMngOnStart)(ConnectionManager* manager);
    int (*pTcpOpen)(tcp_socket_t **socket, int port);
};

struct ConnectionManagerOpsType;

// Function prototypes
int CntOnStart(ConnectionManager* manager);
int TCPOpen(tcp_socket_t** tcpSocket, int port);
extern const struct ConnectionManagerOpsType ConnectionManager_Operations;

// Function prototypes for ConnectionManager
ConnectionManager* ConnectionManager_new(int port);
void ConnectionManager_delete(ConnectionManager* self);

#endif