#pragma once

#include <sys/socket.h>
#include <arpa/inet.h>
#include <string>

class UdpSocket {
    private:
        int fd;
        int af;

        struct sockaddr_in  addr4;
        struct sockaddr_in6 addr6;

    public:
        UdpSocket();
        ~UdpSocket();

        int createV4();
        int createV6();
        int close();
        int setSockOpt(int optname);

        int bind(sockaddr *addr);
        int bind(const char *ip, unsigned short port);

        void sendTo(void *buf, size_t n, struct sockaddr *addr);
        void sendTo(std::string, struct sockaddr *addr);
};