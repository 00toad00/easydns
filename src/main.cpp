#include <cstdio>

#include <dns/request.hpp>
#include <dns/response.hpp>

int main(int argc, char **argv) {
    DnsRequest req;
    DnsResponse res;

    req.encode();

    return 0;
}