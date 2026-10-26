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

        UdpSocket(const UdpSocket&) = delete;
        UdpSocket& operator=(const UdpSocket&) = delete;

        UdpSocket(UdpSocket&& other) noexcept;
        UdpSocket& operator=(UdpSocket&& other) noexcept;

        int createV4();
        int createV6();
        int close();
        int setSockOpt(int optname);

        int bind(sockaddr *addr);
        int bind(const char *ip, unsigned short port);

        ssize_t sendTo(void *data, size_t n, sockaddr *addr);
        ssize_t sendTo(void *data, size_t n, const char *ip, unsigned short port);
        ssize_t sendTo(std::string &data, sockaddr *addr);
        ssize_t sendTo(std::string &data, const char *ip, unsigned short port);

        ssize_t recvFrom(void *buf, size_t n, sockaddr *addr, socklen_t *len);
        ssize_t recvFrom(std::string &buf, sockaddr *addr, socklen_t *len);

        int getFd();
        int getAf();
        sockaddr *getSockAddrIn();
};

inline int UdpSocket::getFd() {
    return fd;
}

inline int UdpSocket::getAf() {
    return af;
}

inline sockaddr *UdpSocket::getSockAddrIn() {
    return (af == AF_INET) ? (sockaddr*)&addr4 : (sockaddr*)&addr6;
}