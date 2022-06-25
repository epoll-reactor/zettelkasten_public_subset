#include <exception>
#include <functional>
#include <optional>
#include <thread>

#include <iostream>


void Sleep(int sleepTime) {
  std::this_thread::sleep_for(std::chrono::seconds(sleepTime));
}

template <typename Result, typename Exception = std::exception>
std::optional<Result> DoWithRetry(
    std::function<Result()> func, int retryCount,
    int sleepTime, bool throwLast) {
  for (int i = 0; i <= retryCount + 1; i++) {
    try {
     return func();
    } catch (Exception& error) {
      if (i < retryCount) {
        Sleep(sleepTime);
      } else {
        if (i == retryCount && !throwLast) {
          return std::nullopt;
        }
        if (i == retryCount && throwLast) {
          throw;
        }
      }
    }
  }
  return func();
}
