#ifndef __CONNECTION_MANAGER_H__
#define __CONNECTION_MANAGER_H__

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
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
    void (*pPortNo)(ConnectionManager* manager);
    void (*pIpAddress)(ConnectionManager* manager);
    void (*pConToDes)(char* destinationIP, int port);
    void (*pShowALl)(void);
    void (*pAdd)(const char* ipAddress, int port);
    void (*pTerminal)(int connectionID);
    void (*pSendData)(int connectionID);
    void (*pTcpOpen)(tcp_socket_t** socket, int port);
    void (*pTcpClose)(tcp_socket_t** socket, int port);
    void (*pTcpSend)(tcp_socket_t* socket, void* buffer, int* size );
    void (*pTcpReceive)(tcp_socket_t* socket, void* buffer, int* size );
    void (*pTCPGetAddr)(tcp_socket_t* socket, char* ipAddress);
    void (*pTCPGetPort)(tcp_socket_t* socket, int* port);
    void (*pTCPGetSd)(tcp_socket_t* socket, int* sd);

};

struct ConnectionManagerOpsType;

// Function prototypes
int CntOnStart(ConnectionManager* manager);
int TCPOpen(tcp_socket_t** tcpSocket, int port);
void DisplayIpAddress(ConnectionManager* manager);
void DisplayPortNumber(ConnectionManager* manager);
int ConnectToDestination(char* destinationIP, int port);
void DisplayAllActiveConnection();
void AddConnection(const char* ipAddress, int port);
bool TerminalConnection(int connectionID);
bool SendDataToConnection(int connectionID);
int TCPOpen(tcp_socket_t** socket, int port);
void TCPClose(tcp_socket_t** socket, int port);
void TCPSend(tcp_socket_t* socket, void* buffer, int* size );
void TCPReceived(tcp_socket_t* socket, void* buffer, int* size );
void TCPGetIpAddress(tcp_socket_t* socket, char* ipAddress);
void TCPGetPort(tcp_socket_t* socket, int* port);
void TCPGetSd(tcp_socket_t* socket, int* sd);

extern const struct ConnectionManagerOpsType ConnectionManager_Operations;

// Function prototypes for ConnectionManager
ConnectionManager* ConnectionManager_new(int port);
void ConnectionManager_delete(ConnectionManager* self);

#endif