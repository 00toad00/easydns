#pragma once

#include <dns/header.h>

class DnsRequest {
    private:
        dns_header_t    header;
    public:
        // DNS Header
        void setHeader(const dns_header_t *hdr);
        void getHeader(dns_header_t *dest_hdr);
        
        void setId(uint16_t id);
        uint16_t getId();

        void setFlags(uint16_t flags);
        uint16_t getFlags();

        void setQDCount(uint16_t qdcount);
        uint16_t getQDCount();

        void setANCount(uint16_t ancount);
        uint16_t getANCount();

        void setNSCount(uint16_t nscount);
        uint16_t getNSCount();

        void setARCount(uint16_t arcount);
        uint16_t getARCount();

        // Request Building
        void encode();
        void decode();
        void clear();
};