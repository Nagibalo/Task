#include "TCPClient.h"

TCPClient::TCPClient() {    
    client_socket = std::make_unique<TCPSocket>();
}

TCPClient::TCPClient(int port, const std::string &host_address) {
    client_socket = std::make_unique<TCPSocket>(port, host_address);
}

bool TCPClient::ConnectToServer() {
    try {
        client_socket->CreateSocket();
        client_socket->Connect();
        PrintMessage("Connect to " + client_socket->GetAddress() + 
                ":" + std::to_string(client_socket->GetPort()));
    }
    catch (int e)
    {
        client_socket->ShutDown();
        PrintMessage(std::strerror(e));
        return false;
    }
    return true;
}

void TCPClient::RunCommunication()
{
    int code = 0;
    while(code != 4) {
        try { 
            std::string responce = client_socket->RecvMessage();
            PrintMessage(responce);
            std::cin >> code;
            std::string code_str = std::to_string(code);
            client_socket->SendMessage(code_str);

            switch (code)
            {
            case 1:
            {
                PrintMessage(client_socket->RecvMessage());
                std::string message;
                while(message != "/exit") {
                    std::cin >> message;
                    client_socket->SendMessage(message);
                    PrintMessage(client_socket->RecvMessage());
                }
                break;
            }        
            default:
                break;
            }
        }
        catch(int e)
        {
            std::cout << e;
        }
        catch(std::invalid_argument)
        {
            client_socket->SendMessage("Error: invalid argument");
        }
    }
    client_socket->ShutDown();
}

void TCPClient::PrintMessage(const std::string &message)
{
    std::time_t time_now = std::time(nullptr);
    char buffer[64];
    std::strftime(buffer, sizeof (buffer) , "%d/%m/%y %X" , std::localtime(&time_now));
    std::cout << '[' << buffer << "] {INFO} " << message << '\n';
}

void TCPClient::PrintMessage(const char *message)
{
    std::time_t time_now = std::time(nullptr);
    char buffer[64];
    std::strftime(buffer, sizeof (buffer) , "%d/%m/%y %X" , std::localtime(&time_now));
    std::cout << '[' << buffer << "] {INFO} " << message << '\n';
}