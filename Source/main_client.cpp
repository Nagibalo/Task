#include "TCPClient.h"

int main(int argc, char const *argv[]) 
{
    TCPClient client(8080, "127.0.0.1");
    if(client.ConnectToServer())
        client.RunCommunication();
    return 0;
}