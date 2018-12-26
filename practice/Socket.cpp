#include "Socket.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

const int DEFAULT_TIMEOUT = 10000; //10 seconds

Socket::Socket(const std::string& ip, const int port, int timeout) 
    : m_ip(ip), 
      m_port(port), 
      m_timeout(timeout) 
{ 
    if (m_timeout <= 0)
    {
        m_timeout = DEFAULT_TIMEOUT;
    }
    m_connected = ConnectToServer(); 
}

Socket::~Socket()
{ 
    CloseSocket(); 
}

bool Socket::IsConnected()
{
    return m_connected;
}

bool Socket::ConnectToServer()
{
    if (m_ip.empty())
    {
        printf("ERROR ip is empty\n");
        return false;
    }

    if (m_port <=0)
    {
        printf("ERROR port is empty\n");
        return false;
    }

    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (m_sockfd < 0)
    {
        printf("ERROR opening socket\n");
        return false;
    }

    struct hostent* server;
    server = gethostbyname(m_ip.c_str());
    if (server == NULL) 
    {
        printf("ERROR, no such host\n");
        return false;
    }

    struct sockaddr_in serv_addr;
    bzero((char *) &serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(m_port);

    if (connect(m_sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    {
        printf("ERROR connecting\n");
        return false;
    }

    return true;
}


bool Socket::SendAndReceive(const std::string& request, std::string& response)
{
    if (!m_connected)
    {
        printf("ERROR calling %s when not connected", __FUNCTION__);
        return false;
    }

    if (m_sockfd < 0)
    {
        printf("ERROR opening socket\n");
        return false;
    }

    const int BUFFER_SIZE = 256;
    char buffer[BUFFER_SIZE];
    bzero(buffer,BUFFER_SIZE);

    strcpy(buffer, request.substr(0, BUFFER_SIZE-1).c_str());

    int n = write(m_sockfd,buffer,strlen(buffer));
    if (n < 0)
    {
        printf("ERROR writing to socket\n");
        return false;
    }
    
    bzero(buffer,256);
    n = read(m_sockfd,buffer,255);
    if (n < 0)
    {
        printf("ERROR reading from socket\n");
        return false;
    }

    response = buffer;
    printf(">>reply: %s\n",buffer);

    return true;
}

bool Socket::CloseSocket()
{
    if (m_sockfd < 0)
    {
        return false;
    }

    close(m_sockfd);
    return true;
}

