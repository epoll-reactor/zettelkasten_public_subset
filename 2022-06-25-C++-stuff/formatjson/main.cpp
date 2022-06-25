#include "json_formatter.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
  if (argc != 2) {
    throw std::invalid_argument
      ("Invalid count of arguments, please add ONE json file as first parameter.");
    }
    Json_formatter formatter(argv[1]);
//  Json_formatter formatter("example.json");
//  Json_formatter formatter("/home/machen/wiki.json");
//  Json_formatter formatter("/home/machen/vk.json");
  std::cout << formatter.format() << std::endl;
}
