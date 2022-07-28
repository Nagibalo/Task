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
        out::PrintMessage("Connect to " + client_socket->GetAddress() + 
                ":" + std::to_string(client_socket->GetPort()), out::INFO);
    }
    catch (int e)
    {
        out::PrintMessage(std::strerror(e), out::ERROR);
        Shutdown();
        return false;
    }
    return true;
}

void TCPClient::RunCommunication()
{
    int code = 0;
    while(code != TCPSocket::EXIT) {
        try { 
            out::PrintMessage(client_socket->RecvMessage(), out::RESPONCE);
            code = getCorrectInputInt("Command code", TCPSocket::ECHO, TCPSocket::EXIT);
            client_socket->SendMessage(std::to_string(code).data());
            switch (code)
            {
                case TCPSocket::ECHO:
                    {
                        std::string message;
                        while(message != "exit") {
                            out::PrintMessage(client_socket->RecvMessage(), out::RESPONCE);
                            std::cin >> message;
                            client_socket->SendMessage(message);
                        }
                    }
                    break;      
                case TCPSocket::FILE:
                    {
                        out::PrintMessage(client_socket->RecvMessage(), out::FILE_SERVICE);
                        std::string file_name;
                        std::cin >> file_name;
                        client_socket->SendMessage(file_name);
                        if(std::stoi(client_socket->RecvMessage()) != TCPSocket::FILE_EXIST)
                        {
                            out::PrintMessage("File isn't exist", out::FILE_SERVICE);
                            code = code = TCPSocket::EXIT;
                            break;
                        }

                        client_socket->SendMessage(std::to_string(TCPSocket::GET_FILE_SIZE).data());
                        std::string file_size = client_socket->RecvMessage();
                        out::PrintMessage("Size " + file_name + " = " + file_size, out::FILE_SERVICE);
                        
                        std::ofstream fs("test.txt");
                        if(!fs.is_open())
                        {
                            fs.close();
                            client_socket->SendMessage(std::to_string(TCPSocket::EXIT).data());
                            code = code = TCPSocket::EXIT;
                            break;
                        }
                        client_socket->SendMessage(std::to_string(TCPSocket::GET_FILE).data());
                        
                        out::PrintMessage("Started download file " + file_name, out::FILE_SERVICE);
                        client_socket->DownloadFile(fs, std::stoul(file_size));
                        out::PrintMessage("File " + file_name + " was download", out::FILE_SERVICE);
                        fs.close();
                    }   
                    break;
                case TCPSocket::ABOUT:
                    {

                    }
                    break;
                case TCPSocket::EXIT:
                    {
                        out::PrintMessage("Try close socket", out::INFO);   
                        code = TCPSocket::EXIT;
                    }
                    break;
                default:
                    {
                        out::PrintMessage("Unknow command", out::INFO);
                    }
                    break;
            }
        }
        catch(int e)
        {
            if(e = ECONNRESET)
                out::PrintMessage("Server was closed", out::INFO);
            else
                out::PrintMessage(std::strerror(e), out::ERROR);
            code = TCPSocket::EXIT;
        }
        catch(std::invalid_argument)
        {
            code = TCPSocket::EXIT;
        }
    }
    Shutdown();
}

int TCPClient::getCorrectInputInt(const std::string& nameValue, int min, int max)
{
    int value;
    while (true)
    {
        std::cout << "Enter " << nameValue << ": ";
        std::cin >> value;
        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            out::PrintMessage("Invalid argument", out::INFO);
        }
        else if (value < min || value > max)
        {
            out::PrintMessage("Invalid argument", out::INFO);
        }
        else
            return value;
    }
}

void TCPClient::Shutdown() {
    client_socket->Shutdown();
    out::PrintMessage("Shutdown client socket", out::INFO);
}