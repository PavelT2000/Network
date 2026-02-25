#include <winsock2.h>
#include <ws2tcpip.h> // Для inet_pton
#include <iostream>
#include <string>

#pragma comment(lib, "ws2_32.lib")

int main() {
    // 1. Инициализация WinSock
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cerr << "WSAStartup failed" << std::endl;
        return 1;
    }

    // 2. Ввод данных для подключения
    std::string ipAddress;
    int port;
    std::cout << "Enter Server IP (e.g. 127.0.0.1): ";
    std::cin >> ipAddress;
    std::cout << "Enter Port: ";
    std::cin >> port;
    std::cin.ignore(); // Очистка буфера после ввода числа

    // 3. Создание сокета
    SOCKET connectSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (connectSocket == INVALID_SOCKET) {
        std::cerr << "Socket creation failed" << std::endl;
        WSACleanup();
        return 1;
    }

    // 4. Настройка структуры адреса сервера
    sockaddr_in clientService;
    clientService.sin_family = AF_INET;
    clientService.sin_port = htons((unsigned short)port);
    inet_pton(AF_INET, ipAddress.c_str(), &clientService.sin_addr); // Преобразуем строку в IP

    // 5. Подключение к серверу
    std::cout << "Connecting..." << std::endl;
    if (connect(connectSocket, (sockaddr*)&clientService, sizeof(clientService)) == SOCKET_ERROR) {
        std::cerr << "Connection failed! Error: " << WSAGetLastError() << std::endl;
        closesocket(connectSocket);
        WSACleanup();
        return 1;
    }

    std::cout << "Connected! Type your message (or 'exit' to quit):" << std::endl;

    // 6. Цикл отправки сообщений
    std::string userInput;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, userInput);

        if (userInput == "exit") break;

        int sendResult = send(connectSocket, userInput.c_str(), (int)userInput.length(), 0);
        if (sendResult == SOCKET_ERROR) {
            std::cerr << "Send failed: " << WSAGetLastError() << std::endl;
            break;
        }
    }

    // 7. Закрытие
    closesocket(connectSocket);
    WSACleanup();
    return 0;
}