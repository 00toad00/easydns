#include <dns/response.hpp>

#include <cstring>

void DnsResponse::setHeader(const dns_header_t *hdr) {
    if (!hdr) { return; }
    memcpy(&header, hdr, sizeof(dns_header_t));
}
void DnsResponse::getHeader(dns_header_t *dest_hdr) {
    if (!dest_hdr) { return; }
    memcpy(dest_hdr, &header, sizeof(dns_header_t));
}

void DnsResponse::setId(uint16_t id) { header.id = id; }
uint16_t DnsResponse::getId() { return header.id; }

void DnsResponse::setFlags(uint16_t flags) { header.flags = flags; }
uint16_t DnsResponse::getFlags() { return header.flags; }

void DnsResponse::setQDCount(uint16_t qdcount) { header.qdcount = qdcount; }
uint16_t DnsResponse::getQDCount() { return header.qdcount; }

void DnsResponse::setANCount(uint16_t ancount) { header.ancount = ancount; }
uint16_t DnsResponse::getANCount() { return header.ancount; }

void DnsResponse::setNSCount(uint16_t nscount) { header.nscount = nscount; }
uint16_t DnsResponse::getNSCount() { return header.nscount; }

void DnsResponse::setARCount(uint16_t arcount) { header.arcount = arcount; }
uint16_t DnsResponse::getARCount() { return header.arcount; }