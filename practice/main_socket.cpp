#include "Socket.h"

#include <string>
#include <iostream>


int main(int argc, char* argv[])
{
    if (argc < 3) 
    {
       fprintf(stderr,"usage %s hostname port\n", argv[0]);
       exit(0);
    }

    std::string ip = argv[1];
    int port = atoi(argv[2]);
    int timeout = 10000; //10 seconds
    Socket socket(ip, port, timeout);
    
    std::string request = "AABBCC00";
    std::string response;
    socket.SendAndReceive(request, response); 
    std::cout << "The response is: " << response << std::endl;
}
