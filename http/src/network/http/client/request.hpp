#ifndef NETWORK_HTTP_CLIENT_REQUEST_INC
#define NETWORK_HTTP_CLIENT_REQUEST_INC

#include <memory>
#include <string>
#include <vector>
#include <utility>
#include <cstdint>
#include <algorithm>
#include <functional>
#include <sstream>
#include <iterator>
#include <boost/range/iterator_range.hpp>
#include <boost/range/algorithm/equal.hpp>
#include <boost/range/as_literal.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/optional.hpp>
#include <network/config.hpp>
#include <network/http/method.hpp>
#include <network/http/client/client_errors.hpp>
#include <network/uri.hpp>

namespace network {
    namespace http {
        namespace client_message {

            enum transfer_direction {
                bytes_written,
                bytes_read,
            };
            typedef enum transfer_direction transfer_direction;

            /*
             * class request_options
             */
            class request_options {
                request_options() :
                    _resolve_timeout(30000),
                    _read_timeout(30000),
                    _total_timeout(30000),
                    _max_redirects(10) {
                
                    }
                request_options(request_options const &other) :
                    _resolve_timeout(other._resolve_timeout),
                    _read_timeout(other._read_timeout),
                    _total_timeout(other._total_timeout),
                    _max_redirects(other._max_redirects) {

                    }
                /*
                 * assignment operator
                 */
                request_options &operator = (request_options other) {
                    other.swap(&this);
                    return (*this);
                }

                ~request_options() {

                }

                void swap(request_options &other) {
                    using std::swap;
                    swap(_resolve_timeout, other._resolve_timeout);
                    swap(_read_timeout, other._read_timeout);
                    swap(_total_timeout, other._total_timeout);
                    swap(_max_redirects, other._max_redirects);
                }

                request_options &resolver_timeout(std::uint64_t rl_to) {
                    _resolve_timeout = rl_to;
                    return (*this);
                }
                
                std::uint64_t resolver_timeout() const {
                    return (_resolve_timeout);
                }

                request_options &read_timeout(std::uint64_t read_to) {
                    _read_timeout = read_to;
                    return *this;
                }

                std::uint64_t read_timeout() const {
                    return _read_timeout;
                }

                request_options &total_timeout(std::uint64_t total_to) {
                    _total_timeout = total_to;
                    return *this;
                }

                std::uint64_t total_timeout() const {
                    return _total_timeout;
                }

                request_options &max_redirects(int max_redirects) {
                    _max_redirects = max_redirects;
                    return *this;
                }

                int max_redirects() const {
                    return _max_redirects;
                }

                request_options &progress(std::function<void (transfer_direction, std::uint64_t)> handler) {
                    _progress_handler = handler;
                    return *this;
                }

                std::function<void (transfer_direction, std::uint64_t)> progress() const {
                    return _progress_handler;
                }

            private:
                std::uint64_t _resolve_timeout;
                std::uint64_t _read_timeout;
                std::uint64_t _total_timeout;
                int           _max_redirects;
                std::function<void (transfer_direction, std::uint64_t)> _progress_handler;

            };



            inline void swap(request_options &lhs, request_options *rhs) {
                lhs.swap(rhs);
            }

            /*
             * class: byte_source
             */
            class byte_source {
            public:
                using std::string;
                using std::size_t;

                virtual ~byte_source() { }

                virtual size_t read(string &src, size_t len);
            };

            class string_byte_source : public byte_source {
            public:
                explicit string_byte_source(string source);
                virtual ~string_byte_source() { }

                virtual size_t read(string &source, size_t len);

            private:
                string _source;
            };

            class request {
            public:
                using std::string;
                using std::size_t;
                
                typedef std::vector<std::pair<string, string> > header_t;
                typedef header_t::iterator header_iterator;
                typedef header_t::const_iterator const_header_iterator;

                using network::http::method;
                using network::uri;

