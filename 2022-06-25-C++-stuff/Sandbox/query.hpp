// Nov 28, 2021
#ifndef QUERY_HPP
#define QUERY_HPP

#include <iostream>
#include <vector>
#include <list>
#include <queue>
#include <stack>
#include <forward_list>
#include <set>
#include <map>
#include <unordered_map>
#include <optional>
#include <cassert>
#include <functional>

namespace query {
namespace container_traits {
/*!
 * Check if type has `iterator` typename
 * @tparam Container type
 */
template<typename T>
class has_iterator final {
private:
  template<typename C> static constexpr std::true_type  test_for_iterator(typename C::iterator*);
  template<typename  > static constexpr std::false_type test_for_iterator(...);

public:
  static constexpr bool value = decltype(test_for_iterator<T>(nullptr))::value;
};
/*!
 * Check if type has `mapped_type` typename
 * @tparam Container type
 */
template <typename T>
class has_mapped_type final {
private:
  template<typename C> static constexpr std::true_type  test_for_mapped_type(typename C::mapped_type*);
  template<typename  > static constexpr std::false_type test_for_mapped_type(...);

public:
  static constexpr bool value = decltype(test_for_mapped_type<T>(nullptr))::value;
};
/*!
 * Check if type has `traits_type` typename
 * @tparam Container type
 * @note   used only to determine std::string's, because they contains `traits_type`
 */
template <typename T>
class has_traits_type final {
private:
  template<typename C> static constexpr std::true_type  test_for_traits_type(typename C::traits_type*);
  template<typename  > static constexpr std::false_type test_for_traits_type(...);

public:
  static constexpr bool value = decltype(test_for_traits_type<T>(nullptr))::value;
};
/*!
 * Check if `<=>` operator in namespace std on type exists
 */
template <typename T>
class has_three_way_comparator final {
private:
  template <typename C> static constexpr auto check_for_operator(C*) -> typename
    std::is_same<
      decltype(std::operator<=>(std::declval<C>(), std::declval<C>())),
      std::strong_ordering
    >::type;
  template <typename> static constexpr std::false_type check_for_operator(...);

public:
  static constexpr bool value = decltype(check_for_operator<T>(nullptr))::value;
};
/*!
 * Check if `+` operator in namespace std on type exists
 */
template <typename T>
class has_plus_operator final {
private:
  template <typename C> static constexpr auto check_for_operator(C*) -> typename
  std::is_same<
    decltype(std::operator+(std::declval<C>(), std::declval<C>())),
    C
  >::type;
  template <typename> static constexpr std::false_type check_for_operator(...);

public:
  static constexpr bool value = decltype(check_for_operator<T>(nullptr))::value;
};

template <typename T>
struct is_sequence_container final : std::integral_constant<
  bool,
  has_iterator   <T>::value &&
 !has_mapped_type<T>::value > {};

template <typename T>
struct is_associative_container final : std::integral_constant<
  bool,
  has_iterator   <T>::value &&
  has_mapped_type<T>::value > {};

template <typename T>
struct is_basic_string final : std::integral_constant<
  bool,
  has_iterator   <T>::value &&
  has_traits_type<T>::value > {};

template <typename... Args> struct need_emplace_param                         final : std::false_type {};
template <typename... Args> struct need_emplace_param<std::vector<Args...>>   final : std:: true_type {};
template <typename... Args> struct need_emplace_param<std::list<Args...>>     final : std:: true_type {};
template <typename... Args> struct need_emplace_param<std::deque<Args...>>    final : std:: true_type {};

template <typename... Args> struct has_not_clear_method                       final : std::false_type {};
template <typename... Args> struct has_not_clear_method<std::stack<Args...>>  final : std:: true_type {};
template <typename... Args> struct has_not_clear_method<std::queue<Args...>>  final : std:: true_type {};

template <typename... Args> struct need_push_back final : std::false_type {};
template <typename... Args> struct need_push_back<std::basic_string<Args...>> final : std::true_type {};

template <typename Container, typename... Args>
  requires (need_emplace_param<Container>::value)
void any_push(Container& container, Args&&... values) {
  container.emplace(container.end(), std::forward<Args>(values)...);
}

template <typename Container, typename... Args>
  requires (need_push_back<Container>::value)
void any_push(Container& container, Args&&... values) {
  container.push_back(std::forward<Args>(values)...);
}

template <typename Container, typename... Args>
void any_push(Container& container, Args&&... values) {
  container.emplace(std::forward<Args>(values)...);
}

template <typename Container>
void any_clear(Container& container) noexcept {
  if constexpr (!has_not_clear_method<Container>::value) {
    container.clear();
  }
}

}// namespace container_traits
}// namespace query

