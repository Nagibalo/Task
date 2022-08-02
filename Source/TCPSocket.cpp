#include "TCPSocket.h"

TCPSocket::TCPSocket() : address(std::make_unique<sockaddr_in>()) {    
    address->sin_family = AF_INET; // AF_INET - Internet domain sockets
    address->sin_addr.s_addr = DEFAULT_ADDRESS;
    address->sin_port = htons(DEFAULT_PORT);
}

TCPSocket::TCPSocket(int port, const std::string &ip_address, int segment_size) : 
    segment_size(segment_size), address(std::make_unique<sockaddr_in>()) {
    address->sin_family = AF_INET; // AF_INET - Internet domain sockets
    address->sin_addr.s_addr = inet_addr(ip_address.data());
    address->sin_port = htons(port);
}

TCPSocket::TCPSocket(int socket_fd, std::unique_ptr<sockaddr_in> &address, int segment_size) : 
    socket_fd(socket_fd), address(std::move(address)), segment_size(segment_size) {

}

TCPSocket::TCPSocket(TCPSocket&& socket) : socket_fd(socket.socket_fd), 
    address(std::move(socket.address)), segment_size(socket.segment_size) {
    
}

TCPSocket::TCPSocket(const TCPSocket& socket) : socket_fd(socket.socket_fd), 
    address(std::make_unique<sockaddr_in>()), segment_size(socket.segment_size) {
    address->sin_family =  socket.address->sin_family;
    address->sin_addr = socket.address->sin_addr;
    address->sin_port = socket.address->sin_port;
}

void TCPSocket::SetSocketOption(int option) {
    if (setsockopt(socket_fd, SOL_SOCKET,
                   SO_REUSEADDR | SO_REUSEPORT | SO_KEEPALIVE, &option, // SO_REUSERADDR && SO_REUSEPORT reuse port and address
                   sizeof(option)) == -1)
        throw errno;
}

void TCPSocket::CreateSocket() {
    socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP); // SOCK_STREAM - Byte-stream socket (TCP) 
    if(socket_fd == -1)
        throw errno;
}

void TCPSocket::Bind() {
    if (bind(socket_fd, (struct sockaddr*)address.get(), sizeof(*address)) == -1)
        throw errno;
}

void TCPSocket::Listen(int max_count_connection_requests) {
    if (listen(socket_fd, max_count_connection_requests) == -1)
        throw errno;
}

std::shared_ptr<TCPSocket> TCPSocket::Accept() {
    auto client_address = std::make_unique<sockaddr_in>();
    int new_socket_fd, len_address = sizeof(*client_address);
    new_socket_fd = accept(socket_fd, (struct sockaddr*)client_address.get(), (socklen_t*)&len_address);
    if (new_socket_fd == -1) 
        throw errno;
    auto newSocket = std::make_shared<TCPSocket>(new_socket_fd, client_address, segment_size);
    return newSocket;
}

void TCPSocket::Connect() {
    if (connect(socket_fd, (struct sockaddr*)address.get(), sizeof(*address)) == -1)
        throw errno;
}

void TCPSocket::SendMessage(std::string &message) {
    if(message.size() < segment_size)
    {
        if(send(socket_fd, message.data(), message.size(), 0) == -1)
            throw errno;
        return;
    }

    size_t total_sent_byte = 0;
    while(total_sent_byte != message.size())
    {
        size_t current_segment_size = message.size() - total_sent_byte > segment_size ? 
                        message.size() - total_sent_byte : segment_size;
        std::string buffer = message.substr(total_sent_byte, current_segment_size);

        size_t number_sent_byte = send(socket_fd, buffer.data(), current_segment_size, 0);
        if(number_sent_byte == -1)
            throw errno;
        total_sent_byte += number_sent_byte;
    }  
}

void TCPSocket::SendMessage(const char* message) {
    size_t len = strlen(message);
    if(len < segment_size)
    {
        if(send(socket_fd, message, len, 0) == -1)
            throw errno;
        return;
    }

    size_t total_sent_byte = 0;
    while(total_sent_byte != len)
    {
        size_t current_segment_size = len - total_sent_byte > segment_size ? 
                        len - total_sent_byte : segment_size;
        char buffer[current_segment_size + 1]; // null-terminated c-string
        for(size_t i = 0; i < current_segment_size; i++) {
            buffer[i] = message[i + total_sent_byte];
        }
        size_t number_sent_byte = send(socket_fd, buffer, current_segment_size, 0);
        if(number_sent_byte == -1)
            throw errno;
        total_sent_byte += number_sent_byte;
    }  
}

std::string TCPSocket::RecvMessage() {
    std::string data;
    char buffer[segment_size + 1] = {0}; // null-terminated c-string
    int value = 0;
    do {
        if (recv(socket_fd, buffer, segment_size, 0) <= 0)
            throw errno;
        if(ioctl(socket_fd, FIONREAD, &value) == -1)
            throw errno;
        data.append(buffer);
    }
    while(value != 0);
    return data;
}

void TCPSocket::SendFile(std::ifstream &fs) 
{
    std::string buffer(segment_size, ' ');
    while(fs)
    {
        fs.read(buffer.data(), segment_size);
        if(fs.bad())
        {
            fs.close();
            throw errno;
        }
        size_t number_sent_byte = send(socket_fd, buffer.data(), segment_size, 0);
        if(number_sent_byte == -1)
        {   
            fs.close();
            throw errno;
        }
    }  
}

void TCPSocket::DownloadFile(std::ofstream &fs, size_t file_size) 
{ 
    size_t total_recv_byte = 0;
    std::string buffer(segment_size, ' ');
    while(total_recv_byte <= file_size)
    {   
        size_t number_recv_byte = recv(socket_fd, buffer.data(), segment_size, 0);
        if (number_recv_byte == -1) 
        {
            fs.close();
            throw errno;
        }
        fs.write(buffer.data(), number_recv_byte);
        if(fs.bad())
        {
            fs.close();
            throw errno;
        }
        fs.flush();
        total_recv_byte += number_recv_byte;
    }  
}

void TCPSocket::Shutdown() {
    shutdown(socket_fd, SHUT_RDWR);
}

void TCPSocket::Close() {
    close(socket_fd);
}


int TCPSocket::GetPort() {
    return address->sin_port;
}

std::string TCPSocket::GetAddress() {
    return inet_ntoa(address->sin_addr);
}