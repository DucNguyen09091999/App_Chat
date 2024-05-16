#ifndef __APP_CHAT_H__
#define __APP_CHAT_H__

// Declaration of AppChat structure
typedef struct AppChat {
    int portNo;
}AppChat;

struct AppChatOpsType {
    int (*pOnStart)(void);
};

struct AppChatOpsType;

// Function prototypes
AppChat* AppChatGetInstance(int PortNo);
void AppChatClean();
int OnStart(void);
extern const struct AppChatOpsType AppChat_Operations;

#endif