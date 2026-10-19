#pragma once

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>

class TcpSocket {
    private:
        int fd;
    public:
        int create();
};