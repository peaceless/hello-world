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

bool Select::DealNewConn(Socket fd, sockaddr_in &client)
{
    RequireAssert(fd.m_fd < FD_SETSIZE, false, std::cout << "Can't afford more conn." << std::endl);

    int sock_fd = fd.m_fd;
    std::cout << "new conn is " << fd.m_fd << std::endl;
    for (int i = 0; i < FD_SETSIZE && clients[i].m_fd < 0; i++)
    {
        if (clients[i].m_fd < 0)
        {
            clients[i] = fd;
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

// TODO: implement it
bool Select::DealErrorConn(Socket fd, int pos)
{
    FD_CLR(fd.m_fd, &allset);
    if (pos != -1)
    {
        clients[pos].m_fd = -1;
    }
    return true;
}

bool Select::Handle(int i)
{
    Socket sock(clients[i].m_fd);
    // char data[10] = "hello";
    // buffer.buf = data;
    // buffer.len = 5;
    // std::cout << "goint to send." << std::endl;
    // sock.Write(buffer);
    // return true;
    Network::Buf ret = sock.Read(10);
    if (ret.len > 0)
    {
        std::cout << "Socket " << sock.m_fd << " has reach msg:\n"
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

        if (FD_ISSET(m_fd, &rest))
        {
            RunAccept();
            n_ready --;
        }

        int sockfd;
        for (int i = 0; i <= index && n_ready > 0; i++)
        {
            if ((sockfd = clients[i].m_fd) < 0 | !FD_ISSET(sockfd, &rest))
            {
                continue;
            }
            Handle(i);
            n_ready--;
        }
    }
}