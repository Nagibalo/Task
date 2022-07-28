#ifndef TCPSERVER_H
#define TCPSERVER_H
#include "TCPSocket.h"
#include "TerminalOuput.h"
#include "ThreadPool.h"
#include <thread>
#include <memory>
#include <signal.h>
#include <atomic>
#include <chrono>
//https://docs.microsoft.com/ru-ru/cpp/standard-library/thread-class?view=msvc-170 std::thread documentation


#define DEFAULT_MAX_COUNT_CONNECTION_REQUESTS 2
#define DEFAULT_MAX_COUNT_CONNECTIONS 2

class TCPServer
{
private:
    /*! \brief Work flag of server */
    static std::atomic<bool> is_server_run;
    /*! \brief Index of signal to close programm reaceved by process*/
    static std::atomic<int> signal_i;

    int max_count_connections, max_count_connection_requests;
    static std::atomic<int> count_connections;
    std::unique_ptr<TCPSocket> server_socket;
    ThreadPool thread_p;

    void RunSignalHander();
    static void SetSignalHandler();   
    static void RunLoopWaitSignal();
    static void SignalHandler(int sig);

    static void ClientHandler(std::shared_ptr<TCPSocket> socket);
public:
    TCPServer();
    TCPServer(int port, const std::string &host_address, 
              int max_count_connections = DEFAULT_MAX_COUNT_CONNECTIONS, 
              int max_count_connection_requests = DEFAULT_MAX_COUNT_CONNECTION_REQUESTS);
    bool CreateServer();
    void RunAcceptConnections();
    void Shutdown();
};

#endif // TCPSERVER_H