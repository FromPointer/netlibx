#ifndef NETWORK_HTTP_CLIENT_CLIENT_INC
#define NETWORK_HTTP_CLIENT_CLIENT_INC

#include <future>
#include <memory>
#include <cstdint>
#include <string>
#include <vector>
#include <chrono>
#include <algorithm>
#include <boost/asio/io_service.hpp>
#include <boost/optional.hpp>
#include <network/config.hpp>
#include <network/version.hpp>
#include <network/http/client/request.hpp>
#include <network/http/client/response.hpp>

namespace network {
    namespace http {
        namespace client_connection {
            class async_resolver;
            class async_connection;
        } // namespace client_connection

        class client_options {
        public:
            client_options () :
                _io_service(boost::none),
                _follow_redirects(false),
                _cache_resolved(false),
                _use_proxy(false),
                _always_verify_peer(false),
                _user_agent(std::string("cpp-netlibx/") + NETLIBX_VERSION),
                _timeout(30000) { }

            client_options(const client_options &other) :
                _io_service(other._io_service),
                _follow_redirects(other._follow_redirects),
                _cache_resolved(other._cache_resolved),
                _use_proxy(other._use_proxy),
                _always_verify_peer(other._always_verify_peer),
                _user_agent(other._user_agent),
                _timeout(other._timeout) { }

            client_options(client_options &&other) :
                _io_service(std::move(other._io_service)),
                _follow_redirects(std::move(other._follow_redirects)),
                _cache_resolved(std::move(other._cache_resolved)),
                _use_proxy(std::move(other._use_proxy)),
                _always_verify_peer(std::move(other._always_verify_peer)),
                _user_agent(std::move(other._user_agent)),
                _timeout(std::move(other._timeout)) { }
            
            client_options &operator = (client_options copts) {
                copts.swap(*this);
                return (*this);
            }

            ~client_options() {

            }

            void swap(client_options &other) noexcept {
                using std::swap;

                swap(_io_service, other._io_service);
                swap(_follow_redirects, other._follow_redirects);
                swap(_cache_resolved, other._cache_resolved);
                swap(_use_proxy, other._use_proxy);
                swap(_always_verify_peer, other._always_verify_peer);
                swap(_user_agent, other._user_agent);
                swap(_timeout, other._timeout);
            }

            client_options &io_service(boost::asio::io_service &iosrv) {
                _io_service = iosrv;
                return *this;
            }

            boost::optional<boost::asio::io_service> io_service() const {
                return _io_service;
            }

            const boost::optional<boost::asio::io_service> &io_service() const {
                return _io_service;
            }

            client_options &follow_redirects(bool bredir) {
                _follow_redirects = bredir;
                return (*this);
            }

            bool follow_redirects () const {
                return _follow_redirects;
            }

            const bool &follow_redirects() const {
                return _follow_redirects;
            }

            client_options &cache_resolved(bool resolved) {
                _cache_resolved = resolved;
                return (*this);
            }

            bool cache_resolved() const {
                return _cache_resolved;
            }

            const bool &cache_resolved() const {
                return _cache_resolved;
            }

            client_options &use_proxy(bool bproxy) {
                _use_proxy = bproxy;
                return (*this);
            }

            bool use_proxy()const {
                return _use_proxy;
            }

            const bool &use_proxy() const {
                return _use_proxy;
            }

            /* _timeout */
            client_options &timeout(std::chrono::milliseconds ms) {
                _timeout = ms;
                return (*this);
            }

            std::chrono::milliseconds timeout() const {
                return _timeout;
            }

            const std::chrono::milliseconds &timeout() const {
                return _timeout;
            }

            /* _openssl_certificate_paths */
            client_options *openssl_certificate_path(string path) {
                _openssl_certificate_paths.emplace_back(std::move(path));
                return (*this);
            }

            vector<string> openssl_certificate_paths() const {
                return _openssl_certificate_paths;
            }

            const vector<string> &openssl_certificate_paths() const {
                return _openssl_certificate_paths;
            }

            /* openssl_verify_path */
            client_options &openssl_verify_path(string path) {
                _openssl_verify_paths.emplace_back(std::move(path));
                return *this;
            }

            vector<string> openssl_verify_paths() const {
                return _openssl_verify_paths;
            }

            const vector<string> &openssl_verify_paths() const {
                return _openssl_verify_paths;
            }

            /* always_verify_peer */
            client_options &always_verify_peer(bool bverify_peer) {
                _always_verify_peer = bverify_peer;
                return (*this);
            }

            bool always_verify_peer() const {
                return _always_verify_peer;
            }

            /* user_agent */
            client_options &user_agent(const string &uagent) {
                _user_agent = uagent;
                return *this;
            }

            string user_agent() const {
                return _user_agent;
            }
            const string &user_agent() const {
                return _user_agent;
            }

        private:
            boost::optional<boost::asio::io_service&> _io_service;
            bool _follow_redirects;
            bool _cache_resolved;
            bool _use_proxy;
            bool _always_verify_peer;
            string _user_agent;
            std::chrono::milliseconds _timeout;
            vector<string> _openssl_certificate_paths;
            vector<string> _openssl_verify_paths;
        };

        inline void swap(client_options &lhs, client_options &rhs) {
            lhs.swap(rhs);
        }

        /* using director */
        typedef client_message::request_options request_options;
        typedef client_message::request         request;
        typedef client_message::response        response;

        class client {
            client(const client &) = delete;
            client& operator = (const client &) = delete;

        public:
            using std::string;

            explicit client(client_options options = client_options());

            client(std::unique_ptr<client_connection::async_resolver> mock_resolver,
                std::unique_ptr<client_connection::async_connection>  mock_connection,
                client_options options = client_options());

            ~client();

            std::future<response> execute(request req, request_options options = request_options());

            std::future<request> get(request req, request_options options = request_options());

            std::future<response> post(request req, request_options options = request_options());

            std::future<response> put(request req, request_options options = request_options());

            std::future<response> delete_(request req, request_options options = request_options());

            std::future<response> head(request req, request_options options = request_options());

            std::future<response> options(request req, request_options options = request_options());

        private:
            struct impl;
            impl *_pimpl;

        };
    } // namespace http
} // namespace network

#endif // NETWORK_HTTP_CLIENT_CLIENT_INC
