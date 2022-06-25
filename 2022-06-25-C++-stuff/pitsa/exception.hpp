#pragma once

#include "backtrace.hpp"


namespace exception
{
class common_exception : public std::exception
{
protected:
    explicit common_exception(const char* what_arg, unsigned int line)
        : error(what_arg)
    {
        backtrace_view{line};
    }
    virtual const char* what() const noexcept override
    {
        return error.what();
    }
    std::string create(const std::string& ename, const int id) const
    {
        return "[telegram.exception." + ename + '.' + std::to_string(id) + "]: ";
    }

private:
    std::runtime_error error;
};

#define GENERATE_EXCEPTION_CLASS(EXCEPTION) \
    class EXCEPTION : public common_exception \
    { \
    public: \
        explicit EXCEPTION(int id, const char* what_arg, unsigned int line) \
            : common_exception(std::string(create(#EXCEPTION, id) + what_arg).c_str(), line) \
        { } \
    }; \

GENERATE_EXCEPTION_CLASS(parse_error)
GENERATE_EXCEPTION_CLASS(bad_gate_error)
GENERATE_EXCEPTION_CLASS(access_error)
} // namespace exception

#define TELEGRAM_THROW(EXCEPTION, ID, MESSAGE) \
    throw EXCEPTION(ID, #MESSAGE, __LINE__)

#undef GENERATE_EXCEPTION_CLASS
