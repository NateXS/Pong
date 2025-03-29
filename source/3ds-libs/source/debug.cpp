#include "../include/debug.h"

void debugMsg(const char* message){
    svcOutputDebugString(message,std::strlen(message));
}

void debugMsg(std::string* message){
    debugMsg(message->c_str());
}

void debugMsg(std::string message){
    debugMsg(message.c_str());
}
