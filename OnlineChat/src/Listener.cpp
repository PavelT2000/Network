#include "Listener.h"
#include <iostream>
#include <string>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

bool isRunning = true;
ThreadArgs args = { 8080, &isRunning };

void startListener() {
    CreateThread(NULL, 0, UDPListenerThread, &args, 0, NULL);
}

DWORD WINAPI UDPListenerThread(LPVOID lpParam) {
    ThreadArgs* args = (ThreadArgs*)lpParam;
    int port = args->port;

    // 1. Создаем UDP сокет (SOCK_DGRAM)
    SOCKET udpSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (udpSocket == INVALID_SOCKET) {
        std::cerr << "[Listener] Socket error: " << WSAGetLastError() << std::endl;
        return 1;
    }

    // 2. Настраиваем адрес для прослушивания
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY; // Слушать все интерфейсы
    serverAddr.sin_port = htons((unsigned short)port);

    // 3. Привязываем сокет к порту
    if (bind(udpSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
        std::cerr << "[Listener] Bind error: " << WSAGetLastError() << std::endl;
        closesocket(udpSocket);
        return 1;
    }

    std::cout << "[UDP Listener] Active on port " << port << ". Listening for broadcast...\n";

    char buffer[1024];
    sockaddr_in clientAddr; // Сюда запишется адрес отправителя
    int clientLen = sizeof(clientAddr);

    // 4. Бесконечный цикл прослушивания
    while (*(args->isRunning)) {
        // Очищаем буфер
        memset(buffer, 0, sizeof(buffer));

        // recvfrom блокирует поток до получения любого UDP пакета
        int bytesReceived = recvfrom(udpSocket, buffer, sizeof(buffer) - 1, 0, (sockaddr*)&clientAddr, &clientLen);

        if (bytesReceived > 0) {
            buffer[bytesReceived] = '\0';

            // Преобразуем IP отправителя в строку
            char remoteIP[INET_ADDRSTRLEN];
            inet_ntop(AF_INET, &clientAddr.sin_addr, remoteIP, INET_ADDRSTRLEN);

            std::cout << "\r\033[K[Received from " << remoteIP << "]: " << buffer << std::endl;
            std::cout << "> " << std::flush;
        } 
        else if (bytesReceived == SOCKET_ERROR) {
            int error = WSAGetLastError();
            // Если ошибка не связана с закрытием сокета, выводим её
            if (error != WSAEINTR) {
                std::cerr << "[Listener] Recv error: " << error << std::endl;
            }
            break;
        }
    }

    std::cout << "[Listener] Stopping...\n";
    closesocket(udpSocket);
    return 0;
}