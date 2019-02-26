//Server.cpp --create a server to wait for Client's link requirements
#include <iostream>
#include <cstring> //use bzero
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
//declration class Server
class Server {
private:
    int serv_sock;
    char receive[100];
//store configuration of server
    sockaddr_in serv_addr;
//store configuration of client
    sockaddr_in clnt_addr;
    socklen_t clnt_addr_size;
    pid_t childpid;
public:
    void init();
    void wait();
};

void Server::init()
{
    //                 family, type,       protocal
    serv_sock = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    bzero(&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(1234);
}

void Server::wait()
{
    //bind socket with address
    bind(serv_sock,(struct sockaddr *)&serv_addr,sizeof(serv_addr));
    //listen the port and define max amount of link
    listen(serv_sock,20);

    while(1){
        clnt_addr_size = sizeof(clnt_addr);
        //accept return a new socket's hanlder,use this communicate with client
        int clnt_sock = accept(serv_sock,(struct sockaddr *)&clnt_addr,&clnt_addr_size);
        if ((childpid = fork()) == 0){
            read(clnt_sock,receive,100);
            std::cout << receive << std::endl;
            write(clnt_sock,receive,sizeof(receive));
            bzero(receive,sizeof(receive));
            exit(0);
        }
        close(clnt_sock);
    }
}

int main()
{
    Server my_server;
    my_server.init();
    my_server.wait();
    return 0;
}
