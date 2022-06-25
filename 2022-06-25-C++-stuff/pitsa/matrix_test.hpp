#pragma once

#define matrix_num_threads 8

#include <chrono>
#include <boost/numeric/ublas/matrix.hpp>
#include <gtest/gtest.h>

#include "matrix.hpp"


using namespace haifisch;

template <typename T>
void debug_info(std::string_view label, const matrix<T>& rhs) noexcept
{
    std::cout << label << " capacity:   " << rhs.width() * rhs.height() << std::endl;
    std::cout << label << " memory:     " << ((rhs.width() * rhs.height()) * sizeof (T)) / 1024.0 / 1024.0 << " MiB." << std::endl;
}

template <typename T>
bool is_matrix_eq(const matrix<T>& lhs, const matrix<T>& rhs) noexcept
{
    return lhs == rhs;
}

template <typename T>
bool is_matrix_not_eq(const matrix<T>& lhs, const matrix<T>& rhs) noexcept
{
    return lhs != rhs;
}

template <typename T>
bool is_matrix_add(const matrix<T>& lhs, const matrix<T>& rhs, const matrix<T>& control) noexcept
{
    return (lhs + rhs) == control;
}

template <typename T>
bool is_matrix_sub(const matrix<T>& lhs, const matrix<T>& rhs, const matrix<T>& control) noexcept
{
    return (lhs - rhs) == control;
}

template <typename T>
bool is_matrix_mul(const matrix<T>& lhs, const matrix<T>& rhs, const matrix<T>& control) noexcept
{
    return (lhs * rhs) == control;
}

template <typename T>
bool is_matrix_transpose(matrix<T>& rhs, const matrix<T>& control) noexcept
{
    transpose(rhs);
    return (rhs == control);
}

template <typename T>
bool is_eq_with_boost_ublas(const matrix<T>& lhs, const boost::numeric::ublas::matrix<T>& rhs) noexcept
{
    if (lhs.height() != rhs.size1()) return false;
    if (lhs.width() != rhs.size2()) return false;

    for (std::size_t i = 0; i < lhs.height(); i++)
    {
        for (std::size_t j = 0; j < lhs.width(); j++)
        {
            if (lhs(i, j) != rhs(i, j))
            {
                return false;
            }
        }
    }

    return true;
}

template <typename T>
matrix<T> gen_matrix(const std::size_t size = 256, const std::size_t val = 0)
{
    matrix<T> mat(size, size);
    mat.fill(val);
    return mat;
}

template <typename T>
bool test_add(const std::size_t size = 256)
{
    matrix<T> template_mat = gen_matrix<T>(size, 10);
    matrix<T> res = gen_matrix<T>(size, 20);
    return (template_mat + template_mat) == res;
}

template <typename T>
bool test_sub(const std::size_t size = 256)
{
    matrix<T> template_mat = gen_matrix<T>(size, 10);
    matrix<T> res = gen_matrix<T>(size, 0);
    return is_matrix_sub(template_mat, template_mat, res);
}

template <typename T>
bool test_mul(const std::size_t size = 256)
{
    matrix<T> template_mat = gen_matrix<T>(size, 10);
    boost::numeric::ublas::matrix<T> boost_template_mat(size, size);
    for (std::size_t i = 0; i < size; i++)
    {
        for (std::size_t j = 0; j < size; j++)
        {
            boost_template_mat(i, j) = 10;
        }
    }
    boost::numeric::ublas::matrix<T> boost_res = boost::numeric::ublas::prod(boost_template_mat, boost_template_mat);
    matrix<T> res = template_mat * template_mat;
    return is_eq_with_boost_ublas(res, boost_res);
}

template <typename T>
bool test_different_mul(const std::size_t size = 256)
{
    const std::size_t coef = 1;
    matrix<T> template_mat1(size, size + coef);
    boost::numeric::ublas::matrix<T> boost_template_mat(size, size + coef);
    T counter = 0;
    for (size_t i = 0; i < size; i++)
    {
        for (size_t j = 0; j < size + coef; j++)
        {
            template_mat1(i, j) = ++counter + 0.5;
            boost_template_mat(i, j) = counter + 0.5;
        }
    }

    matrix<T> template_mat2(size + coef, size);
    boost::numeric::ublas::matrix<T> boost_template_mat2(size + coef, size);
    counter = 10;
    for (size_t i = 0; i < size + coef; i++)
    {
        for (size_t j = 0; j < size; j++)
        {
            template_mat2(i, j) = ++counter + 0.5;
            boost_template_mat2(i, j) = counter + 0.5;
        }
    }

    debug_info("mat1", template_mat1);
    debug_info("mat2", template_mat2);

    auto start = std::chrono::high_resolution_clock::now();

    matrix<T> new_mat = template_mat1 * template_mat2;

    auto timeSpent = std::chrono::high_resolution_clock::now() - start;
    std::cout << "haifisch matrix: " << new_mat.width() << "x" << new_mat.height() << "   -> " << "\033[0;31m" << std::chrono::duration_cast<std::chrono::duration<float>>(timeSpent).count() << " s." << "\033[0;0m" << std::endl;

    start = std::chrono::high_resolution_clock::now();

    boost::numeric::ublas::matrix<T> boost_res = boost::numeric::ublas::prod(boost_template_mat, boost_template_mat2);

    auto timeSpent2 = std::chrono::high_resolution_clock::now() - start;
    std::cout << "boost matrix:    " << boost_res.size1() << "x" << boost_res.size2() << "   -> " << "\033[0;31m" << std::chrono::duration_cast<std::chrono::duration<float>>(timeSpent2).count() << " s." << "\033[0;0m" << std::endl;

    (is_eq_with_boost_ublas(new_mat, boost_res))
        ? std::cout << "eq with boost ublas" << std::endl
        : std::cout << "not eq with boost ublas" << std::endl;

    return true;
}

