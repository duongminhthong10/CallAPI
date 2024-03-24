#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")

const int BUFFER_SIZE = 1024;

int main() {
    // Init Winsock
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        std::cerr << "Cannot Init Winsock" << std::endl;
        return 1;
    }

    // Create socket
    /* 
    AF_INET: IPv4 
    SOCK_STREAM: TCP    
    */

    SOCKET sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sockfd == INVALID_SOCKET) {
        std::cerr << "Cannot create socket" << std::endl;
        WSACleanup();
        return 1;
    }

    // get information
    struct addrinfo *result = NULL;
    struct addrinfo hints;
    // clear hint
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo("www.example.com", "http", &hints, &result) != 0) {
        std::cerr << "Cannot get infomation" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    // connect IP server
    sockaddr_in server_addr;
    memcpy(&server_addr, result->ai_addr, result->ai_addrlen);
    freeaddrinfo(result);

    if (connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        std::cerr << "cannot connect server" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    // HTTP GET
    std::string request = "GET / HTTP/1.1\r\n"
                          "Host: www.example.com\r\n"
                          "Connection: close\r\n"
                          "\r\n";

    if (send(sockfd, request.c_str(), request.size(), 0) == SOCKET_ERROR) {
        std::cerr << "error HTTP GET" << std::endl;
        closesocket(sockfd);
        WSACleanup();
        return 1;
    }

    // received and printf
    char buffer[BUFFER_SIZE];
    int bytes_received;
    while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        std::cout << buffer;
    }

    // close socket and clean Winsock
    closesocket(sockfd);
    WSACleanup();
    return 0;
}
