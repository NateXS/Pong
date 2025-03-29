#ifndef DEBUG_H
#define DEBUG_H

#pragma once

#include <3ds.h>
#include <string>
#include <cstring>

void debugMsg(const char* message);
void debugMsg(std::string* message);
void debugMsg(std::string message);
        

#endif