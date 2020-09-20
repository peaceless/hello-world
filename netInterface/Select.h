#ifndef _SELECT_H_
#define _SELECT_H_
#include "model.h"
class Select : public Network::Server
{
public:
    Select();
    ~Select();

    void Loop() override;
    bool Handle(int i);

private:
    bool DealNewConn(Socket fd, sockaddr_in &client) override;
    bool DealErrorConn(Socket fd, int pos = -1) override;

private:
    Socket clients[FD_SETSIZE];
    int maxfd, index;
    fd_set allset;
};


#endif