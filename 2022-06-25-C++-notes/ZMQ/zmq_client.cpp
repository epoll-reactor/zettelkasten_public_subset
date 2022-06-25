#include <zmqpp/zmqpp.hpp>
#include <iostream>

int main() {
  const std::string endpoint = "tcp://localhost:5555";
  zmqpp::context context;
  zmqpp::socket_type socket_type = zmqpp::socket_type::req;
  zmqpp::socket socket(context, socket_type);
  std::cout << "Connecting...\n";
  socket.connect(endpoint);
  for (int request = 0;  request != 10; ++request) {
    std::cout << "Sending " << request << " request...\n";
    std::string text;
    std::getline(std::cin, text);
    zmqpp::message message(text);
    socket.send(message);
    std::string buffer;
    socket.receive(buffer);
    std::cout << buffer << std::endl;
  }
}
