#pragma once

#include <http/header.hpp>

#include <string>
#include <vector>

class HttpResponse {
    private:
        std::string                 http_version;
        std::string                 status; 
        int                         status_int;
        std::vector<http_header_t>  headers;
        std::string                 body;

    public:
        std::string response;

        HttpResponse();

        void setHttpVersion(std::string version);
        void setStatus(int status);
        void addHeader(std::string name, std::string value);
        void setBody(std::string bod);

        void encode();
        void decode();
        void reset();
};
