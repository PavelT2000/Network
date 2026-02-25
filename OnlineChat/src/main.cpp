#include <iostream>
#include <string>
#include <thread>
#include <winsock2.h>
#include "Listener.h"

#pragma comment(lib, "ws2_32.lib")

void sendMessage(const std::string& text) {
    SOCKET sendSock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    
    BOOL broadcast = TRUE;
    setsockopt(sendSock, SOL_SOCKET, SO_BROADCAST, (char*)&broadcast, sizeof(broadcast));

    sockaddr_in recvAddr;
    recvAddr.sin_family = AF_INET;
    recvAddr.sin_port = htons(8080);
    recvAddr.sin_addr.s_addr = inet_addr("255.255.255.255");

    sendto(sendSock, text.c_str(), text.length(), 0, (sockaddr*)&recvAddr, sizeof(recvAddr));
    closesocket(sendSock);
}

int main() {
    HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD dwMode = 0;
    if (GetConsoleMode(hOut, &dwMode)) {
        dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
        SetConsoleMode(hOut, dwMode);
    }
    // Инициализация WinSock
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    // Запуск слушателя в фоне
    std::thread listenerThread(startListener);
    listenerThread.detach();

    std::string input;
    while (true) {
        // ANSI код: Очистить текущую строку и поставить курсор в начало
        std::cout << "\r\033[K> " << std::flush; 
        
        if (!std::getline(std::cin, input)) break;
        if (input == "exit") break;

        if (!input.empty()) {
            sendMessage(input);
        }
    }

    WSACleanup();
    return 0;
}