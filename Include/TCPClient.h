#ifndef TCPCLIENT_H
#define TCPCLIENT_H
#include "TCPSocket.h"
#include "TerminalOuput.h"

//g++ -o TCPClient main_client.cpp TCPClient.cpp TCPSocket.cpp 
class TCPClient
{
private:
    std::unique_ptr<TCPSocket> client_socket;
    int getCorrectInputInt(const std::string& nameValue, int min, int max);
public:
    TCPClient();
    TCPClient(int port, const std::string &host_address);
    ~TCPClient();
    bool ConnectToServer();
    void RunCommunication();
    void Shutdown();
};

#endif // TCPCLIENT_H