// Jan 30, 2021
#ifndef CALL_SEQUENCE_HPP
#define CALL_SEQUENCE_HPP

#include <iostream>
#include <optional>
#include <any>

/*! @brief Function caller.
 */
class CallSequence {
public:
  /*!
   * @brief Executes fun().fun()... call sequence until passed functor returns
   *        non-empty optional object. If std::nullopt returned, nothing is
   *        executed.
   * @param f       - functor
   * @param args    - functor arguments
   * @return          reference to itself
   */
  template <typename Function, typename... Args>
  CallSequence& fun(Function&& f, Args&&... args) noexcept(noexcept(f)) {
    if (!opt_) return *this;
    if (auto optional = f(std::forward<Args>(args)...))
      opt_ = std::move(optional);
    else
      opt_.reset();
    return *this;
  }

private:
  std::optional<std::any> opt_ = 0;
};

#endif //CALL_SEQUENCE_HPP
