#ifndef NETWORK_HTTP_CLIENT_RESPONSE_INC
#define NETWORK_HTTP_CLIENT_RESPONSE_INC

#include <cstdint>
#include <vector>
#include <utility>
#include <string>
#include <future>
#include <network/http/status.hpp>
#include <network/config.hpp>
#include <network/uri.hpp>
#include <boost/range/iterator_range.hpp>


namespace network {
    namespace http {
        namespace client_message {
            class response {
                public:
                    using std::string;
                    using std::vector;
                    using network::http::status;

                public:
                    typedef vector<pair<string, string> > header_t;
                    typedef header_t::iterator header_iterator;
                    typedef header_t::const_iterator const_header_iterator;

                public:
                    response() { }

                    response(const response &other) :
                        _version(other._version),
                        _status(other._status),
                        _status_msg(other._status_msg),
                        _headers(other._headers),
                        _body(other._body) {

                        }

                    response(response &&other) noexcept :
                        _version(std::move(other._version)),
                        _status(std::move(other._status)),
                        _status_msg(std::move(other._status_msg)),
                        _headers(std::move(other._headers)),
                        _body(std::move(other._body)) {

                        }

                    response &operator = (response other) {
                        // other is tmp object, will release outof range
                        other.swap(*this);
                        return *this;
                    }

                    void swap(response &other) noexcept {
                        using std::swap;
                        swap(_version, other._version);
                        swap(_status, other._status);
                        swap(_status_msg, other._status_msg);
                        swap(_headers, other._headers);
                        swap(_body, other._body);
                    }

                    void version(const string &ver) {
                        _version = ver;
                    }

                    string version() const {
                        return _version;
                    }

                    const string &version() const {
                        return _version;
                    }

                    void status(status::code status) {
                        _status = status;
                    }

                    status::code status() const {
                        return _status;
                    }

                    void status_message(const string &status_msg) {
                        _status_msg = status_msg;
                    }

                    string status_message() const {
                        return _status_msg;
                    }

                    const string &status_message() const {
                        return _status_msg;
                    }

                    void add_header(const string &name, const string &value) {
                        _headers.push_back(std::make_pair(name, value));
                    }

                    const_header_iterator headers_begin() const {
                        return std::begin(_headers);
                    }
                    
                    const_header_iterator headers_end() const {
                        return std::end(_headers);
                    }

                    boost::iterator_range<const_header_iterator>
                        headers() const {
                            return boost::make_iterator_range(headers_begin(), headers());
                        }

                    void append_body(const string &body) {
                        _body.append(body);
                    }

                private:
                    string       _version;
                    status::code _status;
                    string       _status_msg;
                    header_t     _headers;
                    string       _body;
            };
            
            inline void swap(response &lhs, response &rhs) noexcept {
                lhs.swap(rhs);
            }

        } // namespace client_message
    } // namespace http
} // namespace network

#endif // NETWORK_HTTP_CLIENT_RESPONSE_INC
