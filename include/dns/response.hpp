#pragma once

class DnsResponse {
    private:
    public:
        DnsResponse();

        int encode();
        int decode();
        void clear();
};