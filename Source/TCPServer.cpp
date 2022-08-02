#include "TCPServer.h"

TCPServer::TCPServer() : max_count_connection_requests(DEFAULT_MAX_COUNT_CONNECTION_REQUESTS),
                        server_socket(std::make_unique<TCPSocket>()) {   
    max_count_connections = DEFAULT_MAX_COUNT_CONNECTIONS + 1 <= std::thread::hardware_concurrency() ? 
                            DEFAULT_MAX_COUNT_CONNECTIONS : std::thread::hardware_concurrency() - 1;
}

TCPServer::TCPServer(int port, const std::string &host_address, 
                    int max_count_connections, int max_count_connection_requests) : 
                    max_count_connections(max_count_connections), 
                    max_count_connection_requests(max_count_connection_requests), 
                    server_socket(std::make_unique<TCPSocket>(port, host_address)) { 

}
    
TCPServer::~TCPServer() {
    server_socket->Close();
}

/*! \brief Create server socket, set socket options, bind and listen
 * \return If create success return true */
bool TCPServer::CreateServer() {
    try { 
        server_socket->CreateSocket();
        server_socket->SetSocketOption();
        server_socket->Bind();
        server_socket->Listen(max_count_connection_requests);
        return true;
    }
    catch (int e)
    {
        Shutdown();
        out::PrintMessage(std::strerror(e), out::ERROR);
        return false;
    }
}

/*! \brief Run loop accepting new connections by clients */
void TCPServer::RunServer() {
    out::PrintMessage("Server run in " + server_socket->GetAddress() + 
        ":" + std::to_string(server_socket->GetPort()), out::INFO);
    out::PrintMessage("Max count of connections = " + std::to_string(max_count_connections), out::INFO);
    
    thread_p.Start(max_count_connections + 1); // 1 thread for loop accept connections
    thread_p.QueueJob(std::bind(&TCPServer::RunAcceptConnections, this));
    RunSignalHandler();
}

std::atomic<int> TCPServer::count_connections = 0;

/*! \brief Run loop accepting new connections by clients */
void TCPServer::RunAcceptConnections() {
    while(is_server_run)
    {   
        if(count_connections >= max_count_connections) {
            out::PrintMessage("Max count connections. Await...", out::STATUS);
            std::this_thread::sleep_for(std::chrono::seconds(2));
            continue;
        }

        out::PrintMessage("Await new connection...", out::STATUS);
        try {
            auto client_socket = server_socket->Accept();
            out::PrintMessage("New connection " + client_socket->GetAddress() + 
                ":" + std::to_string(client_socket->GetPort()), out::INFO);
            
            clients.push_back(client_socket);
            thread_p.QueueJob(std::bind(&TCPServer::ClientHandler, this, client_socket));
            count_connections.fetch_add(1);
        }
        catch (int e)
        {
            if(e = ECONNRESET)
                out::PrintMessage("Server socket was closed", out::ERROR);
            else
                out::PrintMessage(std::strerror(e), out::ERROR);
            break;
        }
    }
}


void TCPServer::ClientHandler(std::shared_ptr<TCPSocket> socket) {
    int code = 0;
    while(code != TCPSocket::EXIT && is_server_run) {
        try { 
            socket->SendMessage("Server command:\n0)Echo\n1)Get file\n2)About\n3)Exit");
            code = std::stoi(socket->RecvMessage());
            switch (code)
            {
                case TCPSocket::ECHO:
                    {
                        socket->SendMessage(R"(Echo service. Send "exit" for exit)");
                        auto responce = socket->RecvMessage();
                        while(responce != "exit") {
                            out::PrintMessage(responce, out::ECHO_SERVICE);
                            socket->SendMessage(responce);
                            responce = socket->RecvMessage();
                        }
                    }    
                    break;    
                case TCPSocket::FILE:
                    {
                        socket->SendMessage("Send file name");

                        auto file_name = socket->RecvMessage();
                        std::ifstream fs(file_name, std::ifstream::binary);
                        if(!fs.is_open())
                        {
                            socket->SendMessage(std::to_string(TCPSocket::FILE_NOT_EXIST).data());
                            fs.close();
                            code = TCPSocket::EXIT;
                            break;
                        }
                        socket->SendMessage(std::to_string(TCPSocket::FILE_EXIST).data());
                        
                        if(std::stoi(socket->RecvMessage()) != TCPSocket::GET_FILE_SIZE)
                        {
                            fs.close();
                            code = TCPSocket::EXIT;
                            break;
                        }
                        fs.seekg(0, std::ios::end);
                        std::string file_size = std::to_string(fs.tellg());
                        socket->SendMessage(file_size);

                        if(std::stoi(socket->RecvMessage()) != TCPSocket::GET_FILE) 
                        {
                            fs.close();
                            code = TCPSocket::EXIT;
                            break;
                        }

                        out::PrintMessage("Send file " + file_name, out::FILE_SERVICE);
                        fs.seekg(0, std::ios::beg);
                        socket->SendFile(fs);
                        fs.close();
                    }   
                    break;
                case TCPSocket::ABOUT:
                    {   
                        socket->SendMessage("TCP file/echo server\n");
                    }
                    break;
                case TCPSocket::EXIT:
                    {
                        out::PrintMessage("Try to close client socket", out::INFO);   
                    }
                    break;
                default:
                    {
                        socket->SendMessage("Command not found\n");
                    }
                    break;
            }
        }
        catch(int e)
        {
            if(e = ECONNRESET)
                out::PrintMessage("Client socket was closed", out::INFO);
            else
                out::PrintMessage(std::strerror(e), out::ERROR);
            code = TCPSocket::EXIT;
        }
        catch(std::invalid_argument)
        {
            out::PrintMessage("Invalid argument", out::ERROR);
            code = TCPSocket::EXIT;
        }
    }
    out::PrintMessage("Unconnect " + socket->GetAddress() + 
        ":" + std::to_string(socket->GetPort()), out::INFO);
    socket->Shutdown();
    socket.reset();
    count_connections.fetch_sub(1);
}

std::atomic<bool> TCPServer::is_server_run { true };
std::atomic<int> TCPServer::signal_i { 0 };

void TCPServer::SignalHandler(int sig) {
    is_server_run.exchange(false);
    signal_i.exchange(sig);
}

void TCPServer::SetSignalHandler() {
    struct sigaction action = {
        action.sa_handler = SignalHandler
    };

    sigfillset(&action.sa_mask);

    sigaction(SIGHUP,  &action, NULL);
    sigaction(SIGINT,  &action, NULL);
    sigaction(SIGTERM, &action, NULL);
    sigaction(SIGABRT, &action, NULL);
    sigaction(SIGQUIT, &action, NULL);
}

void TCPServer::RunLoopWaitSignal() {
    while (TCPServer::is_server_run) {
        std::this_thread::sleep_for(std::chrono::seconds(2));
    }
}

void TCPServer::RunSignalHandler() {
    SetSignalHandler();
    RunLoopWaitSignal();
    out::PrintMessage(strsignal(signal_i), out::INFO);

    Shutdown();

    out::PrintMessage("Wait to close connections...", out::STATUS);
    for(auto &client : clients) {
        if(client)
            client->Shutdown();
    }
    thread_p.Stop();

    out::PrintMessage("Server was closed", out::INFO);
}

void TCPServer::Shutdown() {
    server_socket->Shutdown();
    out::PrintMessage("Shutdown server socket", out::INFO);
}