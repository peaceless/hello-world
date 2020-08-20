#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/select.h>
#include <unistd.h>

#include <iostream>
#include <vector>

// OPEN_MAX was in limits.h to get file's maximum quantity could be opened
// once,but now it is in sys/source.h,e.g it replaced by RLIMIT_NOFILE
#define OPEN_MAX 1024
void UseSelect(int server_socket);

int main()
{
    int server = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (server < 0)
    {
        std::cout << "Create socket fail." << std::endl;
    }

    sockaddr_in sockaddr;
    // set bits to zero
    bzero(&sockaddr, sizeof(sockaddr));

    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = htons(1234);
    sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (bind(server, (struct sockaddr *)&sockaddr, sizeof(sockaddr)) == -1)
    {
        std::cout << "Bind fail" << std::endl;
    }

    if (listen(server, 10) == -1)
    {
        std::cout << "Listen fail" << std::endl;
    }
    UseSelect(server);
    return 0;
}

void UseSelect(int serve_socket_fd)
{
    int maxfd = serve_socket_fd;
    int index = -1;
    int clients[FD_SETSIZE];

    for (int i = 0; i < FD_SETSIZE; i++)
    {
        clients[i] = -1;
    }

    fd_set rest, allset;
    FD_ZERO(&allset);
    FD_SET(serve_socket_fd, &allset);

    int n_ready;
    int socketfd;
    char receive[100];
    int i;
    while (true)
    {
        rest = allset;
        n_ready = select(maxfd + 1, &rest, NULL, NULL, NULL);
        if (n_ready == 0)
            continue;
        else if (n_ready < 0)
        {
            std::cout << "select error" << std::endl;
            exit(-1);
        }
        if (FD_ISSET(serve_socket_fd, &rest))
        {
            int client_socketfd;
            if ((client_socketfd = accept(serve_socket_fd, NULL, NULL)) < 0)
            {
                if (errno == EINTR)
                {
                    continue;
                }
                else
                    std::cout << "error accept" << std::endl;
            }

            for (i = 0; i < FD_SETSIZE; i++)
            {
                if (clients[i] < 0)
                {
                    clients[i] = client_socketfd;
                    break;
                }
            }
            if (i == FD_SETSIZE)
            {
                std::cerr << "too many clients" << std::endl;
                continue;
            }
            FD_SET(client_socketfd, &allset);
            if (client_socketfd > maxfd)
            {
                maxfd = client_socketfd;
            }
            if (i > index)
            {
                index = i;
            }
            if (--n_ready <= 0)
            {
                continue;
            }
        }
        bzero(receive, 100);
        for (int i = 0; i <= index; i++)
        {
            if ((socketfd = clients[i]) < 0)
            {
                continue;
            }
            if (FD_ISSET(socketfd, &rest))
            {
                int n = -1;
                if ((n = read(socketfd, receive, 2)))
                {
                    std::cout << "send buffer : " << receive << std::endl;
                    if (write(socketfd, receive, n) <= 0)
                    {
                        std::cerr << "the client is disconnected" << std::endl;
                        close(socketfd);
                        FD_CLR(socketfd, &allset);
                        clients[i] = -1;
                        n = 1;
                        break;
                    }
                }
                else
                {
                    std::cerr << "the client is disconnected" << std::endl;
                    close(socketfd);
                    FD_CLR(socketfd, &allset);
                    clients[i] = -1;
                }
                if (--n_ready <= 0)
                {
                    break;
                }
            }
        }
    }
}
