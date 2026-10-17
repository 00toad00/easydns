#include <dns/response.hpp>

#include <cstring>

void DnsResponse::setHeader(const dns_header_t *hdr) {
    if (!hdr) { return; }
    memcpy(&header, hdr, sizeof(dns_header_t));
}
dns_header_t *DnsResponse::getHeader(dns_header_t *dest_hdr) {
    if (!dest_hdr) { return; }
    memcpy(dest_hdr, &header, sizeof(dns_header_t));
}

void DnsResponse::setId(uint16_t id) { header.id = id; }
uint16_t DnsResponse::getId() { return header.id; }