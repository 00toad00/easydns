#pragma once

#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <cstring>

class TcpSocket {
    private:
        int fd;
        int af;

        struct sockaddr_in  addr4;
        struct sockaddr_in6 addr6;

    public:
        TcpSocket();
        ~TcpSocket();

        int createV4();
        int createV6();
        int close();

        int setSockOpt(int opt);

        int bind(struct sockaddr *addr);
        int bind(char *ip, unsigned short port);

        int listen(int backlog);
};