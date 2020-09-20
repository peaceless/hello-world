#include <unistd.h>
#include <arpa/inet.h>

#include <fcntl.h>

#include <cstring>
#include "model.h"

Network::Socket::Socket()
    : m_fd(-1)
{
}
Network::Socket::Socket(int fd)
    : m_fd(fd)
{
}
bool Network::Socket::CreateSock()
{
    RequireAssert(m_fd <= -1, false, TRACE("There is a socket already."));
    m_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    RequireAssert(m_fd != -1, false, TRACE("Socket create failed."));
    return true;
}

Network::Socket::~Socket()
{
    RequireAssert(m_fd != -1, , TRACE("Socket is empty."));
    // Close();
}

// TODO: what if I closed a socked with already closed?
bool Network::Socket::Close()
{
    RequireAssert(m_fd != -1, false, TRACE("Error: Socket num is -1."));
    close(m_fd);
    return true;
}

Network::Buf Network::Socket::Read(int n)
{
    int size = n > BUFFERSIZE ? BUFFERSIZE : n;
    TRACE("Info : going to read");
    TRACE(size);
    TRACE(" bytes.");
    return {m_read_buf, read(m_fd, m_read_buf, size)};
}

ssize_t Network::Socket::Write(Buf &write_buf)
{
    RequireAssert(write_buf.len > 0, -1, TRACE("Error: try to read negitive length."));
    return write(m_fd, write_buf.buf, write_buf.len);
}

Network::Server::Server()
{
    CreateSock();
}

Network::Server::~Server()
{}

bool Network::Server::InitServer(std::string &addr, std::string &port_number)
{
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = inet_addr(addr.c_str());
    server.sin_port = htons(std::stoi(port_number.c_str()));

    int ret;
    ret = bind(m_fd, reinterpret_cast<sockaddr *>(&server), sizeof(server));
    RequireAssert(ret != -1, false, TRACE("Bind failed."));
    ret = listen(m_fd, LISTENQ);
    RequireAssert(ret != -1, false, TRACE("Listen failed."));
    return true;
}

void Network::Server::RunAccept()
{
    sockaddr_in client;
    socklen_t client_len = sizeof(client);
    bzero(&client, sizeof(client));
    int new_conn = accept(m_fd, reinterpret_cast<sockaddr *>(&client), &client_len);
    Socket new_sock(new_conn);
    new_sock.SetNonBlock();
    bool ret = DealNewConn(new_sock, client);
    RequireAssert(ret, , TRACE("Error: Deal new conn error."));
}

inline bool Network::Socket::SetNonBlock()
{
    RequireAssert(m_fd > -1, false, TRACE("Error: Socket num is " << m_fd));
    int opts;
    opts = fcntl(m_fd, F_GETFL);
    RequireAssert(opts >= 0, false, TRACE("Error: Get socket flag failed."));

    opts = opts | O_NONBLOCK;
    opts = fcntl(m_fd, F_SETFL, opts);
    RequireAssert(opts >= 0, false, TRACE("Error: Set socket flag failed."));

    return true;
}