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
    ConnectToServer(); 
}

Socket::~Socket()
{ 
    CloseSocket(); 
}



#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define BUF_SIZE 500

int
main(int argc, char *argv[])
{
    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s, j;
    size_t len;
    ssize_t nread;
    char buf[BUF_SIZE];

    if (argc < 3) {
        fprintf(stderr, "Usage: %s host port msg...\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    /* Obtain address(es) matching host/port */

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

    s = getaddrinfo(argv[1], argv[2], &hints, &result);
    if (s != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
        exit(EXIT_FAILURE);
    }

    /* getaddrinfo() returns a list of address structures.
       Try each address until we successfully connect(2).
       If socket(2) (or connect(2)) fails, we (close the socket
       and) try the next address. */

    for (rp = result; rp != NULL; rp = rp->ai_next) {
        sfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (sfd == -1)
            continue;

        if (connect(sfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;                  /* Success */

        close(sfd);
    }

    if (rp == NULL) {               /* No address succeeded */
        fprintf(stderr, "Could not connect\n");
        exit(EXIT_FAILURE);
    }

    freeaddrinfo(result);           /* No longer needed */

    /* Send remaining command-line arguments as separate
       datagrams, and read responses from server */

    for (j = 3; j < argc; j++) {
        len = strlen(argv[j]) + 1;
                /* +1 for terminating null byte */

        if (len + 1 > BUF_SIZE) {
            fprintf(stderr,
                    "Ignoring long message in argument %d\n", j);
            continue;
        }

        if (write(sfd, argv[j], len) != len) {
            fprintf(stderr, "partial/failed write\n");
            exit(EXIT_FAILURE);
        }

        nread = read(sfd, buf, BUF_SIZE);
        if (nread == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        printf("Received %zd bytes: %s\n", nread, buf);
    }

    exit(EXIT_SUCCESS);
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

    struct addrinfo hints;
    struct addrinfo *result, *rp;
    int sfd, s, j;
    size_t len;
    ssize_t nread;
    char buf[BUF_SIZE];

    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;    /* Allow IPv4 or IPv6 */
    hints.ai_socktype = SOCK_DGRAM; /* Datagram socket */
    hints.ai_flags = 0;
    hints.ai_protocol = 0;          /* Any protocol */

    s = getaddrinfo(m_ip, m_port, &hints, &result);
    if (s != 0) 
    {
        printf("Error getaddrinfo: %s\n", gai_strerror(s));
        return false;
    }

    /* getaddrinfo() returns a list of address structures.
       Try each address until we successfully connect(2).
       If socket(2) (or connect(2)) fails, we (close the socket
       and) try the next address. */

    for (rp = result; rp != NULL; rp = rp->ai_next) 
    {
        m_sockfd = socket(rp->ai_family, rp->ai_socktype,
                     rp->ai_protocol);
        if (m_sockfd == -1)
            continue;

        if (connect(m_sockfd, rp->ai_addr, rp->ai_addrlen) != -1)
            break;                  /* Success */

        close(m_sockfd);
    }

    if (rp == NULL) 
    {               /* No address succeeded */
        printf("Could not connect\n");
        return false;
    }

    freeaddrinfo(result);           /* No longer needed */


//===========================================
//    m_sockfd = socket(AF_INET, SOCK_STREAM, 0);
//    if (m_sockfd < 0)
//    {
//        printf("ERROR opening socket\n");
//        return false;
//    }
//
//    struct hostent* server;
//    server = gethostbyname(m_ip.c_str());
//    if (server == NULL) 
//    {
//        printf("ERROR, no such host\n");
//        return false;
//    }
//
//    struct sockaddr_in serv_addr;
//    bzero((char *) &serv_addr, sizeof(serv_addr));
//
//    serv_addr.sin_family = AF_INET;
//    bcopy((char *)server->h_addr,
//         (char *)&serv_addr.sin_addr.s_addr,
//         server->h_length);
//    serv_addr.sin_port = htons(m_port);
//
//    if (connect(m_sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
//    {
//        printf("ERROR connecting\n");
//        return false;
//    }

    return true;
}


bool Socket::SendAndReceive(const std::string& request, std::string& response)
{
    if (m_sockfd < 0)
    {
        printf("ERROR opening socket\n");
        return false;
    }

    /* Send remaining command-line arguments as separate
       datagrams, and read responses from server */

    //for (j = 3; j < argc; j++) 
    {
        const int BUFFER_SIZE = 256;
        char buffer[BUFFER_SIZE];
        bzero(buffer,BUFFER_SIZE);
    
        strcpy(buffer, request.substr(0, BUFFER_SIZE-1).c_str());
        //len = strlen(request.length()) + 1;
                /* +1 for terminating null byte */

        if (len + 1 > BUF_SIZE) 
        {
            printf("Ignoring long message in argument %d\n", j);
            continue;
        }

        if (write(m_sockfd, argv[j], len) != len) 
        {
            printf("partial/failed write\n");
            return false;
        }

        nread = read(m_sockfd, buf, BUF_SIZE);
        if (nread == -1) 
        {
            printf("Error read");
            return false;
        }

        printf("Received %zd bytes: %s\n", nread, buf);
    }

    exit(EXIT_SUCCESS);

//==================================================================
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

