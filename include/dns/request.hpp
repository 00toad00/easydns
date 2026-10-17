#pragma once

#include <dns/header.h>

class DnsRequest {
    private:
        dns_header_t    header;
    public:
        DnsRequest();

        // DNS Header
        void setHeader(const dns_header_t *hdr);
        dns_header_t *getHeader(dns_header_t *dest_hdr);
        
        void setId(uint16_t id);
        uint16_t getId();

        // Request Building
        void encode();
        void decode();
        void clear();
};