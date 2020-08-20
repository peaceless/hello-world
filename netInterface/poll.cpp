#include <unistd.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cerrno>

#include <poll.h>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <iostream>

#define OPEN_MAX 1024
int main(int argc, char **argv)
{
    int i, maxi, listenfd, connfd, sockfd;
    int nReady;
    ssize_t n;
    char buff[100];
    socklen_t cli_len;
    struct pollfd client[OPEN_MAX];
    struct sockaddr_in cli_addr, serv_addr;
    listenfd = socket(AF_INET, SOCK_STREAM, 0);
    bzero(&serv_addr, sizeof(serv_addr));

    //initialize socket address
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_port = htons(8089);

    //bind socket name to real port
    bind(listenfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
    listen(listenfd, 20);
    client[0].fd = listenfd;
    client[0].events = POLLRDNORM; //normal data in
    for (i = 1; i < OPEN_MAX; i++)
    {
        client[i].fd = -1;
    }
    maxi = 0;

    for (;;)
    {
        // int poll(struct pollfd fds[],nfds_t nfds,int timeout);
        // diffierence between poll() and select() is that after
        // you call poll,it won't reset the set you pass in it,but
        // select will.
        std::cout << "Ready listen.\n";
        nReady = poll(client, maxi + 1, -1); //wait for events
        std::cout << "Ready socket counts: " << nReady;
        // if receive a connect
        if (client[0].revents & POLLRDNORM)
        {
            cli_len = sizeof(cli_addr);
            // in struct cli_addr, stored msg of peer port such as ip
            // connfd = accept(listenfd, NULL, NULL);
            connfd = accept(listenfd, (struct sockaddr *)&cli_addr, &cli_len);
            std::cout << "New client connected." << std::endl;
            for (i = 1; i < OPEN_MAX; i++)
            {
                if (client[i].fd < 0)
                {
                    client[i].fd = connfd;
                    break;
                }
            }

            if (i == OPEN_MAX)
            {
                std::cout << "Too many clients." << std::endl;
            }
            //register new event
            client[i].events = POLLRDNORM;

            if (i > maxi)
            {
                maxi = i;
            }
            std::cout << "Biggest socket is: " << maxi << std::endl;
            if (--nReady <= 0)
            {
                continue;
            }
        }
        std::cout << "Client event." << std::endl;
        for (i = 1; i <= maxi; i++)
        {
            if ((sockfd = client[i].fd) < 0)
            {
                continue;
            }
            if (client[i].revents & (POLLRDNORM | POLLERR))
            {
                std::cout << "Client[" << i << "]";
                if ((n = read(sockfd, buff, 2)) < 0)
                {
                    if (errno == ECONNRESET)
                    {
                        std::cout << " reset connection." << std::endl;
                    }
                    else
                    {
                        std::cout << " error." << std::endl;
                    }
                    close(sockfd);
                    client[i].fd = -1;
                }
                else if (n == 0)
                {
                    std::cout << " closed." << std::endl;
                    close(sockfd);
                    client[i].fd = -1;
                }
                else
                {
                    write(sockfd, buff, n);
                }
                if (--nReady <= 0)
                {
                    break;
                }
            }
       }
    }
}
