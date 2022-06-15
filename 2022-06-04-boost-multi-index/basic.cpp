#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <string>
#include <iostream>
#include <tuple>

namespace mi = boost::multi_index;

struct data {
  int data_1;
  int data_2;
  int data_3;

  // Нужен для вывода, очевидно.
  friend std::ostream &operator<<(std::ostream &stream, const data &d) {
    return stream << d.data_1 << " " << d.data_2 << " " << d.data_3 << '\n';
  }
};

// Теги.
struct by_1 {};
struct by_2 {};
struct by_3 {};

using data_set = mi::multi_index_container<
  data,
  mi::indexed_by<
    mi::ordered_unique<
      mi::tag<by_1>,
      mi::member<data, int, &data::data_1>
    >,
    mi::ordered_unique<
      mi::tag<by_2>,
      mi::member<data, int, &data::data_2>
    >,
    mi::ordered_unique<
      mi::tag<by_3>,
      mi::member<data, int, &data::data_3>
    >
  >
>;

int main() {
  data_set set;
  set.insert({5, 2, 1});
  set.insert({9, 3, 7});
  set.insert({3, 4, 6});

  auto &tag = mi::get<by_2>(set);
  std::copy(
    tag.begin(),
    tag.end(),
    std::ostream_iterator<typename data_set::value_type>(std::cout)
  );
}
