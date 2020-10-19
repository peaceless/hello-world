#include "Poll.h"

Poll::Poll()
{
}

Poll::~Poll()
{
}

bool Poll::DealErrorConn(Socket fd, int pos)
{
    if (pos != -1)
    {
        clients[pos].fd = -1;
    }
    return true;
}

bool Poll::Handle(int i)
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
        else
        {
            std::cout << "Read failed.errno is " << errno << std::endl;
            DealErrorConn(sock, i);
            return false;
        }
        return true;
        
    }
}

void Poll::Loop()
{
    int n_ready = 0;
    while (true)
    {
        n_ready = poll(clients, maxfd + 1, -1);
        if (0 == n_ready)
        {
            continue;
        }
        else if (0 > n_ready)
        {
            std::cout << "Error: Select error." << std::endl;
            return;
        }

        int sockfd;
        for (int i = 0; i <= index && n_ready > 0; i++)
        {
            if ((sockfd = clients[i].fd) < 0)
            {
                continue;
            }
            Handle(i);
            n_ready--;
        }
    }
}