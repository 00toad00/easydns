#include <network/tcpsocket.hpp>

#include <cstdint>

TcpSocket::TcpSocket() {
    fd = -1;
    memset(&addr4, 0, sizeof(sockaddr_in));
    memset(&addr6, 0, sizeof(sockaddr_in6));
}

TcpSocket::~TcpSocket() {
    ::close(fd);
}

int TcpSocket::createV4() {
    af = AF_INET;
    fd = socket(AF_INET, SOCK_STREAM, 0);
    return fd;
}

int TcpSocket::createV6() {
    af = AF_INET6;
    fd = socket(AF_INET6, SOCK_STREAM, 0);
    return fd;
}

int TcpSocket::close() {
    memset(&addr4, 0, sizeof(sockaddr_in));
    memset(&addr6, 0, sizeof(sockaddr_in6));
    int rtn = ::close(fd);
    fd = -1;
    return rtn;
}

int TcpSocket::setSockOpt(int optname) {
    int o = 1;
    return setsockopt(fd, SOL_SOCKET, optname, &o, sizeof(int));
}

int TcpSocket::bind(struct sockaddr *addr) {
    socklen_t len = (af == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
    return ::bind(fd, addr, len);
}

int TcpSocket::bind(char *ip, unsigned short port) {
    if (af == AF_INET) {
        if (inet_pton(AF_INET, ip, &addr4.sin_addr) != 1) { return -1; }
        addr4.sin_family = AF_INET;
        addr4.sin_port = htons(port);
        ::bind(fd, (struct sockaddr*)&addr4, sizeof(sockaddr_in));
    } else if (af == AF_INET6) {
        if (inet_pton(AF_INET6, ip, &addr6.sin6_addr) != 1) { return -1; }
        addr6.sin6_family = AF_INET6;
        addr6.sin6_port = htons(port);
        ::bind(fd, (struct sockaddr*)&addr6, sizeof(sockaddr_in6));
    }
}

int TcpSocket::listen(int backlog) {
    return ::listen(fd, backlog);
}