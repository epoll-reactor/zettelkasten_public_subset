// May 9, 2021
#ifndef SANDBOX_CONTAINER_TRAITS_HPP
#define SANDBOX_CONTAINER_TRAITS_HPP

#include <type_traits>

template<typename T>
class has_const_iterator {
  using yes = struct { char _[1]; };
  using  no = struct { char _[2]; };

  template<typename C> static constexpr yes test(typename C::const_iterator*);
  template<typename C> static constexpr no  test(...);

public:
  using value_type = T;

  static constexpr bool value = sizeof(test<T>(0)) == sizeof(yes);
};

template <typename T>
struct has_begin_end {

  template <typename C>
  static constexpr char (
    &begin_check(
      std::enable_if_t<
        std::is_same_v<
          decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::begin)),
          typename C::const_iterator(C::*)() const
        >,
        void
      >*
    )
  )[1];

  template<typename C>
  static constexpr char (&begin_check(...))[2];

  template <typename C>
  static constexpr char (
    &end_check(
      std::enable_if_t<
        std::is_same_v<
          decltype(static_cast<typename C::const_iterator (C::*)() const>(&C::end)),
          typename C::const_iterator(C::*)() const
        >,
        void
      >*
    )
  )[1];


  template<typename C>
  static constexpr char (&end_check(...))[2];

  static constexpr bool beg_value = sizeof(begin_check<T>(0)) == 1;
  static constexpr bool end_value = sizeof(  end_check<T>(0)) == 1;
};

template <typename T>
struct has_mapped_type{
  using yes = struct { char _[1]; };
  using  no = struct { char _[2]; };

  template<typename C> static constexpr yes test(typename C::mapped_type);
  template<typename C> static constexpr no  test(...);

  using value_type = T;

  static constexpr bool has_type = sizeof(test<T>(0)) == sizeof(yes);
};

#endif //SANDBOX_CONTAINER_TRAITS_HPP
