#include <iostream>

struct Foo {
    virtual ~Foo() = default;
};

struct Bar {
    virtual ~Bar() = default;
};

int main() {
    void* ptr = new Bar;
    static_cast<void>(static_cast<Foo*>(ptr));
}
