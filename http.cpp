#include "http.h"
const int BUFFER_SIZE = 1024;

Http::Http()
{
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data) != 0) {
        cerr << "Cannot Init Winsock" << endl;
        exit(1);
    }
}

Http::~Http()
{
    WSACleanup();
}

bool Http::createSocket()
{
    sockfd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    return sockfd != INVALID_SOCKET;
}

bool Http::connectServer(string url, string protocol)
{
    if (!createSocket())
    {
        cerr << "Cannot create socket";
        return false;
    }

    // get information
    struct addrinfo *result = NULL;
    struct addrinfo hints;
    // clear hint
    ZeroMemory(&hints, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_protocol = IPPROTO_TCP;
    if (getaddrinfo(url.c_str(), protocol.c_str(), &hints, &result) != 0) {
        cerr << "Cannot get infomation" << endl;
        closeConnection();
        return false;
    }
    // connect IP server
    sockaddr_in server_addr;
    memcpy(&server_addr, result->ai_addr, result->ai_addrlen);
    freeaddrinfo(result);

    if (connect(sockfd, (sockaddr*)&server_addr, sizeof(server_addr)) == SOCKET_ERROR) {
        cerr << "cannot connect server" << endl;
        closeConnection();
        return false;
    }
    return true;
}

bool Http::httpGet(string url, string protocol, string request, string &response)
{
    if (!connectServer(url, protocol))
    {
        return false;
    }

    if (request.size() > INT_MAX)
    {
        cerr << "request too long";
        closeConnection();
        return false;
    }

    if (send(sockfd, request.c_str(), static_cast<int>(request.size()), 0) == SOCKET_ERROR)
    {
        cerr << "error HTTP GET" << endl;
        closeConnection();
        return false;
    }

    char buffer[BUFFER_SIZE];
    int bytes_received;
    while ((bytes_received = recv(sockfd, buffer, BUFFER_SIZE - 1, 0)) > 0) {
        buffer[bytes_received] = '\0';
        response += buffer;
    }

    // close socket
    closeConnection();
    return true;
}

void Http::closeConnection()
{
    closesocket(sockfd);
}
