#include <cstdio>
#include <iostream>
#include <cstring>

#include <network/udpsocket.hpp>

int main(int argc, char **argv) {
    UdpSocket s;
    s.createV4();
    s.bind("127.0.0.1", 9000);

    std::string msg;
    if (argc > 1) { msg.append(argv[1]); }
    else { msg = "Hallo"; }

    s.sendTo(msg, "127.0.0.2", 5454);

    return 0;
}