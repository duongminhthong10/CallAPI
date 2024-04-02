#include <iostream>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "http.h"

int main() {
    Http http;
    string request = "GET / HTTP/1.1\r\n"
                          "Host: www.example.com\r\n"
                          "Connection: close\r\n"
                          "\r\n";
    string response = "";
    http.httpGet("www.example.com", "http", request, response);
    cout << response.c_str();
    return 0;
}
