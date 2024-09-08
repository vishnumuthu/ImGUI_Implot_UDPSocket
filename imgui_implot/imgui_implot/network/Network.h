#include <iostream>
#include <winsock2.h>
//#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

class UDPHandler {
private:
    SOCKET sockfd;
    struct sockaddr_in serverAddr, clientAddr;
    int addrLen;
    char buffer[1024];

public:
    UDPHandler(int port) {
        WSADATA wsaData;
        if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
            std::cerr << "WSAStartup failed" << std::endl;
            exit(EXIT_FAILURE);
        }

        sockfd = socket(AF_INET, SOCK_DGRAM, 0);
        if (sockfd == INVALID_SOCKET) {
            std::cerr << "Socket creation failed" << std::endl;
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        // Set socket options using setsockopt()
        int timeout = 10; // 5 seconds
        if (setsockopt(sockfd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeout, sizeof(timeout)) == SOCKET_ERROR) {
            std::cerr << "setsockopt failed" << std::endl;
            closesocket(sockfd);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        memset(&serverAddr, 0, sizeof(serverAddr));
        memset(&clientAddr, 0, sizeof(clientAddr));

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);

        if (bind(sockfd, (struct sockaddr *)&serverAddr, sizeof(serverAddr)) == SOCKET_ERROR) {
            std::cerr << "Bind failed" << std::endl;
            closesocket(sockfd);
            WSACleanup();
            exit(EXIT_FAILURE);
        }

        addrLen = sizeof(clientAddr);
    }

    std::string receiveData() {
        int n = recvfrom(sockfd, buffer, 1024, 0, (struct sockaddr *)&clientAddr, &addrLen);
        if (n == SOCKET_ERROR) {
            std::cerr << "Receive failed" << std::endl;
            return "NILL";
        }
        buffer[n] = '\0';
        return std::string(buffer);
    }

    void sendData(const std::string &data) {
        sendto(sockfd, data.c_str(), data.length(), 0, (struct sockaddr *)&clientAddr, addrLen);
    }

    ~UDPHandler() {
        closesocket(sockfd);
        WSACleanup();
    }
};