            public:
                request () : _byte_source(nullptr) { }
                explicit request(uri url) : _url(url) {
                    /*
                     * The syntax is:
                     * scheme://domain:port/path?query_string#fragment_id
                     */
                    if (auto scheme = url.scheme()) { // scheme/protocol name
                        if (!boost::equal(*scheme, boost::as_literal("http")) &&
                            !boost::equal(*scheme, boost::as_literal("https"))) {
                            throw invalid_url();
                        }

                        if (auto path = url.path()) { // path
                            std::copy(std::begin(*begin), std::end(*path), std::back_inserter(_path));
                        }

                        if (auto query = url.query()) { // query string
                            _path.push_back('?');
                            std::copy(std::begin(*query), std::end(*query), std::back_inserter(_path));
                        }

                        if (auto fragment = url.fragment()) { // fragment id
                            _path.push_back('#');
                            std::copy(std::begin(*fragment), std::end(*fragment), std::back_inserter(_path));
                        }

                        /* domain:port */
                        std::ostringstream oss;
                        std::copy(std::begin(*url.host()), std::end(*url.host()),
                            std::ostream_iterator<char>(oss));
                        if (url.port()) {
                            oss << ":";
                            std::copy(std::begin(*url.port()), std::end(*url.port()),
                                std::ostream_iterator<char>(oss));
                        }
                        append_header("Host", oss.str());
                    } else {
                        throw invalid_url();
                    }
                } // request

                /*
                 * copy constructor
                 */
                request(const request &other) :
                    _url(other._url),
                    _method(other._method),
                    _path(other._path),
                    _version(other._version),
                    _headers(other._headers),
                    _byte_source(other._byte_source) { }

                /*
                 * move constructor
                 */
                request(request &&other) noexcept :
                    _url(std::move(other._url)),
                    _method(std::move(other._method)),
                    _path(std::move(other._path)),
                    _version(std::move(other._version)),
                    _headers(std::move(other._headers)),
                    _byte_source(std::move(other._byte_source)) { }

    
                /*
                 * Assignment constructor
                 */
                request &operator = (request other) {
                    other.swap(*this);
                    return (*this);
                }

                ~request() {

                }

                void swap(request &other) noexcept {
                    using std::swap;
                    swap(_url, other._url);
                    swap(_method, other._method);
                    swap(_path, other._path);
                    swap(_version, other._version);
                    swap(_headers, other._headers);
                    swap(_byte_source, other._byte_source);
                }

                request &url(const uri &url) {
                    _url = url;
                    return *this;
                }
                uri url() const {
                    return _url;
                }

                bool is_https() const {
                    return (
                        _url.scheme() &&
                        boost::equal(*_url.scheme(), boost::as_literal("https"))
                        );
                }

                request &method(method md) {
                    _method = md;
                    return *this;
                }

                method method() const {
                    return _method;
                }

                request &path(string path) {
                    _path = path;
                    return *this;
                }

                string path() const {
                    return _path;
                }

                request &version(string ver) {
                    _version = ver;
                    return (*this);
                }

                string version() const {
                    return _version;
                }

                request &body(std::shared_ptr<byte_source> bsrc) {
                    _byte_source = bsrc;
                    return (*this);
                }

                template <class Handler>
                    void body(size_t len, Handler &&handler) {
                        string body;
                        _byte_source->read(body, len);
                        handler(body);
                    }

                request &append_header(string name, string value) {
                    _headers.emplace_back(std::make_pair(name, value));
                    return (*this);
                }

                boost::optional<string> header(const string &name) {
                    for (auto hdr : _headers) {
                        if (boost::iequals(hdr.first, name))
                            return hdr.second;
                    }
                    return boost::optional<string>();
                }

                const_header_iterator headers_begin() const {
                    return std::begin(_headers);
                }

                const_header_iterator headers_end() const {
                    return std::end(_headers);
                }

                boost::iterator_range<const_header_iterator> 
                    headers() const {
                        return boost::make_iterator_range(headers_begin(), headers_end());
                    }

                void remove_header(const string &name) {
                    auto it = std::remove_if(std::begin(_headers), std::end(_headers), 
                        [&name] (const std::pair<string, string> &hdr) {
                        return boost::iequals(hdr.first, name);
                        });
                    _headers.erase(it, std::end(_headers));
                }

                void clear_headers() {
                    header_t().swap(_headers);
                }

            private:
                network::uri   _url;
                method         _method;
                string         _path;
                string         _version;
                header_t       _headers;
                std::shared_ptr<byte_source> _byte_source;

                /*
                 * friend function
                 */
                friend std::ostream &operator << (std::ostream &os, const request &req) {
                    os << req._method << " " << req._path << " HTTP/" << req._version << "\r\n";

                    for (auto hdr = req._headers.begin(); hdr != req._headers.end(); hrd++) {
                        os << hdr.first << ": " << hdr.second << "\r\n";
                    }
                    os << "\r\n";

                    return os;
                }


            };

            inline void swap(request &lhs, request &rhs) noexcept {
                lhs.swap(rhs);
            }
        } // namespace client_message
    } // namespace http
} // namespace network


#endif // NETWORK_HTTP_CLIENT_REQUEST_INC
