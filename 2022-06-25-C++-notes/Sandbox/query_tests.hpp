// Nov 28, 2021
#include "query.hpp"

namespace test {
namespace container_traits {

void test_push() {
  using namespace query::container_traits;
  std::vector<std::string> v;                             any_push(v, "1");
  std::list<std::string> l;                               any_push(l, "1");
  std::deque<std::string> d;                              any_push(d, "1");
  std::queue<std::string> q;                              any_push(q, "1");
  std::set<std::string> s;                                any_push(s, "1");
  std::stack<std::string> st;                             any_push(st, "1");
  std::multiset<std::string> ms;                          any_push(ms, "1");
  std::map<std::string, std::string> m;                   any_push(m, "1", "2");
  std::multimap<std::string, std::string> mm;             any_push(mm, "1", "2");
  std::unordered_map<std::string, std::string> um;        any_push(um, "1", "2");
  std::unordered_multimap<std::string, std::string> umm;  any_push(um, "1", "2");
}

void test_clear() {
  using namespace query::container_traits;
  std::vector<std::string> v;                             any_clear(v);
  std::list<std::string> l;                               any_clear(l);
  std::deque<std::string> d;                              any_clear(d);
  std::queue<std::string> q;                              any_clear(q);
  std::set<std::string> s;                                any_clear(s);
  std::stack<std::string> st;                             any_clear(st);
  std::multiset<std::string> ms;                          any_clear(ms);
  std::map<std::string, std::string> m;                   any_clear(m);
  std::multimap<std::string, std::string> mm;             any_clear(mm);
  std::unordered_map<std::string, std::string> um;        any_clear(um);
  std::unordered_multimap<std::string, std::string> umm;  any_clear(um);
}

} // namespace container_traits

namespace convert {

void test_traits() {
  using namespace query::container_traits;
  static_assert( has_mapped_type<std::unordered_map<int, int>>::value);
  static_assert( has_mapped_type<std::map<int, int>>::value);
  static_assert( has_mapped_type<std::multimap<int, int>>::value);
  static_assert( has_mapped_type<std::unordered_multimap<int, int>>::value);
  static_assert(!has_mapped_type<std::set<int>>::value);
  static_assert(!has_mapped_type<std::vector<int>>::value);

  static_assert( has_iterator<std::unordered_map<int, int>>::value);
  static_assert( has_iterator<std::map<int, int>>::value);
  static_assert( has_iterator<std::multimap<int, int>>::value);
  static_assert( has_iterator<std::unordered_multimap<int, int>>::value);
  static_assert( has_iterator<std::set<int>>::value);
  static_assert( has_iterator<std::vector<int>>::value);
  static_assert(!has_iterator<int>::value);
  static_assert(!has_iterator<char>::value);
  static_assert(!has_iterator<std::pair<int, int>>::value);

  struct empty {};
  static_assert(!has_three_way_comparator<int>::value);
  static_assert(!has_three_way_comparator<empty>::value);
  static_assert( has_three_way_comparator<std::string>::value);
  static_assert( has_three_way_comparator<std::vector<int>>::value);
  static_assert(!has_three_way_comparator<std::ostreambuf_iterator<int>>::value);
}

template <typename Container>
void test_seq_to_seq_impl() {
  const std::vector<int> v = { 1, 2, 3 };
  const Container assert = { 1, 2, 3 };
  query::cast<decltype(v), Container> cast;
  auto cv = cast(v);
  assert(cv == assert);
}

void test_seq_to_seq() {
  test_seq_to_seq_impl<std::vector<int>>();
  test_seq_to_seq_impl<std::deque<int>>();
  test_seq_to_seq_impl<std::forward_list<int>>();
  test_seq_to_seq_impl<std::list<int>>();
  test_seq_to_seq_impl<std::set<int>>();
  test_seq_to_seq_impl<std::multiset<int>>();
}

void test_seq_to_string() {
  const std::deque<int> v = { 1, 2, 3 };
  const std::string assert = "1 2 3";
  query::cast<decltype(v), std::string> cast;
  const auto cv = cast(v);
  assert(cv == assert);
}

void test_ass_to_string() {
  const std::map<int, int> v = { {1, 1}, {2, 2}, {3, 3} };
  const std::string assert = "(1, 1)(2, 2)(3, 3)";
  query::cast<decltype(v), std::string> cast;
  const auto cv = cast(v);
  assert(cv == assert);
}

template <typename Container>
void test_ass_to_ass_impl() {
  const std::map<int, int> v = { {1, 1}, {2, 2}, {3, 3} };
  const Container assert = { {1, 1}, {2, 2}, {3, 3} };
  query::cast<decltype(v), Container> cast;
  const auto cv = cast(v);
  assert(cv == assert);
}

void test_ass_to_ass() {
  test_ass_to_ass_impl<std::map<int, int>>();
  test_ass_to_ass_impl<std::unordered_map<int, int>>();
  test_ass_to_ass_impl<std::multimap<int, int>>();
  test_ass_to_ass_impl<std::unordered_multimap<int, int>>();
}

template <typename Associative ,typename Sequence>
void test_ordered_ass_to_seq_impl() {
  const Associative v = { {1, 1}, {2, 2}, {3, 3} };
  const Sequence assert = { 1, 1, 2, 2, 3, 3 };
  query::cast<Associative, Sequence> cast;
  const auto cv = cast(v);
  assert(cv == assert);
}

template <typename Associative ,typename Sequence>
void test_ordered_multi_ass_to_seq_impl() {
  const Associative v = { {1, 1}, {1, 1}, {2, 2}, {2, 2}, {3, 3}, {3, 3} };
  const Sequence assert = { 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 };
  query::cast<Associative, Sequence> cast;
  const Sequence cv = cast(v);
  assert(cv == assert);
}

template <typename Associative, typename Sequence>
void test_unordered_ass_to_seq_impl() {
  const Associative v = { {1, 1}, {1, 1}, {2, 2}, {2, 2}, {3, 3}, {3, 3} };
  const Sequence assert = { 1, 1, 2, 2, 3, 3 };
  query::cast<Associative, Sequence> cast;
  const auto cv = cast(v);
  assert(cv == assert);
}

template <typename Associative, typename Sequence>
void test_unordered_multi_ass_to_seq_impl() {
  const Associative v = { {1, 1}, {1, 1}, {2, 2}, {2, 2}, {3, 3}, {3, 3} };
  const Sequence assert = { 1, 1, 1, 1, 2, 2, 2, 2, 3, 3, 3, 3 };
  query::cast<Associative, Sequence> cast;
  const auto cv = cast(v);
  assert(cv == assert);
}

void test_ass_to_seq() {
  test_ordered_ass_to_seq_impl<std::map<int, int>, std::vector<int>>();
  test_ordered_ass_to_seq_impl<std::map<int, int>, std::deque<int>>();
  test_ordered_ass_to_seq_impl<std::map<int, int>, std::list<int>>();
  test_ordered_ass_to_seq_impl<std::map<int, int>, std::set<int>>();
  test_ordered_ass_to_seq_impl<std::map<int, int>, std::multiset<int>>();
  test_ordered_multi_ass_to_seq_impl<std::multimap<int, int>, std::vector<int>>();
  test_ordered_multi_ass_to_seq_impl<std::multimap<int, int>, std::deque<int>>();
  test_ordered_multi_ass_to_seq_impl<std::multimap<int, int>, std::list<int>>();
  test_ordered_multi_ass_to_seq_impl<std::multimap<int, int>, std::set<int>>();
  test_ordered_multi_ass_to_seq_impl<std::multimap<int, int>, std::multiset<int>>();

  /// Easy test for sorted sets only
  test_unordered_ass_to_seq_impl<std::unordered_map<int, int>, std::set<int>>();
  test_unordered_ass_to_seq_impl<std::unordered_multimap<int, int>, std::set<int>>();
  test_unordered_ass_to_seq_impl<std::unordered_map<int, int>, std::multiset<int>>();
  test_unordered_multi_ass_to_seq_impl<std::unordered_multimap<int, int>, std::multiset<int>>();
}

void test_convert() {
  test_traits();
  test_seq_to_seq();
  test_seq_to_string();
  test_ass_to_string();
  test_ass_to_ass();
  test_ass_to_seq();
}

}// namespace convert

namespace conditional {

void test_cond() {
  constexpr query::gate _1(std::equal_to<>{}, 1, 0);
  static_assert(!_1);
  struct equal_comparator {
    constexpr bool operator()(int l, int r) const {
      return l == r;
    }
  };
  constexpr query::gate _2(equal_comparator{}, 1, 1);
  static_assert(_2);
  constexpr query::gate
    _3(std::equal_to<>{}, 1, 1),
    _4(std::equal_to<>{}, 2, ~0),
    _5(std::equal_to<>{}, 3, 3);
  static_assert(!(_3 && _4) || _5);
  static_assert(query::gate(std::equal_to<>{}, 1, 1));
  static_assert(
    (
         !query::gate(std::equal_to<>{}, 1, 1)
      || !query::gate(std::equal_to<>{}, 1, 0)
    ) && (
         !query::gate(std::less_equal<>{}, 1, 1)
      || !query::gate(std::less<>{}, 1, 1)
    )
  );
  constexpr query::gate _6(std::equal_to<>{}, 2);
  static_assert( _6.compare_with(2));
  static_assert(!_6.compare_with(1));
}

} // namespace conditional

namespace from {

template <typename From, typename To>
void from_tests_seq_cast() {
  const From container = { 1, 2, 3 };
  const   To    assert = { 1, 2, 3 };
  const   To converted = query::from<From>(container).to(To{});
  assert(converted == assert);
}

template <typename From, typename To>
void from_tests_ass_cast() {
  const From container = { {1,1}, {2,2}, {3,3} };
  const   To    assert = { {1,1}, {2,2}, {3,3} };
  const   To converted = query::from(container).to(To{});
  assert(converted == assert);
}

void from_tests() {
  from_tests_seq_cast<std::vector<int>, std::vector<int>>();
  from_tests_seq_cast<std::vector<int>, std::deque<int>>();
  from_tests_seq_cast<std::vector<int>, std::list<int>>();
  from_tests_seq_cast<std::vector<int>, std::forward_list<int>>();
  from_tests_seq_cast<std::vector<int>, std::set<int>>();
  from_tests_seq_cast<std::vector<int>, std::multiset<int>>();

  from_tests_seq_cast<std::deque<int>, std::vector<int>>();
  from_tests_seq_cast<std::deque<int>, std::deque<int>>();
  from_tests_seq_cast<std::deque<int>, std::list<int>>();
  from_tests_seq_cast<std::deque<int>, std::forward_list<int>>();
  from_tests_seq_cast<std::deque<int>, std::set<int>>();
  from_tests_seq_cast<std::deque<int>, std::multiset<int>>();

  from_tests_seq_cast<std::list<int>, std::vector<int>>();
  from_tests_seq_cast<std::list<int>, std::deque<int>>();
  from_tests_seq_cast<std::list<int>, std::list<int>>();
  from_tests_seq_cast<std::list<int>, std::forward_list<int>>();
  from_tests_seq_cast<std::list<int>, std::set<int>>();
  from_tests_seq_cast<std::list<int>, std::multiset<int>>();

  from_tests_seq_cast<std::forward_list<int>, std::vector<int>>();
  from_tests_seq_cast<std::forward_list<int>, std::deque<int>>();
  from_tests_seq_cast<std::forward_list<int>, std::list<int>>();
  from_tests_seq_cast<std::forward_list<int>, std::forward_list<int>>();
  from_tests_seq_cast<std::forward_list<int>, std::set<int>>();
  from_tests_seq_cast<std::forward_list<int>, std::multiset<int>>();

  from_tests_seq_cast<std::set<int>, std::vector<int>>();
  from_tests_seq_cast<std::set<int>, std::deque<int>>();
  from_tests_seq_cast<std::set<int>, std::list<int>>();
  from_tests_seq_cast<std::set<int>, std::forward_list<int>>();
  from_tests_seq_cast<std::set<int>, std::set<int>>();
  from_tests_seq_cast<std::set<int>, std::multiset<int>>();

  from_tests_seq_cast<std::multiset<int>, std::vector<int>>();
  from_tests_seq_cast<std::multiset<int>, std::deque<int>>();
  from_tests_seq_cast<std::multiset<int>, std::list<int>>();
  from_tests_seq_cast<std::multiset<int>, std::forward_list<int>>();
  from_tests_seq_cast<std::multiset<int>, std::set<int>>();
  from_tests_seq_cast<std::multiset<int>, std::multiset<int>>();

  from_tests_ass_cast<std::map<int, int>, std::map<int, int>>();
  from_tests_ass_cast<std::map<int, int>, std::multimap<int, int>>();
  from_tests_ass_cast<std::map<int, int>, std::unordered_map<int, int>>();
  from_tests_ass_cast<std::map<int, int>, std::unordered_multimap<int, int>>();

  from_tests_ass_cast<std::multimap<int, int>, std::map<int, int>>();
  from_tests_ass_cast<std::multimap<int, int>, std::multimap<int, int>>();
  from_tests_ass_cast<std::multimap<int, int>, std::unordered_map<int, int>>();
  from_tests_ass_cast<std::multimap<int, int>, std::unordered_multimap<int, int>>();

  from_tests_ass_cast<std::unordered_map<int, int>, std::map<int, int>>();
  from_tests_ass_cast<std::unordered_map<int, int>, std::multimap<int, int>>();
  from_tests_ass_cast<std::unordered_map<int, int>, std::unordered_map<int, int>>();
  from_tests_ass_cast<std::unordered_map<int, int>, std::unordered_multimap<int, int>>();

  from_tests_ass_cast<std::unordered_multimap<int, int>, std::map<int, int>>();
  from_tests_ass_cast<std::unordered_multimap<int, int>, std::multimap<int, int>>();
  from_tests_ass_cast<std::unordered_multimap<int, int>, std::unordered_map<int, int>>();
  from_tests_ass_cast<std::unordered_multimap<int, int>, std::unordered_multimap<int, int>>();
}

} // namespace from

namespace merge {

template <typename Container, typename MergeWith>
void from_tests_merge_seq() {
  const Container to_merge_1 = { 1, 2, 3 };
  const MergeWith to_merge_2 = { 4, 5, 6 };
  const Container     assert = { 1, 2, 3, 4, 5, 6 };
  const Container     merged = query::from(to_merge_1).merge(to_merge_2).to(Container{});
  assert(merged == assert);
}

template <typename Container>
void from_tests_merge_by_il() {
  const Container to_merge_1 = { 1, 2, 3 };
  const Container     assert = { 1, 2, 3, 4, 5, 6 };
  const Container     merged = query::from(to_merge_1).merge({ 4, 5, 6 }).to(Container{});
  assert(merged == assert);
}

void merge_tests() {
  from_tests_merge_seq<std::vector<int>, std::vector<int>>();
  from_tests_merge_seq<std::vector<int>, std::deque<int>>();
  from_tests_merge_seq<std::vector<int>, std::list<int>>();
  from_tests_merge_seq<std::vector<int>, std::forward_list<int>>();
  from_tests_merge_seq<std::vector<int>, std::set<int>>();
  from_tests_merge_seq<std::vector<int>, std::multiset<int>>();

  from_tests_merge_seq<std::deque<int>, std::vector<int>>();
  from_tests_merge_seq<std::deque<int>, std::deque<int>>();
  from_tests_merge_seq<std::deque<int>, std::list<int>>();
  from_tests_merge_seq<std::deque<int>, std::forward_list<int>>();
  from_tests_merge_seq<std::deque<int>, std::set<int>>();
  from_tests_merge_seq<std::deque<int>, std::multiset<int>>();

  from_tests_merge_seq<std::list<int>, std::vector<int>>();
  from_tests_merge_seq<std::list<int>, std::deque<int>>();
  from_tests_merge_seq<std::list<int>, std::list<int>>();
  from_tests_merge_seq<std::list<int>, std::forward_list<int>>();
  from_tests_merge_seq<std::list<int>, std::set<int>>();
  from_tests_merge_seq<std::list<int>, std::multiset<int>>();

  from_tests_merge_seq<std::set<int>, std::vector<int>>();
  from_tests_merge_seq<std::set<int>, std::deque<int>>();
  from_tests_merge_seq<std::set<int>, std::list<int>>();
  from_tests_merge_seq<std::set<int>, std::forward_list<int>>();
  from_tests_merge_seq<std::set<int>, std::set<int>>();
  from_tests_merge_seq<std::set<int>, std::multiset<int>>();

  from_tests_merge_seq<std::multiset<int>, std::vector<int>>();
  from_tests_merge_seq<std::multiset<int>, std::deque<int>>();
  from_tests_merge_seq<std::multiset<int>, std::list<int>>();
  from_tests_merge_seq<std::multiset<int>, std::forward_list<int>>();
  from_tests_merge_seq<std::multiset<int>, std::set<int>>();
  from_tests_merge_seq<std::multiset<int>, std::multiset<int>>();

  from_tests_merge_by_il<std::vector<int>>();
  from_tests_merge_by_il<std::deque<int>>();
  from_tests_merge_by_il<std::list<int>>();
  from_tests_merge_by_il<std::set<int>>();
  from_tests_merge_by_il<std::multiset<int>>();
}

} // namespace merge

namespace where {

template <typename Container>
void where_lambda_test_seq_impl() {
  const Container values = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  const Container assert = {    2,    4,    6,    8    };
  const Container select =
    query::from(values).where([](const auto& element) { return element % 2 == 0; }).to(Container{});
  assert(select == assert);
}

template <typename Container>
void where_gate_test_seq_impl() {
  using query::gate;
  const Container values = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };
  const Container assert = {       3, 4, 5, 6, 7, 8, 9 };
  const Container select =
    query::from(values).where(gate(std::greater_equal<>{}, 3)).to(Container{});
  assert(select == assert);
}

struct human { std::string name; size_t age; };
bool operator==(const human& l, const human& r) {
  return l.name == r.name && l.age == r.age;
}

template <typename Container>
void where_associative_impl() {
  {
    const Container values = { {1,2}, {3,4}, {5,6} };
    const Container assert = {        {3,4}, {5,6} };
    const Container select =
      query::from(values).where_key(query::gate(std::greater_equal<>{}, 3)).to(Container{});
    assert(select == assert);
  } {
    const Container values = { {1,2}, {3,4}, {5,6} };
    const Container assert = {        {3,4}, {5,6} };
    const Container select =
      query::from(values).where_value(query::gate(std::greater_equal<>{}, 4)).to(Container{});
    assert(select == assert);
  } {
    const std::set<int> assert = { 1, 2, 3, 4, 5, 6, 7, 8 };
    const Container        odd = { {1,3}, {5,7} };
    const Container       even = { {2,4}, {6,8} };
    const std::set<int> select = query::from(odd).merge(even).to(std::set<int>{});
    assert (select == assert);
  }
}

void where_by_field_test() {
  const std::vector<human> people = {
    { "John", 42 },
    {  "Rob", 48 },
    { "Alex", 33 },
    {  "Leo", 41 }
  };
  const std::vector<human> assert = {
    { "John", 42 },
    {  "Rob", 48 },
    {  "Max", 45 }
  };
  {
    const std::vector<human> youngest =
      query::from(people)
        .where(&human::age, query::gate(std::greater_equal<>{}, 42))
        .merge({human{"Max", 45}})
        .to(std::vector<human>{});
    assert(youngest == assert);
  } {
    const std::vector<human> youngest =
      query::from(people)
        .where(&human::age, [](auto age){ return age >= 42; })
        .merge({human{"Max", 45}})
        .to(std::vector<human>{});
    assert(youngest == assert);
  }
}

void where_take_count_test() {
  const std::vector<int> values = { 1, 1, 1, 1, 1 };
  const std::vector<int> assert = {             1 };
  const std::vector<int> select =
    query::from(values).take(1).where(query::gate(std::equal_to<>{}, 1)).to(std::vector<int>{});
  assert(select == assert);
}


void where_tests() {
  where_lambda_test_seq_impl<std::vector<int>>();
  where_lambda_test_seq_impl<std::deque<int>>();
  where_lambda_test_seq_impl<std::list<int>>();
  where_lambda_test_seq_impl<std::set<int>>();
  where_lambda_test_seq_impl<std::multiset<int>>();

  where_gate_test_seq_impl<std::vector<int>>();
  where_gate_test_seq_impl<std::deque<int>>();
  where_gate_test_seq_impl<std::list<int>>();
  where_gate_test_seq_impl<std::set<int>>();
  where_gate_test_seq_impl<std::multiset<int>>();

  where_associative_impl<std::map<int, int>>();

  where_by_field_test();
  where_take_count_test();
}

} // namespace where

