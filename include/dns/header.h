#pragma once

#include <stdint.h>

typedef struct __attribute__((packed)) {
    volatile uint16_t id;
    volatile uint16_t flags;
    volatile uint16_t qdcount;
    volatile uint16_t ancount;
    volatile uint16_t nscount;
    volatile uint16_t arcount;
} dns_header_t;