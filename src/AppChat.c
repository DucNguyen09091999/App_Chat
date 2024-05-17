#include "AppChat.h"
#include "ConnectionManager.h"
#include <stdio.h>
#include <stdlib.h>

const struct AppChatOpsType AppChat_Operations = {
    .pOnStart = OnStart,
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

}

int OnStart(void)
{
    printf("On start\n");
    return 0;
}
    