// Aug 12, 2021
#ifndef SSL_CLIENT_HPP
#define SSL_CLIENT_HPP

#include <iostream>
#include <string>
#include <vector>

#include "boost/asio.hpp"
#include "boost/asio/ssl.hpp"

using boost::asio::ip::tcp;
namespace ssl = boost::asio::ssl;

using ssl_socket = ssl::stream<tcp::socket>;

// Verify the certificate of the peer (remote host).
#define SSL_VERIFY 0

class ssl_client {
public:
    ssl_client(boost::asio::io_context& io_context,
        std::string_view host, std::string_view path);

private:
    void async_write();
    void async_read_some();
    void write_handler(boost::system::error_code ec, std::size_t length);
    void read_handler(boost::system::error_code ec, std::size_t length);
    void connect_handler(boost::system::error_code ec, tcp::endpoint endpoint);
    void handshake_handler(boost::system::error_code ec);

    boost::asio::io_context& io_context_;
    std::string host_;
    std::string path_;
    ssl::context ssl_context_;
    ssl::stream<tcp::socket> ssl_socket_;
    boost::asio::streambuf request_;
    std::vector<char> buffer_;
};

ssl_client::ssl_client(boost::asio::io_context& io_context, std::string_view host, std::string_view path)
    : io_context_(io_context), host_(host), path_(path), ssl_context_(ssl::context::sslv23), ssl_socket_(io_context_, ssl_context_), buffer_(1024)
{
    ssl_context_.set_default_verify_paths();
    boost::system::error_code ec;
    // Get a list of endpoints corresponding to the server name.
    tcp::resolver resolver(io_context_);
    auto endpoints = resolver.resolve(host_, "https", ec);
    if (ec) {
        std::cerr << "Resolve failed: " << ec.message() << std::endl;
        return;
    }
    boost::asio::async_connect(ssl_socket_.lowest_layer(), endpoints,
        std::bind(&ssl_client::connect_handler, this,
            std::placeholders::_1,
            std::placeholders::_2));
}

void ssl_client::connect_handler(boost::system::error_code ec, tcp::endpoint)
{
    if (ec) {
        std::cout << "Connect failed: " << ec.message() << std::endl;
    } else {
#if SSL_VERIFY
        ssl_socket_.set_verify_mode(ssl::verify_peer);
#else
        ssl_socket_.set_verify_mode(ssl::verify_none);
#endif  // SSL_VERIFY

        // ssl::host_name_verification has been added since Boost 1.73 to replace
        // ssl::rfc2818_verification.
#if BOOST_VERSION < 107300
        ssl_socket_.set_verify_callback(ssl::rfc2818_verification(host_));
#else
        ssl_socket_.set_verify_callback(ssl::host_name_verification(host_));
#endif  // BOOST_VERSION < 107300

        ssl_socket_.async_handshake(ssl::stream_base::client,
            std::bind(&ssl_client::handshake_handler,
                this, std::placeholders::_1));
    }
}

void ssl_client::handshake_handler(boost::system::error_code ec)
{
    if (ec) {
        std::cerr << "Handshake failed: " << ec.message() << std::endl;
    } else {
        async_write();
    }
}

void ssl_client::async_write()
{
    std::ostream request_stream(&request_);
    request_stream << "GET " << path_ << " HTTP/1.1\r\n";
    request_stream << "Host: " << host_ << "\r\n\r\n";
    boost::asio::async_write(ssl_socket_, request_,
        std::bind(&ssl_client::write_handler, this,
            std::placeholders::_1,
            std::placeholders::_2));
}

void ssl_client::write_handler(boost::system::error_code ec, std::size_t /*length*/)
{
    if (ec) {
        std::cerr << "Write failed: " << ec.message() << std::endl;
    } else {
        async_read_some();
    }
}

void ssl_client::async_read_some()
{
    ssl_socket_.async_read_some(boost::asio::buffer(buffer_),
        std::bind(&ssl_client::read_handler, this,
            std::placeholders::_1,
            std::placeholders::_2));
}


void ssl_client::read_handler(boost::system::error_code ec, std::size_t length)
{
    if (ec) {
        std::cout << ec.message() << std::endl;
    } else {
        std::cout.write(buffer_.data(), length);
    }
}

void Help(const char* argv0)
{
    std::cout << "Usage: " << argv0 << " <host> <path>" << std::endl;
    std::cout << "  E.g.," << std::endl;
    std::cout << "    " << argv0 << " www.boost.org /LICENSE_1_0.txt" << std::endl;
    std::cout << "    " << argv0 << " www.google.com /" << std::endl;
}

void ssl_client_test()
{
    std::string host = "api.vk.com";
    std::string path = "/method/groups.getById?access_token=7483c78eba756697ae4727e5d255d7782226d8bf4dc1a87924897e23278fddd46512c759c960a6468e4cf&v=5.130";

    try {
        boost::asio::io_context io_context;

        ssl_client client(io_context, host, path);

        io_context.run();

    } catch (const std::exception& e) {
        std::cout << "Exception: " << e.what() << std::endl;
    }
}


#endif // SSL_CLIENT_HPP
