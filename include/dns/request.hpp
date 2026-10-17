#pragma once

#include <dns/header.h>

#include <string>

class DnsRequest {
    private:
        dns_header_t    header;
    public:
        DnsRequest();

        int encode();
        int decode();
        void clear();
};