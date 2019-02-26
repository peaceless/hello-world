#include <sys/socket.h>
#include <poll.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <strings.h>
#include <cstdlib>
#include <errno.h>
#include <stdio.h>
#include <iostream>
//OPEN_MAX was in limits.h to get file's maximum quantity could be opened
//once,but now it is in sys/source.h,e.g it replaced by RLIMIT_NOFILE
#define OPEN_MAX 1024
int main(int argc,char **argv)
{
    int i,maxi,listenfd,connfd,sockfd;
    int nReady;
    ssize_t n;
    char buff[100];
    socklen_t cli_len;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in cli_addr,serv_addr;
    listenfd = socket(AF_INET,SOCK_STREAM,0);
    bzero(&serv_addr,sizeof(serv_addr));

    //initialize socket address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8089);

    //bind socket name to real port
    bind(listenfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    char *p = getenv("LISTENQ");
    listen(listenfd,20);
    client[0].fd = listenfd;
    client[0].events = POLLRDNORM;//normal data in
    for (i = 1;i < OPEN_MAX;i ++) {client[i].fd = -1;}
    maxi = 0;

    for (;;) {
        //int poll(struct pollfd fds[],nfds_t nfds,int timeout);
        //diffierence between poll() and select() is that after
        //you call poll,it won't reset the set you pass in it,but
        //select will.
        nReady = poll(client,maxi+1,-1);//wait for events
        //bitand with POLLRDNORM to get eveten type
        if (client[0].revents & POLLRDNORM) {
            cli_len = sizeof(cli_addr);
            connfd = accept(listenfd,(struct sockaddr *)&cli_addr,&cli_len);
            std::cout << "new client" << std::endl;
            for (i = 1;i < OPEN_MAX;i++) {
                if (client[i].fd < 0) {
                    client[i].fd = connfd;
                    break;
                }
            }

            if (i == OPEN_MAX)
                exit(1);
            //register new event
            client[i].events = POLLRDNORM;

            if (i > maxi) {maxi = i;}
            if (--nReady <= 0) {continue;}
        }
        //check all the fd registed
        for (i = 1;i <= maxi;i++) {
            if ((sockfd = client[i].fd) < 0) {continue;}
            if (client[i].revents & (POLLRDNORM | POLLERR)) {
                if ((n = read(sockfd,buff,2)) < 0) {
                    if (errno == ECONNRESET) {
                        std::cout << "client[" << i <<"] aborted connection\n";
                        close(sockfd);
                        client[i].fd = -1;
                    } else {exit(1);}
                } else if (n == 0) {
                    std::cout << "client[" << i <<"]closed connection\n";
                    close(sockfd);
                    client[i].fd = -1;
                } else {write(sockfd,buff,n);}
                if (--nReady <= 0) {break;}
            }
        }
    }
}
