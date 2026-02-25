#ifndef LISTENER_H
#define LISTENER_H

#include <winsock2.h>

struct ThreadArgs {
    int port;
    bool* isRunning;
};

// Функция самого потока
DWORD WINAPI UDPListenerThread(LPVOID lpParam);

// Удобная обертка для запуска из main
void startListener();

#endif