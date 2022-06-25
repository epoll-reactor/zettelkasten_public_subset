#pragma once

#ifndef MATMUL_HPP
#define MATMUL_HPP

#define use_inline
#ifdef use_inline
# define matrix_inline [[gnu::always_inline]]
#else
#define matrix_inline
# endif // use_inline
#undef use_inline

#ifndef matrix_num_threads
# define matrix_num_threads 4
#endif // matrix_num_threads

#include <iostream>
#include <cassert>
#include <cstring>


namespace haifisch
{
template <typename T>
class vector
{
public:
    using size_type = std::uint32_t;

    matrix_inline explicit vector(size_type size) noexcept
        : vec(new T[size])
        , len(size)
    { }
    matrix_inline virtual ~vector()
    {
        if (vec)
        {
            delete[] vec;
            vec = nullptr;
        }
    }
    matrix_inline T& at(size_type index) const noexcept
    {
        assert(index < len);
        return vec[index];
    }
    matrix_inline T& operator[](size_type index) const noexcept
    {
        return at(index);
    }
    matrix_inline constexpr size_type size() const noexcept
    {
        return len;
    }
private:
    T* vec;
    size_type len;
};

template <typename T>
class matrix
{
public:
    using size_type = std::uint32_t;

    matrix_inline constexpr explicit matrix(size_type cols_, size_type rows_) noexcept
        : cols(cols_)
        , rows(rows_)
        , mat(new T[cols * rows])
    { }
    matrix_inline matrix(const matrix& rhs) noexcept
    {
        construct(rhs);
    }
    matrix_inline matrix(matrix&& rhs) noexcept
    {
        this->mat = rhs.mat;
        this->cols = rhs.cols;
        this->rows = rhs.rows;
        rhs.mat = nullptr;
        rhs.rows = 0;
        rhs.cols = 0;
    }
    matrix_inline virtual ~matrix()
    {
        if (mat)
        {
            delete[] mat;
            mat = nullptr;
        }
    }
    matrix_inline void fill(T value) noexcept
    {
        for (size_type i = 0; i < cols * rows; i++)
        {
            mat[i] = value;
        }
    }
    matrix_inline constexpr size_type width() const noexcept
    {
        return cols;
    }
    matrix_inline constexpr size_type height() const noexcept
    {
        return rows;
    }
    matrix_inline T* data() const noexcept
    {
      return mat;
    }
    matrix_inline constexpr T* at_pointer(size_type x, size_type y) const noexcept
    {
        return mat + (cols * y + x);
    }
    matrix_inline constexpr T& at(size_type x, size_type y) const
    {
        assert(y <= cols);
        assert(x <= rows);
        return mat[cols * y + x];
    }
    matrix_inline T& operator () (size_type x, size_type y) noexcept
    {
      return at(x, y);
    }
    matrix_inline const T& operator () (size_type x, size_type y) const noexcept
    {
      return at(x, y);
    }
    matrix_inline matrix& operator = (const matrix& rhs) noexcept
    {
        construct(rhs);
        return *this;
    }
    matrix_inline matrix& operator = (matrix&& rhs) noexcept
    {
        if (mat)
        {
            delete[] this->mat;
        }
        this->mat = rhs.mat;
        this->cols = rhs.cols;
        this->rows = rhs.rows;
        rhs.mat = nullptr;
        rhs.rows = 0;
        rhs.cols = 0;
        return *this;
    }
    matrix_inline matrix& operator += (const matrix& rhs) noexcept
    {
        assert(this->cols == rhs.cols);
        assert(this->rows == rhs.rows);

        for (size_type i = 0; i < cols * rows; i++)
        {
            this->mat[i] += rhs.mat[i];
        }
        return *this;
    }
    matrix_inline matrix& operator -= (const matrix& rhs) noexcept
    {
        assert(this->cols == rhs.cols);
        assert(this->rows == rhs.rows);

        for (size_type i = 0; i < cols * rows; i++)
        {
            this->mat[i] -= rhs.mat[i];
        }
        return *this;
    }
    matrix_inline matrix& operator *= (const matrix& rhs) noexcept
    {
        assert(this->cols == rhs.rows);

        this->rows = rhs.rows;
        T* new_data = new T[cols * rows];

        multiply(new_data, rhs);

        delete[] this->mat;
        this->mat = new_data;

        return *this;
    }
    matrix_inline matrix& operator *= (const T& val) noexcept
    {
        for (size_type i = 0; i < cols * rows; i++)
        {
            this->mat[i] *= val;
        }
        return *this;
    }
    matrix_inline matrix operator + (const matrix& rhs) const noexcept
    {
        matrix<T> result = *this;
        return result += rhs;
    }
    matrix_inline matrix operator - (const matrix& rhs) const noexcept
    {
        matrix<T> result = *this;
        return result -= rhs;
    }
    matrix_inline matrix operator * (const matrix& rhs) const noexcept
    {
        matrix<T> result = *this;
        return result *= rhs;
    }
    matrix_inline bool operator == (const matrix& other) const noexcept
    {
        if (this->rows != other.rows) return false;
        if (this->cols != other.cols) return false;
        return !memcmp(this->mat, other.mat, sizeof(T) * cols * rows);
    }
    matrix_inline bool operator != (const matrix& other) const noexcept
    {
        return !(*this == other);
    }
    template <typename F>
    friend matrix<F> transpose_impl(const matrix<F>& rhs) noexcept;
    matrix_inline void transpose() noexcept
    {
        *this = transpose_impl(*this);
    }

protected:
    matrix_inline void construct(const matrix& rhs)
    {
        if (mat != nullptr && (cols * rows) == (rhs.cols * rhs.rows))
        {
            std::memset(mat, 0, rows * cols);
        }
        else
        {
            if (mat)
            {
                delete[] mat;
            }
            mat = new T[rhs.cols * rhs.rows];
        }

        cols = rhs.cols;
        rows = rhs.rows;

        for (size_type i = 0; i < cols * rows; i++)
        {
            mat[i] = rhs.mat[i];
        }
    }
    matrix_inline constexpr T multiply_impl(const matrix& rhs, size_type i, size_type j) noexcept
    {
        T accumulator = { };
        for (size_type k = 0; k < rhs.cols; k++)
        {
            T lhs_val = (*this)(i, k);
            T rhs_val = rhs(k, j);
            accumulator += lhs_val * rhs_val;
        }
        return accumulator;
    }
    matrix_inline void multiply(T* new_data, const matrix& rhs) noexcept
    {
        #pragma omp parallel num_threads(matrix_num_threads)
        {
        #pragma omp for collapse(2)
        for (std::size_t i = 0; i < rows; i++)
        {
            for (std::size_t j = 0; j < rhs.rows; j++)
            {
                new_data[j * cols + i] = multiply_impl(rhs, i, j);
            }
        }
        } // pragma omp parallel
    }

private:
    size_type cols;
    size_type rows;
    T* mat = nullptr;
};

template <typename T>
matrix_inline std::ostream& operator << (std::ostream& ostream, const vector<T>& vec)
{
    ostream << "[ ";
    for (std::size_t i = 0; i < vec.size(); i++)
    {
        ostream << vec[i] << ' ';
    }
    ostream << ']';
    return ostream;
}
template <typename T>
std::ostream& operator << (std::ostream& ostream, const matrix<T>& mat)
{
    for (std::size_t cols = 0; cols < mat.width(); cols++)
    {
        for (std::size_t rows = 0; rows < mat.height(); rows++)
        {
            ostream << mat(cols, rows) << " ";
        }
        ostream << std::endl;
    }
    return ostream;
}
template <typename T>
matrix_inline vector<T> operator *= (matrix<T>& mat, const vector<T>& vec) noexcept
{
    assert(mat.height() == vec.size());

    vector<T> result(vec.size());
    for (std::size_t i = 0; i < mat.width(); i++)
    {
        T accumulator = {};
        for (std::size_t j = 0; j < mat.height(); j++)
        {
            accumulator += mat.at(i, j) * vec[i];
        }
        result.at(i) = accumulator;
    }
    return result;
}
template <typename T>
matrix_inline matrix<T> transpose_impl(const matrix<T>& rhs) noexcept
{
    std::size_t height = rhs.height();
    std::size_t width = rhs.width();
    matrix<T> transposed(height, width);

    T* src_ptr = rhs.mat;
    T* src_end_ptr = rhs.mat + (height * width);
    T* dst_ptr = transposed.mat;
    while (src_ptr < src_end_ptr)
    {
        *dst_ptr++ = *src_ptr++;
    }

    return transposed;
}
} // namespace haifisch

#undef matrix_inline
#undef matrix_num_threads
#endif // MATMUL_HPP
