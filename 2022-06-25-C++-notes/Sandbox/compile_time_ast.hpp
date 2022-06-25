// Jan 24, 2021
template <auto Tag>
struct UniqueValue {
  static constexpr int Value{0};
};

template <auto Tag = []{}>
constexpr auto UniqueReturn() {
  return &UniqueValue<Tag>::Value;
}

template <typename T, auto Tag = UniqueReturn()>
struct Atom {
  const T Value;
  constexpr auto operator()() const {
    return Value;
  }
};

template <typename LHSType, typename RHSType,
  typename Tag,     auto     Reducer
>
struct Binop {
  const LHSType &LHS;
  const RHSType &RHS;
  constexpr auto operator()() const {
    return Reducer(LHS, RHS);
  }
};

template <typename Tag,     auto     Reducer,
  typename LHSType, typename RHSType
>
constexpr auto MakeBinop(const LHSType &LHS, const RHSType &RHS) {
  return Binop<LHSType, RHSType, Tag, Reducer>{LHS, RHS};
}

template <typename T>
struct Rule {
  constexpr auto operator()() const -> T {}
};

struct AddTag {};
struct Add {
  template <typename LHSType, auto LHSTag,
    typename RHSType, auto RHSTag
  >
  constexpr auto operator()(
    const Atom<LHSType, LHSTag> &LHS,
    const Atom<RHSType, RHSTag> &RHS
  ) const {
    return LHS.Value + RHS.Value;
  }

  template <typename LHSType, typename RHSType,
    typename LHSTag,  auto     Reducer,
    typename Type,    auto     RHSTag
  >
  constexpr auto operator()(
    const Binop<LHSType, RHSType, LHSTag, Reducer> &LHS,
    const Atom <Type, RHSTag>                      &RHS
  ) const {
    return LHS() + RHS.Value;
  }

  template <typename LHSType, auto     LHSTag,
    typename RHSType, typename Type,
    typename RHSTag,  auto     Reducer
  >
  constexpr auto operator()(
    const Atom<LHSType, LHSTag> &LHS,
    const Binop<RHSType, Type, RHSTag, Reducer> &RHS
  ) const {
    return LHS.Value + RHS();
  }

  template <typename LHSType1, typename RHSType1,
    typename LHSTag1,  auto     Reducer1,
    typename LHSType2, typename RHSType2,
    typename LHSTag2,  auto     Reducer2
  >
  constexpr auto operator()(
    const Binop<LHSType1, RHSType1, LHSTag1, Reducer1> &LHS,
    const Binop<LHSType2, RHSType2, LHSTag2, Reducer2> &RHS
  ) const {
    return LHS() + RHS();
  }
};

struct SubTag {};
struct Sub {
  template <typename LHSType, auto LHSTag,
    typename RHSType, auto RHSTag
  >
  constexpr auto operator()(
    const Atom<LHSType, LHSTag> &LHS,
    const Atom<RHSType, RHSTag> &RHS
  ) const {
    return LHS.Value - RHS.Value;
  }

  template <typename LHSType, typename RHSType,
    typename LHSTag,  auto     Reducer,
    typename Type,    auto     RHSTag
  >
  constexpr auto operator()(
    const Binop<LHSType, RHSType, LHSTag, Reducer> &LHS,
    const Atom <Type, RHSTag>                      &RHS
  ) const {
    return LHS() - RHS.Value;
  }

  template <typename LHSType, auto     LHSTag,
    typename RHSType, typename Type,
    typename RHSTag,  auto     Reducer
  >
  constexpr auto operator()(
    const Atom<LHSType, LHSTag> &LHS,
    const Binop<RHSType, Type, RHSTag, Reducer> &RHS
  ) const {
    return LHS.Value - RHS();
  }

  template <typename LHSType1, typename RHSType1,
    typename LHSTag1,  auto     Reducer1,
    typename LHSType2, typename RHSType2,
    typename LHSTag2,  auto     Reducer2
  >
  constexpr auto operator()(
    const Binop<LHSType1, RHSType1, LHSTag1, Reducer1> &LHS,
    const Binop<LHSType2, RHSType2, LHSTag2, Reducer2> &RHS
  ) const {
    return LHS() - RHS();
  }
};

template <typename LHS, typename RHS>
Binop(LHS, RHS) -> Binop<LHS, RHS, AddTag, Add{}>;

void compile_time_ast_tests() {
  static constexpr Atom<int> X{1};
  static constexpr Atom<float> Y{42.};

  static constexpr auto SumXY = Binop{X, Y};
  static constexpr auto SumXXY = Binop{X, SumXY};

  static_assert(SumXY() == 43);
  static_assert(SumXXY() == 44);
}
