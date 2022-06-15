#include <assert.h>
#include <string.h>
#include <iostream>

struct base_1 {
  void from_base_1() {}
};

struct base_2 {
  void from_base_2() {}
};

struct derived : base_1, base_2 {
  void from_derived_1() {}
  int from_derived_2() { return 1; }
  virtual void invalid_vtable_1() {}
  virtual int invalid_vtable_2() { return 1; }
  virtual int invalid_vtable_3() { return 1; }
};

struct placeholder {
  void from_derived_1() {}
  int from_derived_2() { return 2; }
  virtual void invalid_vtable_1() {}
  virtual int invalid_vtable_2() { return 2; }
  virtual void invalid_vtable_3() {}
};

typedef void (    derived::*derived_void)();
typedef  int (    derived::*derived_int)();
typedef  int (placeholder::*placeholder_int)();

template <typename To, typename From>
To bitcast(From f) {
  assert(sizeof(To) == sizeof(From));
  To t;
  memcpy(&t, &f, sizeof(f));
  return t;
}

int main(int argc, char **argv) {
  if (argc == 1) {
    std::cout << "a: non-virtual function pointer  int (derived::*)()\n";
    std::cout << "b: non-virtual function pointer  int (placeholder::*)()\n";
    std::cout << "c:     virtual function pointer  int (derived::*)() vtable of type derived\n";
    std::cout << "d:     virtual function pointer  int (derived::*)() vtable of type placeholder\n";
    std::cout << "e:     virtual function pointer void (derived::*)()\n";
    std::cout << "f: (derived.*&base_1::from_base_1)()\n";
    std::cout << "g: (derived.*&base_2::from_base_2)()\n";
    return 1;
  }
  derived der;
  placeholder pholder;

  switch (argv[1][0]) {
    case 'a':
      (der.*bitcast<int(derived::*)()>(&derived::from_derived_1))();
      break;
    case 'b':
      (pholder.*bitcast<int(placeholder::*)()>(&derived::from_derived_2))();
      break;
    case 'c':
      (der.*bitcast<int(derived::*)()>(&derived::invalid_vtable_1))();
      break;
    case 'd':
      (reinterpret_cast<derived&>(pholder).*&derived::invalid_vtable_2)();
      break;
    case 'e':
      (der.*bitcast<void(derived::*)()>(&placeholder::invalid_vtable_3))();
      break;
  }
}
