#include <iostream>

struct Base {
    virtual ~Base() {}
    virtual void print() { 
        std::cout << "Base::print\n";
    }
};

struct Derived : Base {
    void print() override {
        std::cout << "Derived::print\n";
    }
};

struct Evil {
    Evil() {}
    virtual ~Evil() {}

    // Должна быть виртуальной, чтобы повторить фокус
    virtual void crash() {
        std::cout << "CFI препядствует такому вызову\n";
        std::cout << "Evil::crash\n";
    }
};

int main() {
    Base* dptr = new Derived;

    dptr->print();    
    dptr = reinterpret_cast<Derived*>(new Evil);
    dptr->print(); // Вызывается Evil::crash
}
