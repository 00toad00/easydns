#include <network/udpsocket.hpp>

#include <cstring>
#include <unistd.h>

static inline bool fd_is_alive(int fd) {
    return fd >= 0;
}

UdpSocket::UdpSocket() {
    fd = -1;
    af = -1;
}

UdpSocket::~UdpSocket() {
    if (fd_is_alive(fd)) { ::close(fd); }
}

int UdpSocket::createV4() {
    if (!fd_is_alive(fd)) {
        memset(&addr4, 0, sizeof(sockaddr_in));
        af = AF_INET;
        fd = socket(AF_INET, SOCK_DGRAM, 0);
    }

    return fd;
}

int UdpSocket::createV6() {
    if (!fd_is_alive(fd)) {
        memset(&addr6, 0, sizeof(sockaddr_in6));
        af = AF_INET6;
        fd = socket(AF_INET6, SOCK_DGRAM, 0);
    }

    return fd;
}

int UdpSocket::close() {
    if (!fd_is_alive(fd)) { return 0; }
    int i = ::close(fd);
    fd = -1;
    af = -1;
    return i;
}

int UdpSocket::setSockOpt(int optname) {
    if (!fd_is_alive(fd)) { return -1; }
    int opt = 1;
    return setsockopt(fd, SOL_SOCKET, optname, &opt, sizeof(int));
}

int UdpSocket::bind(sockaddr *addr) {
    if (!fd_is_alive(fd)) { return -1; }
    socklen_t len = (af == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
    return ::bind(fd, addr, len);
}

int UdpSocket::bind(const char *ip, unsigned short port) {
    if (!fd_is_alive(fd)) { return -1; }
    if (af == AF_INET) {
        if  (!inet_pton(AF_INET, ip, &addr4.sin_addr)) { return -1; }
        addr4.sin_family = AF_INET;
        addr4.sin_port = htons(port);
        return ::bind(fd, (sockaddr*)&addr4, sizeof(sockaddr_in));
    } else if (af == AF_INET6) {
        if (!inet_pton(AF_INET6, ip, &addr6.sin6_addr)) { return -1; }
        addr6.sin6_family = AF_INET6;
        addr6.sin6_port = htons(port);
        return ::bind(fd, (sockaddr*)&addr6, sizeof(sockaddr_in6));
    } return 0;
}

ssize_t UdpSocket::sendTo(void *data, size_t n, sockaddr *addr) {
    if (!fd_is_alive(fd)) { return -1; }
    socklen_t len = (af == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
    return sendto(fd, data, n, 0, addr, len);
}

ssize_t UdpSocket::sendTo(void *data, size_t n, const char *ip, unsigned short port) {
    if (!fd_is_alive(fd)) { return -1; }
    socklen_t len = (af == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
    sockaddr_in caddr4;
    sockaddr_in6 caddr6;
    sockaddr *addr = (af == AF_INET) ? (sockaddr*)&caddr4 : (sockaddr*)&caddr6;
    if (af == AF_INET) {
        memset(&caddr4, 0, len);
        caddr4.sin_port = htons(port);
        caddr4.sin_family = AF_INET;
        if (!inet_pton(AF_INET, ip, &caddr4.sin_addr)) { return -1; }
    } else if (af == AF_INET6) {
        memset(&caddr6, 0, len);
        caddr6.sin6_port = htons(port);
        caddr6.sin6_family = AF_INET6;
        if (!inet_pton(AF_INET6, ip, &caddr6.sin6_addr)) { return -1; }
    }
    return sendto(fd, data, n, 0, addr, len);
}

ssize_t UdpSocket::sendTo(std::string &data, sockaddr *addr) {
    if (!fd_is_alive(fd)) { return -1; }
    socklen_t len = (af == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
    return sendto(fd, data.data(), data.size(), 0, addr, len);
}

ssize_t UdpSocket::sendTo(std::string &data, const char *ip, unsigned short port) {
    if (!fd_is_alive(fd)) { return -1; }
    socklen_t len = (af == AF_INET) ? sizeof(sockaddr_in) : sizeof(sockaddr_in6);
    sockaddr_in caddr4;
    sockaddr_in6 caddr6;
    sockaddr *addr = (af == AF_INET) ? (sockaddr*)&caddr4 : (sockaddr*)&caddr6;
    if (af == AF_INET) {
        memset(&caddr4, 0, len);
        caddr4.sin_port = htons(port);
        caddr4.sin_family = AF_INET;
        if (!inet_pton(AF_INET, ip, &caddr4.sin_addr)) { return -1; }
    } else if (af == AF_INET6) {
        memset(&caddr6, 0, len);
        caddr6.sin6_port = htons(port);
        caddr6.sin6_family = AF_INET6;
        if (!inet_pton(AF_INET6, ip, &caddr6.sin6_addr)) { return -1; }
    }
    return sendto(fd, data.data(), data.size(), 0, addr, len);
}

ssize_t UdpSocket::recvFrom(void *buf, size_t n, sockaddr *addr, socklen_t *len) {
    if (!fd_is_alive(fd)) { return -1; }
    return recvfrom(fd, buf, n, 0, addr, len);
}

ssize_t UdpSocket::recvFrom(std::string &buf, sockaddr *addr, socklen_t *len) {
    if (!fd_is_alive(fd)) { return -1; }
    return recvfrom(fd, buf.data(), buf.size(), 0, addr, len);
}