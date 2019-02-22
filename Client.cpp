#include <iostream>
#include <cstdlib>
#include <memory.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
class Client {
private:
    char receive[100];
    char sent[100];
    int serv_sock;
    sockaddr_in serv_addr;
public:
    Client(std::string server_ip = "127.0.0.1");
    void connecting();
};

Client::Client(std::string server_ip)
{
    const char * ip = server_ip.c_str();
    memset(&serv_addr,0,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(ip);
    serv_addr.sin_port = htons(1234);
}

void Client::connecting()
{
    memset(&sent,0,sizeof(sent));
    memset(&receive,0,sizeof(receive));
    while (1) {
        int sock;
        if ((sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP)) < 0) {
            std::cout << "Creat socket fail!" << std::endl;
            exit(0);
        }
        if(connect(sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0) {
            std::cout << "Can't connect!" << std::endl;
            exit(0);
        }
        std::cin >> sent;
        write(sock,sent,sizeof(sent));
        read(sock,receive,sizeof(receive));
        std::cout << "message :" << receive << std::endl;
        memset(&sent,0,sizeof(sent));
        memset(&receive,0,sizeof(receive));
        close(sock);
    }
}

int main()
{
    Client my_client;
    std::string my_ip;
    std::cout << "Enter server ip you'll connect to(enter 1 use the defalut argument)" << std::endl;
    std::cin >> my_ip;
    if (my_ip == "1") {
        my_client = Client();
    } else {
        my_client = Client(my_ip);
    }
    my_client.connecting();
    return 0;
}