namespace query {
/*!
 * Container cast implemented on next type categories:
 *    Sequence ->    Sequence
 *    Sequence ->      String
 * Associative ->      String
 * Associative -> Associative
 * Associative ->    Sequence
 */
template <typename From, typename To>
class cast final {
public:
  using from_type = From;
  using   to_type = To;

  static_assert(
    container_traits::is_sequence_container   <from_type>::value ||
    container_traits::is_associative_container<from_type>::value  ,
    "Source type is not container");
  static_assert(
    container_traits::is_sequence_container   <to_type>::value   ||
    container_traits::is_associative_container<to_type>::value   ||
    container_traits::is_basic_string         <to_type>::value    ,
    "Target type is neither container nor string");

  to_type operator()(const from_type& from)
    requires (
      container_traits::is_sequence_container<from_type>::value &&
      container_traits::is_sequence_container<  to_type>::value &&
     !container_traits::is_basic_string      <  to_type>::value
    ) || (
      container_traits::is_associative_container<from_type>::value &&
      container_traits::is_associative_container<  to_type>::value &&
     !container_traits::is_basic_string         <  to_type>::value )
  { return same_to_same(from); }

  to_type operator()(const from_type& from)
    requires (
      container_traits::is_sequence_container<from_type>::value &&
      container_traits::is_basic_string      <  to_type>::value )
  { return seq_to_string(from); }

  to_type operator()(const from_type& from)
    requires (
      container_traits::is_associative_container<from_type>::value &&
      container_traits::is_sequence_container   <  to_type>::value &&
     !container_traits::is_basic_string         <  to_type>::value )
  { return ass_to_seq(from); }

  to_type operator()(const from_type& from)
    requires (
      container_traits::is_associative_container<from_type>::value &&
      container_traits::is_basic_string         <  to_type>::value )
  { return ass_to_string(from); }

private:
  template <typename T>
  auto to_string(const T& element) {
    std::string output;
    if constexpr (std::is_integral_v<std::decay_t<T>>) {
      output += std::to_string(element);
    } else {
      output += element;
    }
    return output;
  }

  to_type same_to_same(const from_type& sequence) {
    return {std::begin(sequence), std::end(sequence)};
  }

  auto seq_to_string(const from_type& sequence) {
    std::string result;
    for (auto&& element : sequence) {
      result += to_string(element) += ' ';
    }
    if (!result.empty()) {
      result.pop_back();
    }
    return result;
  }

  to_type ass_to_seq(const from_type& associative) {
    to_type result;
    for (auto&&[k, v] : associative) {
      container_traits::any_push(result, k);
      container_traits::any_push(result, v);
    }
    return result;
  }

  auto ass_to_string(const from_type& associative) {
    std::string result;
    for (const auto&[k, v] : associative) {
      result += '(';
      result += to_string(k);
      result += ", ";
      result += to_string(v);
      result += ')';
    }
    return result;
  }
};

} // namespace query

namespace query {
/*!
 * Container merge implemented on next type categories:
 *    Sequence ->    Sequence
 * Associative -> Associative
 */
template <typename Target, typename ToMerge>
class merge final {
public:
  using   target_type = Target;
  using to_merge_type = ToMerge;

  static_assert(
    (
      container_traits::is_sequence_container<  target_type>::value &&
      container_traits::is_sequence_container<to_merge_type>::value
    ) || (
      container_traits::is_associative_container<  target_type>::value &&
      container_traits::is_associative_container<to_merge_type>::value
    ),
    "Both sequence or associative containers expected"
  );

  void operator()(target_type& target, const to_merge_type& to_merge)
    requires (
      container_traits::is_sequence_container<  target_type>::value &&
      container_traits::is_sequence_container<to_merge_type>::value )
  { merge_sequence(target, to_merge); }

