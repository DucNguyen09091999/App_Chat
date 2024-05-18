#ifndef __APP_CHAT_H__
#define __APP_CHAT_H__

// Declaration of AppChat structure
typedef struct AppChat {
    int portNo;
}AppChat;

struct AppChatOpsType {
    void (*pOnStart)(void);
    void (*pMenu)(void);
    void (*pHelp)(void);
};

struct AppChatOpsType;

// Function prototypes
AppChat* AppChatGetInstance(int PortNo);
void AppChatClean();
void OnStart(void);
void DisplayMenu();
void DisplayHelp();
extern const struct AppChatOpsType AppChat_Operations;

#endif