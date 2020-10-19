#include "Select.h"
#include <thread>
#include <cstring>

int main()
{
    Select se;
    Network::Client client(false);

    std::string server_ip = "127.0.0.1";
    std::string port = "8080";
    se.CreateSock();
    TRACE("server fd is");
    TRACE(se.m_fd);
    se.InitServer(server_ip, port);
    client.Connect(server_ip, port);
    TRACE("Client fd is");
    TRACE(client.m_fd);
    se.RunAccept();

    Network::Buf buf;
    char data[10] = "hello";
    buf.buf = data;
    buf.len = std::strlen(data);
    client.Write(buf);
    std::thread server_thread(&Select::Loop, &se);
    std::thread client_thread(&Network::Client::Read, &client, 7);
    client_thread.join();
    return 0;
}