// Feb 27, 2022
#include <tuple>

template <typename... Ts>
constexpr auto tuple_sum(std::tuple<Ts...>&& t) noexcept {
  return [&]<unsigned long... Is>(std::index_sequence<Is...>) noexcept {
    return (std::get<Is>(t) + ...);
  }(std::make_index_sequence<sizeof...(Ts)>());
}
