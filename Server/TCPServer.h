#ifndef TCPSERVER_H
#define TCPSERVER_H
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <thread>
#include <memory>

//https://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html sys/socket.h documentation
//https://docs.microsoft.com/ru-ru/cpp/standard-library/thread-class?view=msvc-170 std::thread documentation

class TCPServer
{
private:
    const int DEFAULT_PORT = 8080;
    int socket_fd; // the socket file descriptor
    struct sockaddr_in address;
    //std::unique_ptr<std::thread> threadAwaitAcceptNewConnection; 
    void awaitAcceptNewConnection();
public:
    TCPServer();
    TCPServer(int port, const char *ip_address);
    bool createServer();
    void runServer();
    void shutDown();
};

#endif // TCPSERVER_H