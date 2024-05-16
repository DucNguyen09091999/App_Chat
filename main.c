#include <stdio.h>
#include "AppChat.h"
#include "TCPSocket.h"
#include "ConnectionManager.h"
#include <stdlib.h>

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("Usage: %s <Port Number>\n", argv[0]);
        return 0;
    }

    printf("Welcome to chat application!\n");

    AppChat* app = AppChatGetInstance(atoi(argv[1]));

    AppChatClean();
    return 0;

}