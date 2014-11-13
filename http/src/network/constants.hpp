#ifndef NETWORK_CONSTANT_HPP
#define NETWORK_CONSTANT_HPP


#include "version.hpp"

namespace network {
    struct constants {
        static char const* crlf();
        static char const* dot();
        static char        dot_char();
        static char const* http_slash();
        static char const* space();
        static char        space_char();
        static char const* slash();
        static char        slash_char();
        static char const* host();
        static char const* colon();
        static char        colon_char();
        static char const* accept();
        static char const* default_accept_mime();
        static char const* accept_encoding();
        static char const* default_accept_encoding();
        static char const* user_agent();
        static char const* default_user_agent();
        static char const* cpp_netlib_slash();
        static char        question_mark_char();
        static char        hash_char();
        static char const* connection();
        static char const* close();
        static char const* https();
        static char const* http();
    };
} // namespace network


namespace network {
    
    char const* constants::crlf() {
        static char crlf_[] = "\r\n";
        return crlf_;
    }

    char const* constants::dot() {
        static char dot_[] = ".";
        return dot_;
    }

    char constants::dot_char() {
        return '.';
    }

    char const* constants::http_slash() {
        static char http_slash_[] = "HTTP/";
        return http_slash_;
    }

    char const* constants::space() {
        static char space_[] = {' ', 0};
        return space_;
    }

    char constants::space_char() {
        return ' ';
    }

    char const* constants::slash() {
        static char slash_[] = {'/', 0};
        return slash_;
    }

    char constants::slash_char() {
        return '/';
    }

    char const* constants::host() {
        static char host_[] = {'H', 'O', 'S', 'T', 0};
        return host_;
    }

    char const* constants::colon() {
        static char colon_[] = {':', 0};
        return colon_;
    }

    char constants::colon_char() {
        return ':';
    }

    char const* constants::accept() {
        static char accept_[] = {'A', 'c', 'c', 'e', 'p', 't', 0};
        return accept_;
    }

    char const* constants::default_accept_mime() {
        static char mime_[] = {'*', '/', '*', 0};
        return mime_;
    }

    char const* constants::accept_encoding() {
        static char accept_encoding_[] = {
            'A',
            'c',
            'c',
            'e',
            'p',
            't',
            '-',
            'E',
            'n',
            'c',
            'o',
            'd',
            'i',
            'n',
            'g',
            0 
        };
        return accept_encoding_;
    }

    char const* constants::default_accept_encoding() {
        static char default_accept_encoding_[] = { 
            'i',
            'd',
            'e',
            'n',
            't',
            'i',
            't',
            'y',
            ';',
            'q',
            '=',
            '1',
            '.',
            '0',
            ',',
            ' ',
            '*',
            ';',
            'q',
            '=',
            '0',
            0
        };
        return default_accept_encoding_;
    }

    char const* constants::user_agent() {
        static char user_agent_[] = { 'U', 's', 'e', 'r', '-', 'A', 'g', 'e', 'n', 't', 0 };
        return user_agent_;
    }

    char const* constants::cpp_netlib_slash() {
        static char cpp_netlib_slash_[] = { 'c', 'p', 'p', '-', 'n', 'e', 't', 'l', 'i', 'b', '/', 0 };
        return cpp_netlib_slash_;
    }

    char constants::question_mark_char() {
        return '?'; 
    }

    char constants::hash_char() { 
        return '#';
    }

    char const* constants::connection() {
        static char connection_[] = "Connection";
        return connection_;
    }

    char const* constants::close() {
        static char close_[] = "close";
        return close_;
    }

    char const* constants::https() {
        static char https_[] = "https";
        return https_;
    }

    char const* constants::http() {
        static char http_[] = "http";
        return http_;
    }

    char const* constants::default_user_agent() {
        static char user_agent_[] = "cpp-netlib/" NETLIBX_VERSION;
        return user_agent_;
    }


    

}


#endif // NETWORK_CONSTANT_HPP
