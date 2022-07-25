#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include "TCPSocket.h"
#include <memory>
#include <ctime>

//https://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html sys/socket.h documentation

class TCPClient
{
private:
    std::unique_ptr<TCPSocket> client_socket;
public:
    TCPClient();
    TCPClient(int port, const std::string &host_address);
    bool ConnectToServer();
    void RunCommunication();
    void ShutDown();
    void PrintMessage(const std::string &message);
    void PrintMessage(const char *message);
};

#endif // TCPCLIENT_H