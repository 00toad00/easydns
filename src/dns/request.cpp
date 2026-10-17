#include <dns/request.hpp>

#include <cstring>

void DnsRequest::setHeader(const dns_header_t *hdr) {
    if (!hdr) { return; }
    memcpy(&header, hdr, sizeof(dns_header_t));
}
void DnsRequest::getHeader(dns_header_t *dest_hdr) {
    if (!dest_hdr) { return; }
    memcpy(dest_hdr, &header, sizeof(header));
}

void DnsRequest::setId(uint16_t id) { header.id = id; }
uint16_t DnsRequest::getId() { return header.id; }

void DnsRequest::setFlags(uint16_t flags) { header.flags = flags; }
uint16_t DnsRequest::getFlags() { return header.flags; }

void DnsRequest::setQDCount(uint16_t qdcount) { header.qdcount = qdcount; }
uint16_t DnsRequest::getQDCount() { return header.qdcount; }

void DnsRequest::setANCount(uint16_t ancount) { header.ancount = ancount; }
uint16_t DnsRequest::getANCount() { return header.ancount; }

void DnsRequest::setNSCount(uint16_t nscount) { header.nscount = nscount; }
uint16_t DnsRequest::getNSCount() { return header.nscount; }

void DnsRequest::setARCount(uint16_t arcount) { header.arcount = arcount; }
uint16_t DnsRequest::getARCount() { return header.arcount; }