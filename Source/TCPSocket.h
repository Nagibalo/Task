#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string>
#include <cstring>

//https://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html sys/socket.h documentation
//https://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html arpa/inet.h documentation ("htons")

class TCPSocket
{
private:
    const int DEFAULT_PORT = 8080, MAX_PACKET_SIZE = 1024;
    int socket_fd; // the socket file descriptor
    struct sockaddr_in address;
public:
    TCPSocket();
    TCPSocket(int port, const std::string &ip_address);
    TCPSocket(int socket_fd, const struct sockaddr_in& address);
    TCPSocket(const TCPSocket& socket);
    void CreateSocket();
    void SetSocketOption(int option = 1);
    void Connect();
    void Bind();
    void Listen(int max_connection_request = 3);
    TCPSocket Accept();
    void SendMessage(std::string &message);
    void SendMessage(const char* message);
    std::string RecvMessage();
    void ShutDown(); 
    int GetPort();
    std::string GetAddress();
};


#endif // TCPSOCKET_H