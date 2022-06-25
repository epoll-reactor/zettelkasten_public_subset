// Aug 8, 2021
#ifndef SANDBOX_EXCEPTION_HOLDER_HPP
#define SANDBOX_EXCEPTION_HOLDER_HPP

#include "function_benchmarking.hpp"

#include <stdexcept>
#include <cassert>

template <typename T>
class Expected
{
    union
    {
        T m_payload;
        std::exception_ptr m_exception_ptr;
    };

    bool m_has_exception;

public:
    Expected() noexcept
        : m_payload()
        , m_has_exception(false) {}

    explicit Expected(T&& data)
        : m_payload(std::forward<T>(data))
        , m_has_exception(false) {}

    explicit Expected(std::exception&& ex) noexcept
        : m_exception_ptr(std::make_exception_ptr(std::move(ex)))
        , m_has_exception(true) {}

    Expected(const Expected& ec)
    {
        if (ec.m_has_exception)
        {
            m_exception_ptr = ec.m_exception_ptr;
            m_has_exception = true;
        }
        else {
            m_payload = ec.m_payload;
            m_has_exception = false;
        }
    }

    Expected(Expected&& ec) noexcept
    {
        if (ec.m_has_exception)
        {
            m_exception_ptr = ec.m_exception_ptr;
            m_has_exception = true;
        }
        else {
            m_payload = std::move(ec.m_payload);
            m_has_exception = false;
        }
    }

    ~Expected()
    {
        if (m_has_exception)
        {
            m_exception_ptr.~exception_ptr();
        }
        else {
            m_payload.~T();
        }
    }

    const T& payload() const noexcept { assert(!m_has_exception); return m_payload; }
    T& payload()             noexcept { assert(!m_has_exception); return m_payload; }

    std::exception_ptr& exception() noexcept { assert(m_has_exception); return m_exception_ptr; }
    void rethrow()                           { assert(m_has_exception); std::rethrow_exception(m_exception_ptr); }

    explicit operator T() { return m_payload; }
};

int throw_f()
{
    throw std::runtime_error("");
}

Expected<int> Expected_exception() noexcept
{
    return Expected<int>(std::runtime_error(""));
}

void exception_holder_tests()
{
    fun_benchmark(10000, []{
        try { throw_f(); } catch (...) { }
    });
    fun_benchmark(10000, []{
        try { Expected_exception().rethrow(); } catch (...) { }
    });
    fun_benchmark(10000, []{
        Expected_exception().exception();
    });
    fun_benchmark(10000, []{
        Expected<int>{}.payload();
    });
}


#endif //SANDBOX_EXCEPTION_HOLDER_HPP
