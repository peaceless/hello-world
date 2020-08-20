//Client.cpp impliment a Client link to server
#include <iostream>
// net
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <errno.h>
// vector and multi-threads
#include <vector>
#include <thread>
// string and bzero()
#include <string>
#include <cstring>

#define NUM 10
class Client
{
public:
    Client(std::string server_ip = "127.0.0.1", std::string port = "8080");
    void connecting();

private:
    sockaddr_in serv_addr;
};

Client::Client(std::string server_ip, std::string port)
{
    const char *ip = server_ip.c_str();
    bzero(&serv_addr, sizeof(serv_addr));

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(std::stoi(port.c_str()));
}

void Client::connecting()
{
    int sock;
    if ((sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0)
    {
        std::cout << "Create socket fail!" << std::endl;
        return;
    }
    std::cout << "create client " << sock << std::endl;
    if (connect(sock, reinterpret_cast<struct sockaddr *>(&serv_addr), sizeof(serv_addr)) < 0)
    {
        std::cout << "Can't connect!" << std::endl;
        std::cout << "errno is " << errno << std::endl;
        return;
    }
    std::cout << "Link established." << std::endl;

    // Init receive buffer
    char remsg[100] = "\0";
    write(sock, "GET /index.html HTTP/1.1\r\n\r\n", 28);
    read(sock, remsg, 100);
    std::cout << remsg << std::endl;

    close(sock);
}

int main()
{
    Client my_client;
    std::vector<std::thread> vect;
    for (int i = 0; i < NUM; i++)
    {
        vect.emplace_back(&Client::connecting, &my_client);
    }
    for (int i = 0; i < NUM; i++)
        vect[i].join();
    return 0;
}