namespace set {

template <typename Container>
void union_with_seq_test() {
  const Container   values = { 1, 2, 3, 4, 5, 5, 5 };
  const Container to_union = {       3, 4, 5, 6, 7 };
  const Container   assert = { 1, 2, 3, 4, 5, 5, 5, 6, 7 };
  const Container   select =
    query::from(values).union_with(to_union).to(Container{});
  assert(select == assert);
}

void set_tests() {
  union_with_seq_test<std::vector<int>>();
  union_with_seq_test<std::deque<int>>();
  union_with_seq_test<std::list<int>>();
  union_with_seq_test<std::set<int>>();
  union_with_seq_test<std::multiset<int>>();
}

} // namespace set

namespace numeric {

template <typename Container>
void seq_min() {
  Container values = { "1", "2", "3" };
  auto min_value = query::from(values).min();
  assert(min_value == "1");
  auto max_value = query::from(values).max();
  assert(max_value == "3");
  auto sum = query::from(values).sum();
  assert(sum == "123");
}

void numeric_tests() {
  seq_min<std::vector<std::string>>();
}

} // namespace numeric

namespace order {

template <typename Container>
void order_test_sort_impl() {
  Container values = { 3, 1, 2 };
  Container assert = { 1, 2, 3 };
  Container sorted = query::from(values).sort().to(Container{});
  assert(sorted == assert);
}

template <typename Container>
void order_test_reverse_sort_impl() {
  Container values = { 1, 2, 3 };
  Container assert = { 3, 2, 1 };
  Container sorted = query::from(values).reverse_sort().to(Container{});
  assert(sorted == assert);
}

template <typename Container>
void order_test_reverse_impl() {
  Container values = { 1, 3, 2 };
  Container assert = { 2, 3, 1 };
  Container sorted = query::from(values).reverse().to(Container{});
  assert(sorted == assert);
}

void order_tests() {
  order_test_sort_impl<std::vector<int>>();
  order_test_sort_impl<std::deque<int>>();
  order_test_sort_impl<std::list<int>>(); // !
//  order_test_sort_impl<std::forward_list<int>>(); // !
//  order_test_sort_impl<std::set<int>>(); // !
//  order_test_sort_impl<std::multiset<int>>(); // !

  order_test_reverse_sort_impl<std::vector<int>>();
  order_test_reverse_sort_impl<std::deque<int>>();
  order_test_reverse_sort_impl<std::list<int>>();
//  order_test_reverse_sort_impl<std::forward_list<int>>();
//  order_test_reverse_sort_impl<std::set<int>>();
//  order_test_reverse_sort_impl<std::multiset<int>>();

  order_test_reverse_impl<std::vector<int>>();
  order_test_reverse_impl<std::deque<int>>();
  order_test_reverse_impl<std::list<int>>();
//  order_test_reverse_impl<std::forward_list<int>>();
//  order_test_reverse_impl<std::set<int>>();
//  order_test_reverse_impl<std::multiset<int>>();
}

} // namespace order

void complex_test() {
  const std::vector<int> values_1 = { 9,  7,  5,  3,  1 };
  const std::vector<int> values_2 = { 2,  4,  6,  8, 10 };
  const std::set<int> result =
    query::from(values_1)
      .merge(values_2)
      .where(query::gate(std::greater_equal<>{}, 5))
      .merge({11, 12, 13, 14, 15, 16})
      .to(std::set<int>{});
  const std::set<int> assert = {
     5,   6,   7,   8,
     9,  10,  11,  12,
    13,  14,  15,  16
  };
  assert(result == assert);
}

} // namespace test

void query_tests() {
  test::container_traits::test_push();
  test::container_traits::test_clear();
  test::convert::test_convert();
  test::conditional::test_cond();
  test::from::from_tests();
  test::merge::merge_tests();
  test::where::where_tests();
  test::set::set_tests();
  test::numeric::numeric_tests();
  test::order::order_tests();
  test::complex_test();
}
