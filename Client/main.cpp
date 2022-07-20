#include <iostream>
#include "TCPClient.h"

 using namespace std;

int main(int argc, char const *argv[]) 
{
    TCPClient client(8080, "127.0.0.1");
    client.connectToServer();
    return 0;
}