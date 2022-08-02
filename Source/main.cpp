#include "TCPServer.h"

int main(int argc, char const *argv[]) 
{
    TCPServer server(8080, "127.0.0.1");
    if(!server.CreateServer())
    {  
       return -1; 
    }
    server.RunServer();
    return 0;
}