  void operator()(target_type& target, const to_merge_type& to_merge)
    requires (
      container_traits::is_associative_container<  target_type>::value &&
      container_traits::is_associative_container<to_merge_type>::value )
  { merge_associative(target, to_merge); }

private:
  void merge_sequence(target_type& target, const to_merge_type& to_merge) {
    for (const auto& value : to_merge) {
      container_traits::any_push(target, value);
    }
  }

  void merge_associative(target_type& target, const to_merge_type& to_merge) {
    for (const auto&[key, value] : to_merge) {
      container_traits::any_push(target, key, value);
    }
  }
};

} // namespace query

namespace query {
/*!
 * Actually to rework...
 */
template <typename Comparator, typename T>
class gate final {
public:
  using      value_type = T;
  using comparator_type = Comparator;

  static_assert(std::is_move_constructible_v<value_type>, "T requires to be move constructible");

  constexpr explicit gate(comparator_type comparator, value_type left, value_type right) noexcept(std::is_nothrow_constructible_v<comparator_type>)
    : comparator_   (std::forward<Comparator>(comparator))
    , left_         (std::move(left))
    , right_        (std::move(right))
    , logical_value_(comparator_(left_, right_)) {}

  constexpr explicit gate(comparator_type comparator, value_type value) noexcept(std::is_nothrow_constructible_v<comparator_type>)
    : comparator_   (std::forward<comparator_type>(comparator))
    , left_         (std::move(value))
    , right_        ()
    , logical_value_(false) {}

  constexpr explicit operator bool() const noexcept(is_noexcept_comparator_) {
    return logical_value_;
  }

  constexpr bool operator &&(const gate& other) const noexcept(is_noexcept_comparator_) {
    return logical_value_ && other.logical_value_;
  }

  constexpr bool operator ||(const gate& other) const noexcept(is_noexcept_comparator_) {
    return logical_value_ || other.logical_value_;
  }

  constexpr bool compare_with(const T& value) const noexcept(is_noexcept_comparator_) {
    return comparator_(value, left_);
  }

private:
  static constexpr bool is_noexcept_comparator_ = noexcept(comparator_type()(std::declval<T>(), std::declval<T>()));
  const comparator_type comparator_;
  const T               left_;
  const T               right_;
  bool                  logical_value_;
};

} // namespace query

namespace query {
/*!
 * Implementation of something similar to SELECT from SQL.
 */
template <typename Buffer>
class where final {
public:
  using buffer_type = Buffer;

  static_assert(
    container_traits::is_sequence_container   <buffer_type>::value ||
    container_traits::is_associative_container<buffer_type>::value  ,
    "Both sequence or associative containers expected");

  enum struct select_policy { by_key, by_value, none };

  explicit where(buffer_type& buffer, ssize_t to_take) : buffer_(buffer), to_take_(to_take) {}

  template <typename Gate>
  void by_gate(const Gate& logical_gate) {
    where_sequence([&](const auto& element) { return logical_gate.compare_with(element); });
  }

  template <typename Field, typename Gate>
  void by_gate(Field field, const Gate& logical_gate) {
    where_sequence([&](const auto& element) { return logical_gate.compare_with(element.*field); });
  }

  template <typename Lambda>
  void by_lambda(Lambda lambda) {
    where_sequence([&](const auto& element) { return lambda(element); });
  }

  template <typename Field, typename Lambda>
  void by_lambda(Field field, Lambda lambda) {
    where_sequence([&](const auto& element) { return lambda(element.*field); });
  }

  template <typename Gate>
  void by_gate(enum select_policy policy, const Gate& logical_gate) {
    where_associative([&](const auto& key, const auto& value) {
      return logical_gate.compare_with((policy == select_policy::by_key) ? key : value);
    });
  }

  template <typename Field, typename Gate>
  void by_gate(enum select_policy policy, Field field, const Gate& logical_gate) {
    where_associative([&](const auto& key, const auto& value) {
      return logical_gate.compare_with((policy == select_policy::by_key) ? key.*field : value.*field);
    });
  }

  template <typename Lambda>
  void by_lambda(enum select_policy policy, Lambda lambda) {
    where_associative([&](const auto& key, const auto& value) {
      return (policy == select_policy::by_key) ? lambda(key) : lambda(value);
    });
  }

