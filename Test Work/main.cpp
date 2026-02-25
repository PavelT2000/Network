#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")


int main()
{
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout<<"Error";
    }
}
