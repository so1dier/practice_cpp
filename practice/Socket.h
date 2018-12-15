#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <string>

class Socket
{
public:
    explicit Socket(const std::string& ip, const int port, int timeout);
    ~Socket();
    bool SendAndReceive(const std::string& request, std::string& response);
    bool IsConnected();

private:
    bool ConnectToServer();
    bool CloseSocket();
    bool Ping();

    std::string m_ip{};
    int m_port{};
    int m_sockfd{};
    bool m_connected{};
    bool m_timeout{};
};

#endif // _SOCKET_H_
