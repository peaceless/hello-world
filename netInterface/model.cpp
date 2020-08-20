#include <iostream>

// in Linux
#include <netinet/in.h>
#include <arpa/inet.h>

#include <cstring>
#include <thread>

#define LISTENQ 10
// Warning:this worked under ipv4 and TCP
class Network {
public:
    Network();
    ~Network();

    bool InitServer(std::string& addr, int port_number);
    void ServerRun();
    bool DealNewConn(int fd, sockaddr_in* client);

private:
    int fd;
    // TODO: question is, does sockaddr_in still need after bind()?
    sockaddr_in server;
};

bool Network::InitServer(std::string& addr, int port_number)
{
    fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(addr.c_str());
    server.sin_port = htons(port_number);

    int ret;
    ret = bind(fd, reinterpret_cast<sockaddr*>(&server), sizeof(server));
    if (ret == -1)
    {
        std::cout << "Bind failed." << std::endl;
        return false;
    }
    ret = listen(fd, LISTENQ);
    if (ret == -1)
    {
        std::cout << "Listen failed." << std::endl;
        return false;
    }
    return true;
}

void Network::ServerRun()
{
    sockaddr_in client;
    socklen_t client_len = sizeof(client);
    while(true)
    {
        bzero(&client, sizeof(client));
        int new_conn = accept(fd, reinterpret_cast<sockaddr*>(&client), &client_len);
        if (!DealNewConn(new_conn, &client))
        {
            std::cout << "error happened when deal new conn." << std::endl;
            break;
        }
    }
}

bool Network::DealNewConn(int fd, sockaddr_in* client)
{}