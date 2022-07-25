#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "TCPSocket.h"
#include <thread>
#include <memory>

//https://pubs.opengroup.org/onlinepubs/7908799/xns/syssocket.h.html sys/socket.h documentation
//https://docs.microsoft.com/ru-ru/cpp/standard-library/thread-class?view=msvc-170 std::thread documentation

class TCPServer
{
private:
    std::unique_ptr<TCPSocket> server_socket;
    std::thread thr;
public:
    TCPServer();
    TCPServer(int port, const std::string &host_address);
    bool CreateServer();
    void RunServer();
    static void ThreadClient(TCPSocket socket);
    void ShutDown();
    static void PrintMessage(const std::string &message);
    static void PrintMessage(const char *message);
};

#endif // TCPSERVER_H