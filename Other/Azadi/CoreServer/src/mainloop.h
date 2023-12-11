#ifndef MAINLOOP_H
#define MAINLOOP_H

#include <unistd.h>
#include <cstdio>
#include <string>

#ifdef WIN32
#include <windows.h>
#endif

#include "logging/logging.h"
#include "sql/sqlCon.h"

int mainLoop();

void handleConnection();

void handleAI();

#endif
