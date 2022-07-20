#include "TCPServer.h"

TCPServer::TCPServer() {    
    address.sin_family = AF_INET; // AF_INET - Internet domain sockets
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(DEFAULT_PORT);
    //threadAwaitAcceptNewConnection = std::make_unique<std::thread>(awaitAcceptNewConnection, tcpSocket, address);
}

TCPServer::TCPServer(int port, const char *ip_address) {
    address.sin_family = AF_INET; // AF_INET - Internet domain sockets
    address.sin_addr.s_addr = inet_addr(ip_address);
    address.sin_port = htons(port);
}

bool TCPServer::createServer() {

    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // SOCK_STREAM - Byte-stream socket (TCP) 
    if(socket_fd < 0)
    {
        perror("Create socket failed");
        return false;
    }
    
  
    // Forcefully attaching socket to the port 8080
    /*if (setsockopt(socket_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &opt,
                   sizeof(opt))) {
        perror("setsockopt");
    }*/
 
    if (bind(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        return false;
    }

    if (listen(socket_fd, 3) < 0) {
        perror("Listen failed");
        return false;
    }
    
    std::cout << inet_ntoa(address.sin_addr) << '\n';
    std::cout << address.sin_port;

    
    //awaitAcceptNewConnection(tcpSocket, address);
    //threadAwaitAcceptNewConnection->detach();
    //valread = recv(new_socket, buffer, 1024, MSG_WAITALL);
    //printf("%s\n", buffer);

  // closing the connected socket
  // closing the listening socket
    //shutdown(tcpSocket, SHUT_RDWR);
    return true;
}

void TCPServer::runServer() {
    for(int i = 0; i < 2; i++)
    {
        std::cout << "Await";
        int client_socket_fd, len_address = sizeof(address);
        client_socket_fd = accept(socket_fd, (struct sockaddr*)&address, (socklen_t*)&len_address);
        if (client_socket_fd < 0) {
            perror("Accept failed");
            return;
        }
        std::cout << "New connection";

        send(client_socket_fd, "test", 4, 0);
        shutdown(client_socket_fd, SHUT_RDWR);
    }
}