#include <network/srall.hpp>

#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int send_all(int fd, const std::string &data) {
    size_t sent = 0;
    while (sent < data.size()) {
        ssize_t n = send(fd, data.data() + sent, data.size() - sent, MSG_NOSIGNAL);
        if (n < 0) {
            if (errno == EINTR) continue;
            return 1;
        }
        sent += (size_t)n;
    }
    return 0;
}

int recv_all(int fd, std::string &data) {
    char buf[4096];
    for (;;) {
        ssize_t n = recv(fd, buf, sizeof(buf), 0);
        if (n > 0) { data.append(buf, (size_t)n); }
        else if (n == 0) { break; }
        else {
            if (errno == EINTR) { continue; }
            return 1;
        }
    }
    return 0;
}