#include <network/listener.h>

#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>

int create_tcp_listener_v4(const char *ip, unsigned short port, int backlog, int id) {
    struct sockaddr_in addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(port);

    if (inet_pton(AF_INET, ip, &addr.sin_addr) != 1) {
        fprintf(stderr, "[%d] not a valid IPv4 address %s\n", id, ip);
        return -1;
    }

    int fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) { 
        fprintf(stderr, "[%d] socket creation failed: %m\n", id);
        return -1; 
    }

    int opt = 1;
    if(setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        fprintf(stderr, "[%d] socket setsockopt SO_REUSEADDR failed: %m\n", id);
        goto fail;
    }

    if (bind(fd, (struct sockaddr*)&addr, sizeof(addr)) < 0) { 
        fprintf(stderr, "[%d] socket bind failed: %m\n", id);
        goto fail;
    }

    if (listen(fd, backlog) < 0) {
        fprintf(stderr, "[%d] socket listen failed: %m\n", id);
        goto fail;
    }

    return fd;

    fail:
        close(fd);
        return -1;
}

int create_tcp_listener_v6(const char *ip, unsigned short port, int backlog, int id) {
    struct sockaddr_in6 addr;
    memset(&addr, 0, sizeof(addr));
    addr.sin6_family = AF_INET6;
    addr.sin6_port   = htons(port);

    if (inet_pton(AF_INET6, ip, &addr.sin6_addr) != 1) {
        fprintf(stderr, "[%d] invalid IPv6 address: %s\n", id, ip);
        return -1;
    }

    int fd = socket(AF_INET6, SOCK_STREAM, 0);
    if (fd < 0) { 
        fprintf(stderr, "[%d] socket creation failed: %m\n", id);
        return -1; 
    }

    int opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) < 0) {
        fprintf(stderr, "[%d] socket setsockopt SO_REUSEADDR failed: %m\n", id);
        goto fail;
    }
 
    /* IPv6 only, so an IPv4 listener can use the same port. */
    if (setsockopt(fd, IPPROTO_IPV6, IPV6_V6ONLY, &opt, sizeof(opt)) < 0) {
        fprintf(stderr, "[%d] socket setsockopt IPV6_V6ONLY failed: %m\n", id);
        goto fail;
    }


    if (bind(fd, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
        fprintf(stderr, "[%d] socket bind failed: %m\n", id);
        goto fail;
    }
 
    if (listen(fd, backlog) < 0) {
        fprintf(stderr, "[%d] socket listen failed: %m\n", id);
        goto fail;
    }

    return fd;

fail:
    close(fd);
    return -1;
}