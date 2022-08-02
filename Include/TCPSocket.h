#ifndef TCPSOCKET_H
#define TCPSOCKET_H
#include <iostream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <string>
#include <cstring>
#include <fstream>
#include <iostream>
#include <memory>
#include <unistd.h>

//https://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html sys/socket.h documentation
//https://pubs.opengroup.org/onlinepubs/7908799/xns/arpainet.h.html arpa/inet.h documentation (use htons)
//https://pubs.opengroup.org/onlinepubs/009604599/functions/ioctl.html sys/ioctl.h documentaion (use ioctl)
//https://www.opennet.ru/man.shtml?topic=inet_ntoa&category=3&russian=0 netinet/in.h (use inet_ntoa)

#define DEFAULT_PORT 8080
#define DEFAULT_SEGMENT_SIZE 1024
#define DEFAULT_ADDRESS INADDR_ANY
#define DEFAULT_SOCK_OPTION 1

class TCPSocket
{
private:
    /*! \brief Socket file desription*/
    int socket_fd; 
    /*! \brief Size of segment sending and receaving message*/
    size_t segment_size;
    std::unique_ptr<sockaddr_in> address;
public:
    TCPSocket();
    TCPSocket(int port, const std::string &ip_address, int segment_size = DEFAULT_SEGMENT_SIZE);
    TCPSocket(int socket_fd, std::unique_ptr<sockaddr_in> &address, int segment_size);
    TCPSocket(const TCPSocket& socket);
    TCPSocket(TCPSocket&& socket);

    void CreateSocket();
    void SetSocketOption(int option = DEFAULT_SOCK_OPTION);
    void Connect();
    void Bind();
    void Listen(int max_connection_request);
    std::shared_ptr<TCPSocket> Accept();

    void SendMessage(std::string &message);
    void SendMessage(const char* message);
    std::string RecvMessage();

    void SendFile(std::ifstream &fs);
    void DownloadFile(std::ofstream &fs, size_t file_size);
    void Shutdown(); 
    void Close();
    
    int GetPort();
    std::string GetAddress();

    enum RequestType{
        ECHO, 
        FILE,
        ABOUT,
        EXIT,
        GET_FILE_SIZE,
        FILE_EXIST,
        FILE_NOT_EXIST,
        GET_FILE
    };
};


#endif // TCPSOCKET_H