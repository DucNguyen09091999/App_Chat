#include "ConnectionManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <pthread.h>
#include "vector.h"
#include <poll.h>
static struct pollfd *poll_fds = NULL;
static tcp_socket_t serverConnection;
static int serverFd;
const struct ConnectionManagerOpsType ConnectionManager_Operations = {
    .pCntMngOnStart = CntOnStart,
    .pIpAddress = DisplayIpAddress,
    .pPortNo = DisplayPortNumber,
    .pConToDes = ConnectToDestination,
    .pShowALl = DisplayAllActiveConnection,
    .pAdd = AddConnection,
    .pTerminal = TerminalConnection,
    .pSendData = SendDataToConnection,
    .pTcpOpen = TCPOpen,
    .pTcpClose = TCPClose,
    .pTcpSend = TCPSend,
    .pTcpReceive = TCPReceived,
    .pTCPGetAddr = TCPGetIpAddress,
    .pTCPGetPort = TCPGetPort,
    .pTCPGetSd = TCPGetSd
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


static int tcpWaitForConnection(tcp_socket_t* socket, tcp_socket_t** newSocket)
{
    int result;
    struct sockaddr_in addr;
    tcp_socket_t *tmpSocket;
    unsigned int length = sizeof(struct sockaddr_in);
    char * p;

    tmpSocket = (tcp_socket_t *)malloc(sizeof(tcp_socket_t));
    tmpSocket->sd = accept(socket->sd, (struct sockaddr*) &addr, &length);

    p = inet_ntoa(addr.sin_addr);

    tmpSocket->port = ntohs(addr.sin_port);
    strcat(tmpSocket->ipAddress,p);

    *newSocket = tmpSocket;
    return TCP_NO_ERROR;
}

static void *connectionManagerThread(void *arg)
{   
    int success = FAILURE;
    int poll_res;
    int connCounter = 0;
    int numbRead;
    tcp_socket_t* sPointer = NULL;
    tcp_socket_t dumpy;

    poll_fds = (struct pollfd *)malloc(sizeof(struct pollfd));
    poll_fds[0].fd = serverConnection.sd;
    poll_fds[0].events = POLLIN;

    while ((poll_res = poll(poll_fds, (connCounter+1), TIMEOUT*1000)) || connCounter) {
        if (poll_res == FAILURE)
            break;

        // When an event is received from Server socket, create new socket unless limit is reached
        if ((poll_fds[0].revents & POLLIN) && connCounter < MAX_PENDING) {
            
            // Blocks until a connection is processed
            if((tcpWaitForConnection(&serverConnection, &(sPointer))) != TCP_NO_ERROR) {
                printf("connectionManagerThread: failed to accept new connection");
                //cerr << time(NULL) << " connectionManagerThread: failed to accept new connection (" << success << ")" << endl; 
            }

            // cout << "tmpSocket->sd : " << sPointer->sd  << endl;
            // cout << "Port: " << sPointer->port << endl;
            // cout << "IP: " << sPointer->ipAddress << endl;

            connCounter++; // Increment number of connections
            poll_fds = (struct pollfd *)realloc(poll_fds, sizeof(struct pollfd)*(connCounter+1)); // Increase poll_fd array size
            poll_fds[connCounter].fd = sPointer->sd;
            poll_fds[connCounter].events = POLLIN | POLLHUP;

            // Push to list active connection
            //activeConnections.push_back(*sPointer);
            Vector_Operations.pAdd(&activeConnections, *sPointer);
            poll_res--;
        }

        //When an event is received from Client socket
        for (int i = 1; i < (connCounter+1) && poll_res > 0; i++) {
            if((poll_fds[i].revents & POLLIN)) {
                char reciveBuffer[100];
                memset(reciveBuffer, '0', sizeof(reciveBuffer));
                numbRead = read(poll_fds[i].fd, reciveBuffer, sizeof(reciveBuffer));
                if (numbRead == -1)
                    printf("connectionManagerThread: read failed\n");
                    //cout << "connectionManagerThread: read failed" << endl;
                else {
                    // Assuming activeConnections is an array of tcp_socket_t and activeConnections_size is the size of the array

                    for (int j = 0; j < Vector_Operations.pSize(&activeConnections); j++) 
                    {
                        tcp_socket_t t_Current = Vector_Operations.pCurrent(&activeConnections, j);
                        if (t_Current.sd == poll_fds[i].fd) 
                        {
                            printf("\nMessage received from %s\n", t_Current.ipAddress);
                            printf("Sender’s Port: <%d>\n", ntohs(t_Current.port));
                            break;
                        }
                    }
                    printf("Message: %s\n",reciveBuffer );
                    //cout << "Message: " << reciveBuffer << endl;
                    break;
                }
            }
        }
    }
    printf("thread oke\n");
    return 0;
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
    //serverConnection.ipAddress = manager->serverIPAddress;n
    strncpy(serverConnection.ipAddress, manager->serverIPAddress, sizeof(serverConnection.ipAddress) - 1);
    Vector_Operations.pAdd(&activeConnections, serverConnection);

    if (pthread_create(&manager->threadID, NULL, &connectionManagerThread, NULL)) {
        printf("ConnectionManager: %s %d\n",__func__, __LINE__);
        //cerr << "ConnectionManager: " << __func__ << " " << __LINE__ << endl;
        return -1;
    }

    printf("ConnectionManager: start successfully\n");
    return 0;  
}


void DisplayIpAddress(ConnectionManager* manager)
{
    printf("System IP Address is: %s\n",manager->serverIPAddress);
};
void DisplayPortNumber(ConnectionManager* manager)
{
    printf("Listening port is: %d\n",manager->serverListeningPort);
};
int ConnectToDestination(char* destinationIP, int port)
{
    tcp_socket_t serverConnection;
    struct sockaddr_in serverAddr;

    serverFd = socket(AF_INET, SOCK_STREAM, 0);
    if (serverFd == -1) {
        printf("Failed to create a socket\n");
        return -1;
    }

    // Prepare the server address structure
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = inet_addr(destinationIP);

    // Attempt to connect to the specified destination
    if (connect(serverFd, (struct sockaddr*)(&serverAddr), sizeof(serverAddr)) == -1) {
        printf("Failed to connect to the destination\n");
        close(serverFd);
        return false;
    }

    serverConnection.id = activeConnections.size + 1;
    serverConnection.sd = serverFd;
    serverConnection.port = port;
    //serverConnection.ipAddress = manager->serverIPAddress;n
    strncpy(serverConnection.ipAddress, destinationIP, sizeof(serverConnection.ipAddress) - 1);
    Vector_Operations.pAdd(&activeConnections, serverConnection);

    printf("Connected to %s on port %d\n", destinationIP, port);

    return SUCCESS;
};
void DisplayAllActiveConnection()
{
    printf("List all active connections\n");
    if (Vector_Operations.pEmpty(&activeConnections)) {
        printf("No  active connections\n");
    } else {
        printf("id\tIP adrress\tPort No\n");
        for (size_t i = 0; i < Vector_Operations.pSize(&activeConnections); ++i) {
            tcp_socket_t t_Current = Vector_Operations.pCurrent(&activeConnections, i);
            printf("%d\t%s\t%d\n",(i+1),t_Current.ipAddress, t_Current.port);
        }
    }
};
void AddConnection(const char* ipAddress, int port)
{
    tcp_socket_t newConnection;

    serverConnection.id = activeConnections.size + 1;
    newConnection.sd = -1;
    newConnection.port = port;
    strncpy(serverConnection.ipAddress, ipAddress, sizeof(serverConnection.ipAddress) - 1);
    Vector_Operations.pAdd(&activeConnections, newConnection);
};
bool TerminalConnection(int connectionID)
{
    bool isConnectionIDValid = false;

    // for (auto conn = activeConnections.begin(); conn != activeConnections.end();) {
    //     if (conn->id == connectionID) {
    //         conn = activeConnections.erase(conn);
    //         isConnectionIDValid = true;
    //     } else {
    //         ++conn;
    //     }
    // }

    for (int j = 0; j < Vector_Operations.pSize(&activeConnections); j++) 
    {
        tcp_socket_t t_Current = Vector_Operations.pCurrent(&activeConnections, j);
        if (t_Current.id == connectionID) 
        {
            Vector_Operations.pRemove(&activeConnections, j);
            isConnectionIDValid = true;
        }
    }

    // reorganizeConnectionIds(activeConnections);
    // displayAllActiveConnection();

    return isConnectionIDValid;
};
bool SendDataToConnection(int connectionID)
{

};
void TCPClose(tcp_socket_t** socket, int port)
{

};
void TCPSend(tcp_socket_t* socket, void* buffer, int* size )
{

};
void TCPReceived(tcp_socket_t* socket, void* buffer, int* size )
{

};
void TCPGetIpAddress(tcp_socket_t* socket, char* ipAddress)
{

};
void TCPGetPort(tcp_socket_t* socket, int* port)
{

};
void TCPGetSd(tcp_socket_t* socket, int* sd)
{

};