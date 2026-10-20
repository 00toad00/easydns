#include <http/status.h>
#include <stdlib.h>

const char *http_status_str(int code) {
    switch (code) {
        case HTTP_CODE_CONTINUE:                        return "100 Continue";
        case HTTP_CODE_SWITCHING_PROTOCOLS:             return "101 Switching Protocols";
        case HTTP_CODE_PROCESSING:                      return "102 Processing";
        case HTTP_CODE_EARLY_HINTS:                     return "103 Early Hints";
    
        case HTTP_CODE_OK:                              return "200 OK";
        case HTTP_CODE_CREATED:                         return "201 Created";
        case HTTP_CODE_ACCEPTED:                        return "202 Accepted";
        case HTTP_CODE_NON_AUTHORITATIVE_INFORMATION:   return "203 Non-Authoritative Information";
        case HTTP_CODE_NO_CONTENT:                      return "204 No Content";
        case HTTP_CODE_RESET_CONTENT:                   return "205 Reset Content";
        case HTTP_CODE_PARTIAL_CONTENT:                 return "206 Partial Content";
        case HTTP_CODE_MULTI_STATUS:                    return "207 Multi-Status";
        case HTTP_CODE_ALREADY_REPORTED:                return "208 Already Reported";
        case HTTP_CODE_IM_USED:                         return "226 IM Used";
    
        case HTTP_CODE_MULTIPLE_CHOICES:                return "300 Multiple Choices";
        case HTTP_CODE_MOVED_PERMANENTLY:               return "301 Moved Permanently";
        case HTTP_CODE_FOUND:                           return "302 Found";
        case HTTP_CODE_SEE_OTHER:                       return "303 See Other";
        case HTTP_CODE_NOT_MODIFIED:                    return "304 Not Modified";
        case HTTP_CODE_USE_PROXY:                       return "305 Use Proxy";
        case HTTP_CODE_TEMPORARY_REDIRECT:              return "307 Temporary Redirect";
        case HTTP_CODE_PERMANENT_REDIRECT:              return "308 Permanent Redirect";
    
        case HTTP_CODE_BAD_REQUEST:                     return "400 Bad Request";
        case HTTP_CODE_UNAUTHORIZED:                    return "401 Unauthorized";
        case HTTP_CODE_PAYMENT_REQUIRED:                return "402 Payment Required";
        case HTTP_CODE_FORBIDDEN:                       return "403 Forbidden";
        case HTTP_CODE_NOT_FOUND:                       return "404 Not Found";
        case HTTP_CODE_METHOD_NOT_ALLOWED:              return "405 Method Not Allowed";
        case HTTP_CODE_NOT_ACCEPTABLE:                  return "406 Not Acceptable";
        case HTTP_CODE_PROXY_AUTHENTICATION_REQUIRED:   return "407 Proxy Authentication Required";
        case HTTP_CODE_REQUEST_TIMEOUT:                 return "408 Request Timeout";
        case HTTP_CODE_CONFLICT:                        return "409 Conflict";
        case HTTP_CODE_GONE:                            return "410 Gone";
        case HTTP_CODE_LENGTH_REQUIRED:                 return "411 Length Required";
        case HTTP_CODE_PRECONDITION_FAILED:             return "412 Precondition Failed";
        case HTTP_CODE_CONTENT_TOO_LARGE:               return "413 Content Too Large";
        case HTTP_CODE_URI_TOO_LONG:                    return "414 URI Too Long";
        case HTTP_CODE_UNSUPPORTED_MEDIA_TYPE:          return "415 Unsupported Media Type";
        case HTTP_CODE_RANGE_NOT_SATISFIABLE:           return "416 Range Not Satisfiable";
        case HTTP_CODE_EXPECTATION_FAILED:              return "417 Expectation Failed";
        case HTTP_CODE_IM_A_TEAPOT:                     return "418 I'm a teapot";
        case HTTP_CODE_MISDIRECTED_REQUEST:             return "421 Misdirected Request";
        case HTTP_CODE_UNPROCESSABLE_CONTENT:           return "422 Unprocessable Content";
        case HTTP_CODE_LOCKED:                          return "423 Locked";
        case HTTP_CODE_FAILED_DEPENDENCY:               return "424 Failed Dependency";
        case HTTP_CODE_TOO_EARLY:                       return "425 Too Early";
        case HTTP_CODE_UPGRADE_REQUIRED:                return "426 Upgrade Required";
        case HTTP_CODE_PRECONDITION_REQUIRED:           return "428 Precondition Required";
        case HTTP_CODE_TOO_MANY_REQUESTS:               return "429 Too Many Requests";
        case HTTP_CODE_REQUEST_HEADER_FIELDS_TOO_LARGE: return "431 Request Header Fields Too Large";
        case HTTP_CODE_UNAVAILABLE_FOR_LEGAL_REASONS:   return "451 Unavailable For Legal Reasons";
    
        case HTTP_CODE_INTERNAL_SERVER_ERROR:           return "500 Internal Server Error";
        case HTTP_CODE_NOT_IMPLEMENTED:                 return "501 Not Implemented";
        case HTTP_CODE_BAD_GATEWAY:                     return "502 Bad Gateway";
        case HTTP_CODE_SERVICE_UNAVAILABLE:             return "503 Service Unavailable";
        case HTTP_CODE_GATEWAY_TIMEOUT:                 return "504 Gateway Timeout";
        case HTTP_CODE_HTTP_VERSION_NOT_SUPPORTED:      return "505 HTTP Version Not Supported";
        case HTTP_CODE_VARIANT_ALSO_NEGOTIATES:         return "506 Variant Also Negotiates";
        case HTTP_CODE_INSUFFICIENT_STORAGE:            return "507 Insufficient Storage";
        case HTTP_CODE_LOOP_DETECTED:                   return "508 Loop Detected";
        case HTTP_CODE_NOT_EXTENDED:                    return "510 Not Extended";
        case HTTP_CODE_NETWORK_AUTHENTICATION_REQUIRED: return "511 Network Authentication Required";
    }
    return NULL;
}

const char *http_status_reason(int code) {
    const char *s = http_status_str(code);
    return s ? s + 4 : 0;
}