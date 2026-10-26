#include <cstdio>
#include <iostream>
#include <cstring>
#include <vector>

#include <network/udpsocket.hpp>

#include <sys/epoll.h>

struct conn_t {
    UdpSocket socket;
    epoll_event event;
};

int main(int argc, char **argv) {
    std::vector<conn_t> conns;
}