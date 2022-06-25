#include <iostream>
#include <thread>
#include <list>
#include <array>

#include <boost/asio.hpp>

void asio_hello_world() {
  std::thread server([]{
    boost::asio::io_service service;
    boost::asio::post(service, [&]{
      boost::asio::const_buffer data = boost::asio::buffer("Hello, World");
      boost::asio::ip::udp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8080);
      boost::asio::ip::udp::socket socket(service);
      socket.open(boost::asio::ip::udp::v4());
      boost::system::error_code ec;
      socket.send_to(data, endpoint, 0, ec);
    });
    service.run();
  });
  std::thread client([]{
    boost::asio::io_service service;
    boost::asio::ip::udp::socket socket(service);
    boost::asio::ip::udp::endpoint endpoint;
    socket.open(boost::asio::ip::udp::v4());
    socket.bind(boost::asio::ip::udp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8080));
    std::array<char, 1024> recv_buffer{0};
    socket.async_receive_from(boost::asio::buffer(recv_buffer), endpoint, [&recv_buffer](
      const boost::system::error_code& error, size_t bytes_transferred){
      if (error) {
        std::cout << "Receive failed: " << error.message() << "\n";
        return;
      }
      std::cout << std::string(recv_buffer.begin(), recv_buffer.begin() + bytes_transferred);
    });
    service.run();
  });
  client.join();
  server.join();
}
