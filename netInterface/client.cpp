#include "model.h"

#include <arpa/inet.h>
#include <cstring>
// class Client : public Network::Socket{
// public:
//     Client();
//     ~Client();

//     bool Connect(std::string& server_ip, std::string& port);
// };

Network::Client::Client(bool isNonBlock)
{
    std::cout << "Client create." << std::endl;
}
Network::Client::~Client()
{
    std::cout << "Client destructure." << std::endl;
}
bool Network::Client::Connect(std::string& server_ip, std::string& port)
{
    sockaddr_in serv_addr;
    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(server_ip.c_str());
    serv_addr.sin_port = htons(std::stoi(port.c_str()));

    CreateSock();
    int ret = connect(m_fd, reinterpret_cast<struct sockaddr*>(&serv_addr), sizeof(serv_addr));
    RequireAssert(ret != -1, false, TRACE("Error: Connect error."));
    TRACE("Connection established.");
    return true;
}