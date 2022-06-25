#include <zmqpp/zmqpp.hpp>
#include <iostream>
#include <chrono>
#include <thread>

int main() {
  const std::string endpoint = "tcp://*:5555";
  zmqpp::context context;
  zmqpp::socket_type socket_type = zmqpp::socket_type::reply;
  zmqpp::socket socket(context, socket_type);
  socket.bind(endpoint);
  while (true) {
    zmqpp::message message;
    socket.receive(message);
    std::string text;
    message >> text;
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(1s);
    std::cout << "Received: " << text << std::endl;
    socket.send("World");
  }
}
