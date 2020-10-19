#ifndef _MODEL_H_
#define _MODEL_H_
// in Linux
#include <netinet/in.h>

#include <string>

#include <iostream>
// TODO: error code in errno
#define RequireAssert(con, ret, msg) \
{                                    \
    if (!(con))                        \
    {                                \
        msg;                         \
        return ret;                  \
    }                                \
}                                    
#define LISTENQ 10
#define BUFFERSIZE 128
#define TRACE(args) std::cout << args << std::endl
// Warning:this worked under ipv4 and TCP
namespace Network
{
    struct Buf {
        char* buf;
        ssize_t len;
    };

    class Socket
    {
    public:
        Socket();
        explicit Socket(int fd);
        bool CreateSock();
        virtual ~Socket();
        
        bool SetNonBlock();
        bool Close();

        virtual Buf Read(int n);
        virtual ssize_t Write(Buf& write_buf);

    public:
        int fd;
        bool is_nonblock;
    
    private:
        char m_read_buf[BUFFERSIZE];
    };
    class Server : public Socket
    {
    public:
        Server();
        ~Server() override;

        bool InitServer(std::string &addr, std::string &port_number);
        void RunAccept();
        virtual void Loop() = 0;
    private:
        virtual bool DealNewConn(Socket fd, sockaddr_in& client) = 0;
        virtual bool DealErrorConn(Socket fd, int pos = -1) = 0;

    private:
        // TODO: question is, does sockaddr_in still need after bind()?
        sockaddr_in server;
    };

    class Client : public Socket
    {
    public:
        Client(bool isNonBlock);
        ~Client() override;
        bool Connect(std::string& server_ip, std::string& port);
    };
} // namespace Network
#endif