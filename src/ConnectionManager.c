#include "ConnectionManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include "vector.h"

static tcp_socket_t serverConnection;
const struct ConnectionManagerOpsType ConnectionManager_Operations = {
    .pCntMngOnStart = CntOnStart,
    .pTcpOpen = TCPOpen,
};


// Initialize a new instance of ConnectionManager
ConnectionManager* ConnectionManager_new(int port) {
    ConnectionManager* instance = (ConnectionManager*)malloc(sizeof(ConnectionManager));
    if (instance == NULL) {
        perror("Failed to allocate memory for ConnectionManager instance");
        exit(EXIT_FAILURE);
    }
    instance->serverListeningPort = port;

    int fd;
    struct ifreq ifr;

    fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (fd == -1) {
        perror("Socket creation failed");
        free(instance);
        exit(EXIT_FAILURE);
    }

    ifr.ifr_addr.sa_family = AF_INET;
    strncpy(ifr.ifr_name, "ens33", IFNAMSIZ - 1);

    if (ioctl(fd, SIOCGIFADDR, &ifr) == -1) {
        perror("IOCTL failed");
        close(fd);
        free(instance);
        exit(EXIT_FAILURE);
    }

    close(fd);

    strcpy(instance->serverIPAddress, inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr));
    printf("System IP Address is: %s\n", instance->serverIPAddress);
    printf("Listening at port: %d\n", port);

    return instance;
}


// Deallocate memory used by the ConnectionManager instance
void ConnectionManager_delete(ConnectionManager* self) {
    if (self != NULL) {
        free(self);
        //printf("Free CntManger successfully\n");
    }
}

int TCPOpen(tcp_socket_t** tcpSocket, int port)
{
    int result, opt;
    struct sockaddr_in addr;
    tcp_socket_t *tmpSocket;

    tmpSocket = (tcp_socket_t *)malloc(sizeof(tcp_socket_t));

    if(!tmpSocket)
    {
        printf("Malloc failure\n");
        return FAILURE;
    }

    tmpSocket->sd = socket(AF_INET, SOCK_STREAM, 0);

    if (tmpSocket->sd == FAILURE)
    {
        printf("Create socket failure\n");
        free(tmpSocket);
        return FAILURE;
    }

    if (setsockopt(tmpSocket->sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &opt, sizeof(opt)))
    {
        printf("Set socket opt failure\n");
        free(tmpSocket);
        return FAILURE;
    }

    memset(&addr, 0, sizeof(struct sockaddr_in));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(tmpSocket->sd, (struct sockaddr*)&addr, sizeof(addr)) == FAILURE)
    {
        printf("Binding socket failure\n");
        free(tmpSocket);
        return FAILURE;
    }

    if (listen(tmpSocket->sd, MAX_PENDING) == -1)
    {
        printf("Listening socket failure\n");
        free(tmpSocket);
        return FAILURE;
    }
 
    (*tcpSocket) = tmpSocket;
    return TCP_NO_ERROR;

}


int CntOnStart(ConnectionManager* manager)
{
   tcp_socket_t *sPointer = NULL;

    // Initially array for 1 element (listening server)
    if (TCPOpen(&sPointer, manager->serverListeningPort) != TCP_NO_ERROR) {
        printf("ConnectionManager: failed to start\n");
        return FAILURE;
    }

    serverConnection.id = activeConnections.size + 1;
    serverConnection.sd = sPointer->sd;
    serverConnection.port = manager->serverListeningPort;
    //serverConnection.ipAddress = manager->serverIPAddress;
    strncpy(serverConnection.ipAddress, manager->serverIPAddress, sizeof(serverConnection.ipAddress) - 1);
    vector_add(&activeConnections, serverConnection);

    // if (pthread_create(&threadID, NULL, &connectionManagerThread, NULL)) {
    //     cerr << "ConnectionManager: " << __func__ << " " << __LINE__ << endl;
    //     return -1;
    // }

    printf("ConnectionManager: start successfully\n");
    return 0;  
}