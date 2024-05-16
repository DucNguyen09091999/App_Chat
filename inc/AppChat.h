#ifndef __APP_CHAT_H__
#define __APP_CHAT_H__

// Declaration of AppChat structure
typedef struct AppChat {
    int portNo;
}AppChat;

// Function prototypes
AppChat* AppChatGetInstance(int PortNo);
void AppChatClean();

#endif