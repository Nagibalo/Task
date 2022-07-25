#include "TCPSocket.h"

TCPSocket::TCPSocket() {    
    address.sin_family = AF_INET; // AF_INET - Internet domain sockets
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(DEFAULT_PORT);
}

TCPSocket::TCPSocket(int port, const std::string &ip_address) {
    address.sin_family = AF_INET; // AF_INET - Internet domain sockets
    address.sin_addr.s_addr = inet_addr(ip_address.data());
    address.sin_port = htons(port);
}

TCPSocket::TCPSocket(int socket_fd, const struct sockaddr_in& address) : socket_fd(socket_fd), 
                                                                        address(address) {
    
}

TCPSocket::TCPSocket(const TCPSocket& socket) : socket_fd(socket.socket_fd), 
                                                address(socket.address) {
    
}

void TCPSocket::SetSocketOption(int option) {
    if (setsockopt(socket_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT, &option,
                   sizeof(option)) == -1)
        throw errno;
}

void TCPSocket::CreateSocket() {

    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // SOCK_STREAM - Byte-stream socket (TCP) 
    if(socket_fd == -1)
        throw errno;
}

void TCPSocket::Bind() {
    if (bind(socket_fd, (struct sockaddr*)&address, sizeof(address)) == -1)
        throw errno;
}

void TCPSocket::Listen(int max_connection_request) {
    if (listen(socket_fd, max_connection_request) == -1)
        throw errno;
}

TCPSocket TCPSocket::Accept() {
    sockaddr_in client_address;
    int new_socket_fd, len_address = sizeof(client_address);
    new_socket_fd = accept(socket_fd, (struct sockaddr*)&client_address, (socklen_t*)&len_address);
    if (new_socket_fd == -1) 
        throw errno;
    TCPSocket newSocket(new_socket_fd, client_address);
    return newSocket;
}

void TCPSocket::Connect() {
    if (connect(socket_fd, (struct sockaddr*)&address, sizeof(address)) == -1)
        throw errno;
}

void TCPSocket::SendMessage(std::string &message) {
    if(message.size() < MAX_PACKET_SIZE)
    {
        if(send(socket_fd, message.data(), message.size(), 0) == -1)
            throw errno;
        return;
    }

    size_t total_sent_byte = 0;
    while(total_sent_byte != message.size())
    {
        int segment_size = message.size() - total_sent_byte > MAX_PACKET_SIZE ? 
                        message.size() - total_sent_byte : MAX_PACKET_SIZE;
        std::string segment_data = message.substr(total_sent_byte, segment_size);

        size_t number_sent_byte = send(socket_fd, segment_data.data(), segment_size, 0);
        if(number_sent_byte == -1)
            throw errno;
        total_sent_byte += number_sent_byte;
    }  
}

void TCPSocket::SendMessage(const char* message) {
    int len = strlen(message);
    if(len < MAX_PACKET_SIZE)
    {
        if(send(socket_fd, message, len, 0) == -1)
            throw errno;
        return;
    }

    size_t total_sent_byte = 0;
    while(total_sent_byte != len)
    {
        int segment_size = len - total_sent_byte > MAX_PACKET_SIZE ? 
                        len - total_sent_byte : MAX_PACKET_SIZE;
        char *buffer = new char[segment_size];
        strcpy(buffer, message);
        size_t number_sent_byte = send(socket_fd, buffer, segment_size, 0);
        if(number_sent_byte == -1)
            throw errno;
        total_sent_byte += number_sent_byte;
    }  
}

std::string TCPSocket::RecvMessage() {
    /*int number_readed_byte;
    std::string data;
    while(number_readed_byte) {
        char buffer[MAX_PACKET_SIZE] = {0};
        number_readed_byte = recv(socket_fd, buffer, MAX_PACKET_SIZE, 0);
        if (number_readed_byte == -1)
            throw errno;
        data += buffer;
    }*/
    char buffer[MAX_PACKET_SIZE] = {0};
    if (recv(socket_fd, buffer, MAX_PACKET_SIZE, 0) == -1)
        throw errno;
    std::string data(buffer);
    return data;
}

void TCPSocket::ShutDown() {
    shutdown(socket_fd, SHUT_RDWR);
}

int TCPSocket::GetPort() {
    return address.sin_port;
}

std::string TCPSocket::GetAddress() {
    return inet_ntoa(address.sin_addr);
}