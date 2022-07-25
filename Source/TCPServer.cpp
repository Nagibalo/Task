#include "TCPServer.h"

TCPServer::TCPServer() {    
    server_socket = std::make_unique<TCPSocket>();
}

TCPServer::TCPServer(int port, const std::string &host_address) {
    server_socket = std::make_unique<TCPSocket>(port, host_address);
}

bool TCPServer::CreateServer() {
    try { 
        server_socket->CreateSocket();
        server_socket->SetSocketOption();
        server_socket->Bind();
        server_socket->Listen();
        PrintMessage("Server run in " + server_socket->GetAddress() + 
            ":" + std::to_string(server_socket->GetPort()));
        return true;
    }
    catch (int e)
    {
        server_socket->ShutDown();
        PrintMessage(std::strerror(e));
        return false;
    }
}

void TCPServer::RunServer() {
    for(int i = 0; i < 2; i++)
    {   
        //std::unique_ptr<TCPSocket> client_socket;
        PrintMessage("Await");
        try {
            //client_socket = std::make_unique<TCPSocket>(server_socket->Accept());
            TCPSocket client_socket = server_socket->Accept();
            PrintMessage("New client " + client_socket.GetAddress() + 
                ":" + std::to_string(client_socket.GetPort()));
            //thr = std::thread(ThreadClient, client_socket);
            //thr.detach();
            ThreadClient(client_socket);
        }
        catch (int e)
        {
            PrintMessage(std::strerror(e));
            break;
        }
 

    }
}



void TCPServer::ThreadClient(TCPSocket socket) {
    int code = 0;
    while(code != 4) {
        try { 
            socket.SendMessage("Server command:\n1)Echo\n2)Get file\n3)About\n4)Exit\n\0");
            code = std::stoi(socket.RecvMessage());

            switch (code)
            {
            case 1:
            {
                socket.SendMessage(R"(Echo service. Print "/exit" for exit)");
                std::string responce;
                while(responce != "/exit") {
                    responce = socket.RecvMessage();
                    PrintMessage(responce);
                    socket.SendMessage(responce);
                }
                break;
            }        
            default:
                break;
            }
        }
        catch(int e)
        {
            PrintMessage(std::strerror(e));
            code = 4;
        }
        catch(std::invalid_argument)
        {
            socket.SendMessage("Error: invalid argument");
        }
    }
    socket.ShutDown();
}

void TCPServer::PrintMessage(const std::string &message)
{
    std::time_t time_now = std::time(nullptr);
    char buffer[64];
    std::strftime(buffer, sizeof (buffer) , "%d/%m/%y %X" , std::localtime(&time_now));
    std::cout << '[' << buffer << "] {INFO} " << message << '\n';
}

void TCPServer::PrintMessage(const char *message)
{
    std::time_t time_now = std::time(nullptr);
    char buffer[64];
    std::strftime(buffer, sizeof (buffer) , "%d/%m/%y %X" , std::localtime(&time_now));
    std::cout << '[' << buffer << "] {INFO} " << message << '\n';
}