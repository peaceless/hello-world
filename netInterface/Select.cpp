#include "Select.h"

#include <sys/select.h>

Select::Select()
    : maxfd(-1), index(-1)
{
    FD_ZERO(&allset);
}

Select::~Select()
{
}

bool Select::DealNewConn(Socket sock, sockaddr_in &client)
{
    RequireAssert(sock.fd < FD_SETSIZE, false, std::cout << "Can't afford more conn." << std::endl);

    int sock_fd = sock.fd;
    std::cout << "new conn is " << sock.fd << std::endl;
    for (int i = 0; i < FD_SETSIZE && clients[i].fd < 0; i++)
    {
        if (clients[i].fd < 0)
        {
            clients[i] = sock;
            break;
        }
    }

    FD_SET(sock_fd, &allset);
    if (sock_fd > maxfd)
    {
        maxfd = sock_fd;
    }
    if (sock_fd > index)
    {
        index = sock_fd;
    }
    return true;
}

// TODO: add a method for put server fd into queue
// TODO: implement it
bool Select::DealErrorConn(Socket sock, int pos)
{
    FD_CLR(sock.fd, &allset);
    if (pos != -1)
    {
        clients[pos].fd = -1;
    }
    return true;
}

bool Select::Handle(int i)
{
    Socket sock(clients[i].fd);
    Network::Buf ret = sock.Read(10);
    if (ret.len > 0)
    {
        std::cout << "Socket " << sock.fd << " has reach msg:\n"
                  << ret.buf << std::endl;
        int n = Write(ret);
        if (n <= 0)
        {
            std::cout << "Write failed." << std::endl;
            DealErrorConn(sock, i);
            n = 1;
        }
    }
    else
    {
        std::cout << "Read failed.errno is " << errno << std::endl;
        DealErrorConn(sock, i);
        return false;
    }
    return true;
}

void Select::Loop()
{
    fd_set rest;
    int n_ready = 0;
    while (true)
    {
        rest = allset;
        n_ready = select(maxfd + 1, &rest, NULL, NULL, NULL);
        if (n_ready == 0)
        {
            continue;
        }
        else if (n_ready < 0)
        {
            std::cout << "Error: Select error." << std::endl;
            return;
        }

        int sockfd;
        for (int i = 0; i <= index && n_ready > 0; i++)
        {
            if ((sockfd = clients[i].fd) < 0 | !FD_ISSET(sockfd, &rest))
            {
                continue;
            }
            Handle(i);
            n_ready--;
        }
    }
}