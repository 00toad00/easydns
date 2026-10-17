#include <dns/request.hpp>

#include <cstring>

void DnsRequest::setHeader(const dns_header_t *hdr) {
    if (!hdr) { return; }
    memcpy(&header, hdr, sizeof(dns_header_t));
}
dns_header_t *DnsRequest::getHeader(dns_header_t *dest_hdr) {
    if (!dest_hdr) { return; }
    memcpy(dest_hdr, &header, sizeof(header));
}

void DnsRequest::setId(uint16_t id) { header.id = id; }
uint16_t DnsRequest::getId() { return header.id; }