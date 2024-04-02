#ifndef HTTP_H
#define HTTP_H

#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>

using namespace std;
#pragma comment(lib, "ws2_32.lib")

class Http
{
    SOCKET sockfd;
    WSADATA wsa_data;
public:
    Http();
    ~Http();
    bool createSocket();
    bool connectServer(string url, string protocol);
    bool httpGet(string url, string protocol, string request, string &response);
    void closeConnection();
};

#endif // HTTP_H
