#include <variant>

struct Foo {
  operator std::variant<int, float>() {
    return 0;
  }
};

constexpr void foo() {
  std::variant<int, float, Foo> var1 = 0;
  std::variant<int, float> var2;
  var2 = std::visit([](auto arg) -> decltype(var2) { return arg; }, var1);
}

int main(int argc, char** argv) {
  return 0;
}
