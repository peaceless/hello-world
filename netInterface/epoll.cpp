#include <iostream>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <errno.h>

#include <cstring>

#define MAXLINE 5
#define OPEN_MAX 1024
#define LINSTENQ 20
#define SERV_PORT 8000
#define INFTIM 1000

void SetNonBlocking(int sock)
{
    int opts;
    opts = fcntl(sock, F_GETFL);
    if (opts < 0)
    {
        perror("fcntl(sock, GETFL)");
        return;
    }
    opts = opts | O_NONBLOCK;
    if (fcntl(sock, F_SETFL, opts) < 0)
    {
        perror("error");
        return;
    }
}

int main(int argc, char* argv[])
{
    int i, maxi, listenfd, connfd, sockfd, epfd, nfds, portnumber;
    ssize_t n;
    char line[MAXLINE];
    socklen_t clilen;

    if (2 == argc)
    {
        if ((portnumber = atoi(argv[1]) < 0))
        {
            fprintf(stderr, "Usage:%s portnumber/a/n", argv[0]);
            return 1;
        }
    }
    else
    {
        fprintf(stderr, "Usage:%s portnumber/a/n", argv[0]);
    }
    
    // claim epoll_event struct, ev used for registing events
    // array is for storing callback events
    struct epoll_event ev, events[OPEN_MAX];

    // file description epfd is handle for epoll,
    // all events will registed to epfd though epoll_ctl
    // 256 is max length epfd could handle.
    // after Linux 2.6.8, 256 is ignored.
    epfd = epoll_create(256);

    struct sockaddr_in clientaddr;
    struct sockaddr_in serveraddr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);

    // set listenfd into epoll struct
    ev.data.fd = listenfd;
    // EPOLLET set model to et
    ev.events = EPOLLIN | EPOLLET;

    // registe events
    epoll_ctl(epfd, EPOLL_CTL_ADD, listenfd, &ev);

    bzero(&serveraddr, sizeof(serveraddr));
    serveraddr.sin_family = AF_INET;
    serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serveraddr.sin_port = htons(portnumber);
    /*
     * char *local_addr = "127.0.0.1";
     * inet_aton(local_addr, &(serveraddr.sin_addr));
     */

    bind(listenfd, (sockaddr*)&serveraddr, sizeof(serveraddr));
    listen(listenfd, LINSTENQ);
    maxi = 0;

    // deal epoll events
    for (;;)
    {
        // wait epoll happens
        // nfds is the count of happen events
        nfds = epoll_wait(epfd, events, sizeof(events), 500);

        for (i = 0; i < nfds; i++)
        {
            if (events[i].data.fd == listenfd)
            {
                connfd = accept(listenfd, NULL, NULL);
                if (connfd < 0)
                {
                    perror("connfd < 0");
                    return -1;
                }
                ev.data.fd = connfd;
                ev.events = EPOLLIN | EPOLLET;

                epoll_ctl(epfd, EPOLL_CTL_ADD, connfd, &ev);
            }
            else if (events[i].events & EPOLLIN)
            {
                if ((sockfd = events[i].data.fd) < 0)
                {
                    continue;
                }
                if ((n = read(sockfd, line, MAXLINE)) < 0)
                {
                    if (errno == ECONNRESET)
                    {
                        close(sockfd);
                        events[i].data.fd = -1;
                    }
                    else
                    {
                        std::cout << "read line error." << std::endl;
                    }
                }
                else if (n == 0)
                {
                    close(sockfd);
                    events[i].data.fd = -1;
                }
                line[n] = '\0';
                std::cout << "read" << line << std::endl;
                ev.data.fd = sockfd;
                ev.events = EPOLLOUT | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
            }
            else if(events[i].events & EPOLLOUT)
            {
                sockfd = events[i].data.fd;
                write(sockfd, line, n);
                ev.data.fd = sockfd;
                ev.events = EPOLLIN | EPOLLET;
                epoll_ctl(epfd, EPOLL_CTL_MOD, sockfd, &ev);
            }
        }
    }
}