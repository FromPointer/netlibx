#ifndef NETWORK_HTTP_METHOD_INC
#define NETWORK_HTTP_METHOD_INC

#include <string>
#include <ostream>

namespace network {
    namespace http {
        enum method {
            none    = 0,
            get     = 1,
            post    = 2,
            put     = 3,
            delete_ = 4,
            head    = 5,
            options = 6,
            trace   = 7,
            connect = 8,
            merge   = 9,
            patch   = 10,
            num     = 11,
        };

        typedef enum method method;

        inline std::ostream &operator << (std::ostream *os, method m) {
            switch (m) {
            case method::get:
                return os << "GET";
            case method::post:
                return os << "POST";
            case method::put:
                return os << "PUT";
            case method::delete_:
                return os << "DELETE";
            case method::head:
                return os << "HEAD";
            case method::options:
                return os << "OPTIONS";
            case method::trace:
                return os << "TRACE";
            case method::connect:
                return os << "CONNECT";
            case method::merge:
                return os << "MERGE";
            case method::patch:
                return os << "PATCH";
            default:
                return os << "NOT_EXIST";
            }
            return os;
        }
    } // namespace http
}// namespace network



#endif // NETWORK_HTTP_METHOD_INC
