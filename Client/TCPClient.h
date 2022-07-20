#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//https://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html sys/socket.h documentation

class TCPClient
{
private:
    const int DEFAULT_PORT = 8080;
    int socket_fd; // the socket file descriptor
    struct sockaddr_in address;
public:
    TCPClient();
    TCPClient(int port, const char *ip_address);
    bool connectToServer();
    void shutDown();
};

#endif // TCPCLIENT_H