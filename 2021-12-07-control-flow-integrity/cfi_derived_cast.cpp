#include <iostream>

struct Base {
    Base(const std::string &s) : name(s) {}
    virtual ~Base() = default;

    std::string name;
};

struct Derived : Base {
    Derived(const std::string &s) : Base(s) {}
    ~Derived() override = default;

    const unsigned long variable  = 0x12345678;

    void printName() {
        std::cout << "I am: " << name << ", my member variable is: " << std::hex << variable << std::endl;
    }
};

int main() {
    Base B("base class");
    Derived D("derived class");

    D.printName();

    // Это UB, которое чаще свего будет работать
    Derived &dptr = static_cast<Derived&>(B);

    dptr.printName();
}
