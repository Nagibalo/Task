#include <iostream>
#include "TCPServer.h"

 using namespace std;

int main(int argc, char const *argv[]) 
{
    TCPServer server(8080, "127.0.0.1");
    server.shutDown();
    if(!server.createServer())
    {  
       return -1; 
    }
    server.runServer();
    return 0;
}