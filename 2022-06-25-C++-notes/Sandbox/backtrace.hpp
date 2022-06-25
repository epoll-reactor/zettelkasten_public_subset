#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <execinfo.h>
#include <cxxabi.h>

#include <iostream>


class backtrace_view
{
public:
    explicit backtrace_view(size_t lineno) noexcept
        : line(lineno)
    {
        generate();
        std::cerr << std::endl;
    }
    ~backtrace_view()
    {
        free(symbol_list);
    }
private:
    void generate() noexcept
    {
        std::cerr << "<backtrace>" << std::endl;
        int backtrace_size = ::backtrace(address_list, sizeof(address_list) / sizeof(void*));
        if (backtrace_size == 0)
        {
            std::cerr << indent << "<empty>" << std::endl;
            return;
        }
        symbol_list = ::backtrace_symbols(address_list, backtrace_size);
        address_loop(backtrace_size);
    }
    void address_loop(size_t backtrace_size) noexcept
    {
        for (size_t i = 1; i < backtrace_size; i++)
        {
            get_address_info(i);
            print(i);
        }
    }
    void get_address_info(size_t i) noexcept
    {
        for (char *p = symbol_list[i]; *p; ++p)
        {
            switch (*p)
            {
                case '(': begin_name    = p; break;
                case '+': begin_offset  = p; break;
                case ')': end_offset    = p; return;
            }
        }
    }
    void print(size_t i) noexcept
    {
        if (begin_name && begin_offset && end_offset && begin_name < begin_offset)
        {
            char* func_name = new char[max_func_length];
            *begin_name++   = '\0';
            *begin_offset++ = '\0';
            *end_offset++   = '\0';
            print_impl(func_name, i);
        }
        else std::cerr << indent << symbol_list[i] << std::endl;
    }
    void print_impl(char* func_name, size_t i) noexcept
    {
        int status;
        char* ret = abi::__cxa_demangle(begin_name, func_name, &max_func_length, &status);
        if (status == 0)
        {
            func_name = ret;
            std::cerr << indent << symbol_list[i] << ':' << line << ' ' << func_name << '+' << begin_offset << std::endl;
        }
        else
        {
            std::cerr << indent << symbol_list[i] << ':' << line << ' ' << begin_name << "()+" << begin_offset << std::endl;
        }
        delete[] func_name;
    }

    const size_t line;
    static constexpr const char* indent = "    ";
    static constexpr size_t max_frames = 64;
    size_t max_func_length = 256;
    void* address_list[max_frames];

    char** symbol_list   = nullptr;
    char*  begin_name    = nullptr;
    char*  begin_offset  = nullptr;
    char*  end_offset    = nullptr;
};
