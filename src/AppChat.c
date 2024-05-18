#include "AppChat.h"
#include "ConnectionManager.h"
#include "vector.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
const struct AppChatOpsType AppChat_Operations = {
    .pOnStart = OnStart,
    .pMenu  = DisplayMenu,
    .pHelp  = DisplayHelp
};


// Static variables for singleton pattern
static AppChat* AppChatInstance = NULL;
static ConnectionManager* connmgr = NULL;

// Constructor equivalent
static AppChat* AppChat_new(int portno)
{
    AppChat* AppChatInstance = (AppChat*)malloc(sizeof(AppChat));
    if (AppChatInstance == NULL)
    {
        fprintf(stderr, "Failed to allocate memory for AppChat instance.\n");
        exit(EXIT_FAILURE);            
    }
    printf("Allocate AppChatInstance successfully\n");
    AppChatInstance->portNo = portno;
    printf("port is %d\n",AppChatInstance->portNo);
    if (connmgr == NULL)
    {
        connmgr = ConnectionManager_new(portno);
    }
    // Initialize the activeConnections vector with a capacity of 10
    Vector_Operations.pInit(&activeConnections, 10);

    ConnectionManager_Operations.pCntMngOnStart(connmgr);

    return AppChatInstance;
}

// Singleton instance getter
AppChat* AppChatGetInstance(int portno)
{
    if (AppChatInstance == NULL)
    {
        AppChatInstance = AppChat_new(portno);
    }

    // AppChatInstance->cntManager = (ConnectionManager*)malloc(sizeof(ConnectionManager));
    // if (AppChatInstance->cntManager == NULL)
    // {
    //     fprintf(stderr, "Failed to allocate memory for ConnectionManager.\n");
    //     free(AppChatInstance);
    //     exit(EXIT_FAILURE);        
    // }
    // printf("Allocate cntManager successfully\n");
    // AppChatInstance->cntManager->portNo = portno;
    // printf("Port no is %d\n",AppChatInstance->cntManager->portNo);

    return AppChatInstance;

}

// Destructor equivalent
void AppChatClean() {
    if (AppChatInstance != NULL) {
        free(AppChatInstance);
        printf("Release AppChatInstance successfully\n");
        AppChatInstance = NULL;
    }

    if (connmgr != NULL)
    {
        ConnectionManager_delete(connmgr);
        printf("Release connmgr successfully\n");
        connmgr = NULL;
    }
    // Clean up the activeConnections vector
    vector_free(&activeConnections);

}

void OnStart(void)
{
    int option;
    int port;
    int connectionID;
    bool success = false;
    char destinationIP[100];
    while (true) {
        AppChat_Operations.pMenu();
        printf("Select your option: ");
        scanf("%d", &option);
        system("clear");

        switch (option) {
            case 1:
                AppChat_Operations.pHelp();
                //return 0;

            case 2:
                ConnectionManager_Operations.pIpAddress(connmgr);
                break;

            case 3:
                ConnectionManager_Operations.pPortNo(connmgr);
                break;

            case 4:
                printf("Enter the destination IP address: ");
                scanf("%s", destinationIP);
                printf("Enter the port number: ");
                scanf("%d", &port);

                ConnectionManager_Operations.pConToDes(destinationIP, port);
                break;

            case 5:
                ConnectionManager_Operations.pShowALl();
                break;

            case 6:
                printf("Enter the connection ID: ");
                scanf("%d", &connectionID);

                //success = terminateConnection(connectionID);
                //if (!success)
                //    printf("Connection id %d is not valid\n", connectionID);
                //break;

            case 7:
                printf("Enter the connection ID: ");
                scanf("%d", &connectionID);

                //sendDataToConnection(connectionID);
                break;

            case 0:
                printf("Exit program\n");
                return 0;

            default:
                break;
        }
    }
    return 0;
}

void DisplayMenu()
{
    printf("\n\n#################################################\n");
    printf("Select these functions below\n");
    printf("[1] help\t\n");
    printf("[2] myip\t\n");
    printf("[3] myport\t\n");
    printf("[4] connect <destination> <port no>\t\n");
    printf("[5] list\t\n");
    printf("[6] terminate <connection id>\t\n");
    printf("[7] send <connection id> <message>\t\n");
    printf("[0] exit\t\n");
}

void DisplayHelp()
{
    printf("A Chat Application for Remote Message Exchange\n");
    printf("[1] help  \tDisplay information about the available user interface options or command manual\n");
    printf("[2] myip  \tDisplay the IP address of this process\n");
    printf("[3] myport\tDisplay the port on which this process is listening for incoming connections\n");
    printf("[4] connect <destination> <port no>\tThis command establishes a new TCP connection\n");
    printf("                                   \t<destination>: The IP address of the computer\n");
    printf("                                   \t<port no>: Port number\n");
    printf("[5] list                           \tDisplay a numbered list of all the connections this process is part of\n");
    printf("[6] terminate <connection id>      \tThis command will terminate the connection listed under the specified number\n");
    printf("[7] send <connection id> <message> \tThis command will send the message to the host on the connection\n");
    printf("                                   \t<connection id>: Connection id of machine\n");
    printf("                                   \t<message>: The message to be sent\n");
    printf("[0] exit                           \tExit program\n");
}
    