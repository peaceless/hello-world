#ifndef _POLL_H_
#define _POLL_H_

#include "model.h"
#include <poll.h>

class Poll : public Network::Server
{
public:
    Poll();
    ~Poll() override;

    void Loop() override;
    bool Handle(int i);

private:
    bool DealNewConn(Socket fd, sockaddr_in &client) override;
    bool DealErrorConn(Socket fd, int pos = -1) override;
    enum {MAX_FD = 1024};

    struct pollfd clients[MAX_FD];
    int maxfd, index;
};

#endif