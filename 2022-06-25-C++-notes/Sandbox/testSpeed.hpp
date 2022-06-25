#include <iostream>
#include <chrono>

#include "logger.hpp"


template <class Function, class... Args>
void testFuncSpeed(std::string_view label, uint64_t iterations, Function function, Args&&... args) {
  std::cout << label << "\t: " << iterations << " iteration(s): ";
  auto start = std::chrono::high_resolution_clock::now();
  while (iterations-- > 0) {
    function(std::forward<Args>(args)...);
  }
  auto timeSpent = std::chrono::high_resolution_clock::now() - start;
  std::cout << std::chrono::duration_cast<std::chrono::duration<float>>(timeSpent).count() << " s." << std::endl;
  std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(timeSpent).count() << " ms." << std::endl;
}

template <class Function, class... Args>
void testFuncSpeed(uint64_t iterations, Function function, Args&&... args) {
  std::string info = std::to_string(iterations) + " iteration(s): ";
  auto start = std::chrono::high_resolution_clock::now();
  while (iterations-- > 0) {
    function(std::forward<Args>(args)...);
  }
  auto timeSpent = std::chrono::high_resolution_clock::now() - start;
  info += std::to_string(std::chrono::duration_cast<std::chrono::duration<float>>(timeSpent).count());
  info += " s., ";
  info += std::to_string(std::chrono::duration_cast<std::chrono::milliseconds>(timeSpent).count());
  info += " ms.";
  std::cout << info << std::endl;
}

