#include "ConnectionManager.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>


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
        printf("Free CntManger successfully\n");
    }
}
