#pragma once

#include <http/header.hpp>

#include <string>
#include <vector>

class HttpRequest {
    private:
        std::string                 method;
        std::string                 request_target;
        std::string                 http_version;
        std::vector<http_header_t>  headers;
        std::string                 body;

    public:
        std::string request;

        HttpRequest();

        void setMethod(std::string method);
        void setRequestTarget(std::string target);
        void setHttpVersion(std::string version);
        void addHeader(std::string name, std::string value);
        void setBody(std::string bod);

        void encode();
        void decode();
        void reset();
};