template <typename T>
bool test_transpose(const std::size_t cols, const std::size_t rows, const T value)
{
    matrix<T> template_mat(cols, rows);
    template_mat.fill(value);
    matrix<T> transpose_control(rows, cols);
    transpose_control.fill(value);
    template_mat.transpose();
    return template_mat == transpose_control;
}

template <typename T>
bool test_vec_mul(const std::size_t size)
{
    matrix<T> template_mat(size, size);
    vector<T> template_vec(size);

    template_mat.fill(10);

    for (std::size_t i = 0; i < size; i++)
    {
        template_vec.at(i) = i;
    }

    vector<T> res_vec = template_mat *= template_vec;
    return true;
}

TEST(arithmetic_test, arithmetic)
{
    ASSERT_TRUE(test_mul<int>(10));
    ASSERT_TRUE(test_mul<int>(20));
    ASSERT_TRUE(test_mul<int>(30));
    ASSERT_TRUE(test_sub<int>(200));
    ASSERT_TRUE(test_mul<int>(300));
    ASSERT_TRUE(test_add<float>(100));
    ASSERT_TRUE(test_sub<float>(200));
    ASSERT_TRUE(test_mul<float>(300));
    ASSERT_TRUE(test_add<double>(100));
    ASSERT_TRUE(test_sub<double>(200));
    ASSERT_TRUE(test_mul<double>(300));
}

TEST(transpose_test, transpose)
{
    ASSERT_TRUE(test_transpose<int>(32, 64, 10));
    ASSERT_TRUE(test_transpose<int>(64, 128, 10));
    ASSERT_TRUE(test_transpose<int>(128, 256, 10));
    ASSERT_TRUE(test_transpose<int>(256, 512, 10));
    ASSERT_TRUE(test_transpose<int>(256, 512, 10));
    ASSERT_TRUE(test_transpose<int>(512, 1024, 10));
    ASSERT_TRUE(test_transpose<int>(1024, 2048, 10));
    ASSERT_TRUE(test_transpose<int>(2058, 4096, 10));
}

// CPU: AMD Ryzen 5 2500U.

//  [int]    [500x500]   [8 threads] haifisch mul       -> 0.05766 s.
//  [int]    [500x500]   [1 thread ] boost ublas mul    -> 3.25886 s.

//  [int]    [1000x1000] [8 threads] haifisch mul       -> 0.66614 s.
//  [int]    [1000x1000] [1 thread ] boost ublas mul    -> 25.6260 s.

//  [int]    [1500x1500] [8 threads] haifisch mul       -> 3.05941 s.
//  [int]    [1500x1500] [1 thread ] boost ublas mul    -> 79.8228 s.

//  [float]  [500x500]   [8 threads] haifisch mul       -> 0.05239 s.
//  [float]  [500x500]   [1 thread ] boost ublas mul    -> 3.10974 s.

//  [float]  [1000x1000] [8 threads] haifisch mul       -> 0.70433 s.
//  [float]  [1000x1000] [1 thread ] boost ublas mul    -> 25.2708 s.

//  [float]  [1500x1500] [8 threads] haifisch mul       -> 3.10457 s.
//  [float]  [1500x1500] [1 thread ] boost ublas mul    -> 83.8923 s.

//  [double] [500x500]   [8 threads] haifisch mul       -> 0.08510 s.
//  [double] [500x500]   [1 thread ] boost ublas mul    -> 3.41337 s.

//  [double] [1000x1000] [8 threads] haifisch mul       -> 0.89308 s.
//  [double] [1000x1000] [1 thread ] boost ublas mul    -> 26.4036 s.

//  [double] [1500x1500] [8 threads] haifisch mul       -> 3.80144 s.
//  [double] [1500x1500] [1 thread ] boost ublas mul    -> 92.1564 s.
