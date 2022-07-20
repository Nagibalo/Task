#include "TCPClient.h"

TCPClient::TCPClient() {    
    address.sin_family = AF_INET; // AF_INET - Internet domain sockets
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(DEFAULT_PORT);
}

TCPClient::TCPClient(int port, const char *ip_address) {
    address.sin_family = AF_INET; // AF_INET - Internet domain sockets
    address.sin_addr.s_addr = inet_addr(ip_address);
    address.sin_port = htons(port);
}

bool TCPClient::connectToServer() {
    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // SOCK_STREAM - Byte-stream socket (TCP) 
    if(socket_fd < 0)
    {
        perror("create socket");
        return false;
    }

    if (connect(socket_fd, (struct sockaddr*)&address, sizeof(address)) < 0) {
        perror("bind failed");
        return false;
    }

    
    std::cout << inet_ntoa(address.sin_addr) << '\n';
    std::cout << address.sin_port;
    
    for(int i = 0; i < 1; i++) {
        char buffer[1024] = {0};
        int count_readed_byte;
        count_readed_byte = recv(socket_fd, buffer, 1024, MSG_WAITALL);
        std::cout << buffer;
    }
    shutdown(socket_fd, SHUT_RDWR);
    return true;
}