  template <typename Field, typename Lambda>
  void by_lambda(enum select_policy policy, Field field, Lambda lambda) {
    where_associative([&](const auto& key, const auto& value) {
      return (policy == select_policy::by_key) ? lambda(key.*field) : lambda(value.*field);
    });
  }

private:
  template <typename Comparator>
  void where_sequence(Comparator comparator) {
    Buffer new_buffer;
    ssize_t total_found = 0;
    for (const auto& element : buffer_) {
      if (total_found == to_take_) {
        break;
      }
      if (comparator(element)) {
        container_traits::any_push(new_buffer, element);
        ++total_found;
      }
    }
    buffer_ = std::move(new_buffer);
  }

  template <typename Comparator>
  void where_associative(Comparator comparator) {
    Buffer new_buffer;
    ssize_t total_found = 0;
    for (const auto&[key, value] : buffer_) {
      if (total_found == to_take_) {
        break;
      }
      if (comparator(key, value)) {
        container_traits::any_push(new_buffer, key, value);
        ++total_found;
      }
    }
    buffer_ = std::move(new_buffer);
  }

  buffer_type& buffer_;
  ssize_t      to_take_;
};

} // namespace query

namespace query {
/*!
 * Set operation implementation (union, intersect, difference)
 * \todo Make something with insert iterators
 */
template <typename Buffer>
class set_operation final {
public:
  using buffer_type = Buffer;

  explicit set_operation(buffer_type& buffer) : buffer_(buffer) {}

  static_assert(
    container_traits::is_sequence_container   <buffer_type>::value ||
    container_traits::is_associative_container<buffer_type>::value,
    "Both sequence or associative containers expected"
  );

  template <typename Target>
  void union_with(const Target& with) {
    buffer_type new_buffer;
    std::set_union(buffer_.begin(), buffer_.end(), with.begin(), with.end(), std::inserter(new_buffer, new_buffer.end()));
    buffer_ = std::move(new_buffer);
  }

  template <typename Target>
  void intersect_with(const Target& with) {
    buffer_type new_buffer;
    std::set_intersection(buffer_.begin(), buffer_.end(), with.begin(), with.end(),  std::inserter(new_buffer, new_buffer.end()));
    buffer_ = std::move(new_buffer);
  }

  template <typename Target>
  void difference_with(const Target& with) {
    buffer_type new_buffer;
    std::set_difference(buffer_.begin(), buffer_.end(), with.begin(), with.end(),  std::inserter(new_buffer, new_buffer.end()));
    buffer_ = std::move(new_buffer);
  }

private:
  buffer_type& buffer_;
};

} // namespace query

namespace query {
/*!
 * Numeric operations (min, max, sum) implementation
 */
template <typename Buffer>
class numeric final {
public:
  using buffer_type = Buffer;
  using  value_type = typename Buffer::value_type;

  static_assert(
    container_traits::is_sequence_container<buffer_type>::value ||
    container_traits::is_basic_string      <buffer_type>::value ,
    "Sequence container or string expected"
  );

  explicit numeric(const buffer_type& buffer) : buffer_(buffer) {}

  value_type min() requires (container_traits::has_three_way_comparator<value_type>::value) {
    value_type min = *buffer_.begin();
    for (const auto& element : buffer_) {
      if (element < min) {
        min = element;
      }
    }
    return min;
  }

  value_type max() requires (container_traits::has_three_way_comparator<value_type>::value) {
    value_type min = *buffer_.begin();
    for (const auto& element : buffer_) {
      if (element > min) {
        min = element;
      }
    }
    return min;
  }

  value_type sum() requires (container_traits::has_plus_operator<value_type>::value) {
    value_type sum;
    for (const auto& element : buffer_) {
      sum = sum + element;
    }
    return sum;
  }

private:
  const buffer_type& buffer_;
};

} // namespace query

namespace query {
/*!
 * Ordering operations implementation (sort, reverse sort, reverse)
 */
template <typename Buffer, typename T = typename Buffer::value_type>
  requires
    (std::is_same_v<Buffer, std::vector      <T>>) ||
    (std::is_same_v<Buffer, std::deque       <T>>) ||
    (std::is_same_v<Buffer, std::list        <T>>) ||
    (std::is_same_v<Buffer, std::forward_list<T>>) ||
    (std::is_same_v<Buffer, std::set         <T>>) ||
    (std::is_same_v<Buffer, std::multiset    <T>>)
class order final {
public:
  using buffer_type = Buffer;

