#ifndef NETWORK_HTTP_CLIENT_ERRORS_INC
#define NETWORK_HTTP_CLIENT_ERRORS_INC

#include <system_error>
#include <stdexcept>
#include <network/config.hpp>

namespace network {
    namespace http {
        enum client_error {
            // request
            invalid_request,

            // response
            invalid_response,
        };
        typedef enum client_error client_error;

        const std::error_category &client_category();
        std::error_code make_error_code(client_error e);

        class invalid_url : public std::invalid_argument {
        public:

            explicit invalid_url();
            virtual ~invalid_url() noexcept;
        };

        class client_exception : public std::system_error {
        public:
            explicit client_exception(client_error err);
            virtual  client_exception() noexcept;
        };

    } // namespace http
} // namespace network

#endif // NETWORK_HTTP_CLIENT_ERRORS_INC
