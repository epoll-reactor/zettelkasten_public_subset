// Aug 25, 2021
#ifndef OPTIONAL_HPP
#define OPTIONAL_HPP

#include <utility>
#include <memory>
#include <cassert>

struct BadOptionalAccess {};

template <typename T>
class Optional {
public:
    Optional() = default;

    Optional(const T& elem) {
        copy_data(elem);
    }

    Optional(T&& elem) {
        move_data(std::forward<T>(elem));
    }

    Optional(const Optional& other) {
        copy(other);
    }

    Optional& operator=(const Optional& other) {
        if (this != &other) {
            copy(other);
        }
        return *this;
    }

    Optional& operator=(const T& elem) {
        copy_data(elem);
        return *this;
    }

    Optional& operator=(T&& elem) {
        if (!defined) {
            move_data(std::forward<T>(elem));
        } else {
            *get_data() = std::forward<T>(elem);
        }
        return *this;
    }

    bool has_value() const {
        return defined;
    }

    T& operator*() {
        ensure_defined();
        return *get_data();
    }

    const T& operator*() const {
        ensure_defined();
        return *get_data();
    }

    T* operator->() {
        ensure_defined();
        return get_data();
    }
    const T* operator->() const {
        ensure_defined();
        return get_data();
    }

    T& value() {
        ensure_defined();
        return *get_data();
    }

    const T& value() const {
        ensure_defined();
        return *get_data();
    }

    void reset() {
        if (defined) {
            get_data()->~T();
            defined = false;
        }
    }

    ~Optional() {
        reset();
    }

private:
    /*alignas(T)*/ unsigned char data[sizeof(T)];
    bool defined = false;

    T* get_data() {
        return reinterpret_cast<T*>(data);
    }

    const T* get_data() const {
        return reinterpret_cast<const T*>(data);
    }

    void initialize() {
        if (!defined) {
            new (data) T;
            defined = true;
        }
    }

    void copy_data(const T& elem) {
        if (!defined) {
            new (data) T(elem);
            defined = true;
        } else {
            *get_data() = elem;
        }
    }

    void move_data(T&& elem) {
        if (!defined) {
            new (data) T(std::forward<T>(elem));
            defined = true;
        }
    }

    void copy(const Optional& other) {
        if (other.defined)
            copy_data(*other);
        else
            reset();
    }

    void move(Optional && other) {
        if (other.defined)
            move_data(*other);
        else
            reset();
    }

    void ensure_defined() const {
        if (!defined)
            throw ::BadOptionalAccess();
    }

};

void run_optional_tests() {
    Optional<int> opt;
    assert(!opt.has_value());
    try {
        opt.value();
        assert(false);
    } catch (...) {}
    opt = 1;
    assert(opt.has_value());
    assert(opt.value() == 1);
    assert(*opt == 1);
    opt.reset();
    assert(!opt.has_value());
}

#endif // OPTIONAL_HPP