  class vector_deque_order final {
  public:
    explicit vector_deque_order(buffer_type& buffer) : buffer_(buffer) {}

    void sort() {
      std::sort(std::begin(buffer_), std::end(buffer_), [](const auto& lhs, const auto& rhs) { return lhs < rhs; });
    }

    void reverse_sort() {
      std::sort(std::begin(buffer_), std::end(buffer_), [](const auto& lhs, const auto& rhs) { return lhs > rhs; });
    }

    void reverse() {
      std::reverse(std::begin(buffer_), std::end(buffer_));
    }

  private:
    buffer_type& buffer_;
  };

  class list_order final {
  public:
    explicit list_order(buffer_type& buffer) : buffer_(buffer) {}

    void         sort() { buffer_.sort(); }
    void reverse_sort() { buffer_.sort(); buffer_.reverse(); }
    void reverse     () { buffer_.reverse(); }

  private:
    buffer_type& buffer_;
  };

public:
  explicit order(buffer_type& buffer) : buffer_(buffer) {}

  void sort()
    requires (std::is_same_v<Buffer, std::vector<T>> || std::is_same_v<Buffer, std::deque<T>>) {
    vector_deque_order order(buffer_);
    order.sort();
  }

  void sort() requires (std::is_same_v<Buffer, std::list<T>> || std::is_same_v<Buffer, std::forward_list<T>>) {
    list_order order(buffer_);
    order.sort();
  }

  void reverse_sort() requires (std::is_same_v<Buffer, std::vector<T>> || std::is_same_v<Buffer, std::deque <T>>) {
    vector_deque_order order(buffer_);
    order.reverse_sort();
  }

  void reverse_sort() requires (std::is_same_v<Buffer, std::list<T>> || std::is_same_v<Buffer, std::forward_list<T>>) {
    list_order order(buffer_);
    order.reverse_sort();
  }

  void reverse() requires (std::is_same_v<Buffer, std::vector<T>> || std::is_same_v<Buffer, std::deque <T>>) {
    vector_deque_order order(buffer_);
    order.reverse();
  }

  void reverse() requires (std::is_same_v<Buffer, std::list<T>> || std::is_same_v<Buffer, std::forward_list<T>>) {
    list_order order(buffer_);
    order.reverse();
  }

private:
  buffer_type& buffer_;
};

} // namespace query

namespace query {
/*!
 * Supported operations:
 * - where
 * - take
 * - merge
 * - sort
 * - reverse sort
 * - reverse
 * - union with
 * - difference with
 * - intersect with
 * - to
 * - min
 * - max
 * - sum
 */
template <
  typename Container,
  typename Buffer                                           = Container,
  template <typename          > typename WherePolicy        = where,
  template <typename          > typename SetOperationPolicy = set_operation,
  template <typename          > typename NumericPolicy      = numeric,
  template <typename          > typename OrderPolicy        = order,
  template <typename, typename> typename MergePolicy        = merge,
  template <typename, typename> typename CastPolicy         = cast
>
class from final {
public:
  using   container_type = Container;
  using      buffer_type = Buffer;
  using     where_policy = WherePolicy<buffer_type>;
  using       set_policy = SetOperationPolicy<buffer_type>;
  template <typename T1>
  using   numeric_policy = NumericPolicy<T1>;
  template <typename T1>
  using     order_policy = OrderPolicy<T1>;
  template <typename T1, typename T2>
  using     merge_policy = MergePolicy<T1, T2>;
  template <typename T1, typename T2>
  using      cast_policy = CastPolicy<T1, T2>;

  explicit from(const container_type& container) : container_(container), elements_to_take_(-1) {}

  template <typename T, typename Comparator>
  from& where(gate<Comparator, T>&& logical_gate) {
    populate_buffer_if_empty();
    where_policy policy(buffer_, elements_to_take_);
    policy.by_gate(logical_gate);
    return *this;
  }

  template <typename Field, typename T, typename Comparator>
  from& where(Field field, gate<Comparator, T>&& logical_gate) {
    populate_buffer_if_empty();
    where_policy policy(buffer_, elements_to_take_);
    policy.by_gate(field, logical_gate);
    return *this;
  }

