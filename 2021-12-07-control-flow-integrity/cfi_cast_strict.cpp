#include <iostream>

struct Base {
    virtual ~Base() = default;
    virtual void func() {
        std::cout << "Base: " << __FUNCTION__ << "\n";
    } 
};

struct Derived : Base {};

int main() {
    Base b;

    // это UB, которое чаще всего будет работать
    Derived& d = static_cast<Derived&>(b);
    d.func();
}
