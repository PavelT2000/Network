#include <winsock2.h>
#include <iostream>
#include <fstream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

int main() {
    // Чтение порта
    int port = 0;
    std::ifstream configFile("config.txt");
    if (!(configFile >> port)) {
        std::cerr << "Error: Config file not found or empty!" << std::endl;
        return 1;
    }

    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET listenSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    
    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons((unsigned short)port);

    bind(listenSocket, (sockaddr*)&serverAddr, sizeof(serverAddr));
    listen(listenSocket, SOMAXCONN);

    std::cout << "Server is listening on port " << port << "..." << std::endl;

    

    while (true) {
        sockaddr_in clientAddr;
        int clientAddrSize = sizeof(clientAddr);
        SOCKET clientSocket = accept(listenSocket, (sockaddr*)&clientAddr, &clientAddrSize);
        
        if (clientSocket != INVALID_SOCKET) {
            std::cout << "[Client connected]" << std::endl;

            // ВНУТРЕННИЙ ЦИКЛ: Общаемся с клиентом до победного
            char buffer[1024];
            while (true) {
                int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
                
                if (bytesReceived > 0) {
                    buffer[bytesReceived] = '\0';
                    std::cout << "Client says: " << buffer << std::endl;
                } 
                else if (bytesReceived == 0) {
                    std::cout << "[Client disconnected gracefully]" << std::endl;
                    break; 
                } 
                else {
                    std::cout << "[Connection error or closed]" << std::endl;
                    break;
                }
            }
            closesocket(clientSocket); // Закрываем только когда клиент ушел
        }
    }

    closesocket(listenSocket);
    WSACleanup();
    return 0;
}