  template <typename T, typename Comparator>
  from& where_key(gate<Comparator, T>&& logical_gate) {
    populate_buffer_if_empty();
    where_policy policy(buffer_, elements_to_take_);
    policy.by_gate(where_policy::select_policy::by_key, logical_gate);
    return *this;
  }

  template <typename Field, typename T, typename Comparator>
  from& where_key(Field field, gate<Comparator, T>&& logical_gate) {
    populate_buffer_if_empty();
    where_policy policy(buffer_, elements_to_take_);
    policy.by_gate(where_policy::select_policy::by_key, field, logical_gate);
    return *this;
  }

  template <typename T, typename Comparator>
  from& where_value(gate<Comparator, T>&& logical_gate) {
    populate_buffer_if_empty();
    where_policy policy(buffer_, elements_to_take_);
    policy.by_gate(where_policy::select_policy::by_value, logical_gate);
    return *this;
  }

  template <typename Lambda>
  from& where(Lambda lambda) {
    populate_buffer_if_empty();
    where_policy policy(buffer_, elements_to_take_);
    policy.by_lambda(lambda);
    return *this;
  }

  template <typename Field, typename Lambda>
  from& where(Field field, Lambda lambda) {
    populate_buffer_if_empty();
    where_policy policy(buffer_, elements_to_take_);
    policy.by_lambda(field, lambda);
    return *this;
  }

  from& take(ssize_t to_take) noexcept {
    elements_to_take_ = to_take;
    return *this;
  }

  template <typename Target>
  from& merge(const Target& with) {
    merge_impl(with);
    return *this;
  }

  template <typename T>
  from& merge(std::initializer_list<T> with) {
    merge_impl(with);
    return *this;
  }

  from& sort() {
    populate_buffer_if_empty();
    order_policy<buffer_type> policy(buffer_);
    policy.sort();
    return *this;
  }

  from& reverse_sort() {
    populate_buffer_if_empty();
    order_policy<buffer_type> policy(buffer_);
    policy.reverse_sort();
    return *this;
  }

  from& reverse() {
    populate_buffer_if_empty();
    order_policy<buffer_type> policy(buffer_);
    policy.reverse();
    return *this;
  }

  template <typename Target>
  from& union_with(Target&& container) {
    populate_buffer_if_empty();
    set_policy policy(buffer_);
    policy.union_with(std::forward<Target>(container));
    return *this;
  }

  template <typename Target>
  from& intersect_with(Target&& container) {
    populate_buffer_if_empty();
    set_policy policy(buffer_);
    policy.intersect_with(std::forward<Target>(container));
    return *this;
  }

  template <typename Target>
  from& difference_with(Target&& container) {
    populate_buffer_if_empty();
    set_policy policy(buffer_);
    policy.difference_with(std::forward<Target>(container));
    return *this;
  }

  template <typename Target>
  Target to(Target) {
    if (buffer_.empty()) {
      cast_policy<container_type, Target> policy;
      return policy(container_);
    } else {
      cast_policy<buffer_type, Target> policy;
      return policy(buffer_);
    }
  }

  auto min() {
    if (buffer_.empty()) {
      numeric_policy<container_type> policy(container_);
      return policy.min();
    } else {
      populate_buffer_if_empty();
      numeric_policy<buffer_type> policy(buffer_);
      return policy.min();
    }
  }

  auto max() {
    if (buffer_.empty()) {
      numeric_policy<container_type> policy(container_);
      return policy.max();
    } else {
      populate_buffer_if_empty();
      numeric_policy<buffer_type> policy(buffer_);
      return policy.max();
    }
  }

  auto sum() {
    if (buffer_.empty()) {
      numeric_policy<container_type> policy(container_);
      return policy.sum();
    } else {
      populate_buffer_if_empty();
      numeric_policy<buffer_type> policy(buffer_);
      return policy.sum();
    }
  }

private:
  template <typename Target>
  void merge_impl(const Target& with) {
    populate_buffer_if_empty();
    merge_policy<buffer_type, Target> policy;
    policy(buffer_, with);
  }

  void populate_buffer_if_empty() {
    if (buffer_.empty()) {
      merge_policy<buffer_type, container_type> policy;
      policy(buffer_, container_);
    }
  }

  const container_type& container_;
  buffer_type           buffer_;
  ssize_t               elements_to_take_;
};

} // namespace query

#endif // QUERY_HPP
