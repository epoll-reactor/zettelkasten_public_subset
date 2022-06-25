// Mar 9, 2022
#include <iostream>
#include <random>
#include <chrono>
#include <memory>
#include <cstring>

template <typename T>
class matrix;

template <typename T>
class submatrix {
public:
  explicit submatrix(const matrix<T>& source) noexcept;
  explicit submatrix(
    const matrix<T>& source,
    std::size_t left,
    std::size_t right,
    std::size_t up,
    std::size_t down
  ) noexcept;
  explicit submatrix(
    const submatrix<T>& sub_matrix,
    std::size_t left,
    std::size_t right,
    std::size_t up,
    std::size_t down
  ) noexcept;

  const T& operator()(std::size_t rows, std::size_t cols) const {
    return matrix_(up_ + rows, left_ + cols);
  }

  matrix<T> operator+(const submatrix<T>& rhs) {
    return arithmetic(rhs, std::plus<>{});
  }
  matrix<T> operator-(const submatrix<T>& rhs) {
    return arithmetic(rhs, std::minus<>{});
  }

  std::size_t rows() const noexcept {
    return down_ - up_;
  }
  std::size_t cols() const noexcept {
    return right_ - left_;
  }

private:
  template <typename Operator>
  matrix<T> arithmetic(const submatrix<T>& rhs, Operator binary_operator);

  const matrix<T>& matrix_;
  std::size_t left_, right_, up_, down_;
};

template <typename T>
std::ostream& operator<<(std::ostream& out_stream, const submatrix<T>& matrix) {
  for (size_t i = 0; i < matrix.rows(); ++i) {
    for (size_t j = 0; j < matrix.cols(); ++j) {
      out_stream << matrix(i, j) << ' ';
    }
    out_stream << std::endl;
  }
  return out_stream << std::endl;
}


template <typename T>
class matrix {
public:
  matrix(std::size_t rows, std::size_t cols)
    : rows_(rows), cols_(cols), matrix_(new T[rows * cols]) {}

  matrix(const matrix<T>& rhs)
    : rows_(rhs.rows()), cols_(rhs.cols()), matrix_(new T[rhs.rows() * rhs.cols()]) {
    for (std::size_t i = 0U; i < rows() * cols(); ++i) {
      matrix_[i] = rhs.matrix_[i];
    }
  }

  void randomize() {
    std::uniform_int_distribution<> distribution(1, 5);
    std::mt19937 engine;
    for (size_t i = 0; i < rows(); ++i) {
      for (size_t j = 0; j < cols(); ++j) {
        engine.seed(
          static_cast<double>(
            std::chrono::system_clock::now().time_since_epoch().count()
          )
        );
        (*this)(i, j) = distribution(engine);
      }
    }
  }

  T& operator()(std::size_t rows, std::size_t cols) {
    return matrix_[rows_ * rows + cols];
  }
  const T& operator()(std::size_t rows, std::size_t cols) const {
    return (const_cast<matrix<T>&>(*this))(rows, cols);
  }

  matrix<T> operator+(const matrix<T>& rhs) {
    return arithmetic(rhs, std::plus<>{});
  }
  matrix<T> operator-(const matrix<T>& rhs) {
    return arithmetic(rhs, std::minus<>{});
  }

  bool operator==(const matrix<T>& rhs) {
    if (rows() != rhs.rows() || cols() != rhs.cols()) {
      std::cout << "Size mismatch, " << rows() << "x" << cols() << " vs " << rhs.rows() << "x" << rhs.cols() << std::endl;
      return false;
    }
    for (size_t i = 0; i < rows(); ++i) {
      for (size_t j = 0; j < cols(); ++j) {
        if ((*this)(i, j) != rhs(i, j)) {
          std::cout << "At indices (" << i << ", " << j << ") " << (*this)(i, j) << " != " << rhs(i, j) << std::endl;
          return false;
        }
      }
    }
    return true;
  }

  bool operator!=(const matrix<T>& rhs) {
    return !(*this == rhs);
  }

  std::size_t rows() const { return rows_; }
  std::size_t cols() const { return cols_; }

#if 0
  matrix<T> linear(const matrix<T>& rhs) {
    return linear(submatrix{*this}, submatrix{rhs}, cols());
  }

  matrix<T> linear(const submatrix<T>& lhs, const submatrix<T>& rhs, std::size_t size) {
    matrix<T> result(size, size);
    T sum = T();
    for (size_t i = 0; i < size; ++i) {
      for (size_t j = 0; j < size; ++j) {
        for (size_t k = 0; k < size; ++k) {
          sum += lhs(i, k) * rhs(k, j);
        }
        result(i, j) = sum;
        sum = T();
      }
    }
    return result;
  }

  matrix<T> strassen(const matrix<T>& rhs) {
    return strassen(submatrix{*this}, submatrix{rhs}, cols());
  }

  matrix<T> strassen(const submatrix<T>& lhs, const submatrix<T>& rhs, std::size_t size) {
    if (size == 1)
      return linear(lhs, rhs, size);
    matrix<T> result(size, size);
    size_t k = size / 2;

    std::size_t a_mid = lhs.rows() / 2;
    std::size_t b_mid = lhs.cols() / 2;
    std::size_t c_mid = rhs.cols() / 2;
    std::size_t a_end = a_mid * 2;
    std::size_t b_end = b_mid * 2;
    std::size_t c_end = c_mid * 2;

    submatrix<T> a11(lhs,     0, a_mid,     0, a_mid);
    submatrix<T> a12(lhs, c_mid, c_end,     0, a_mid);
    submatrix<T> a21(lhs,     0, c_mid, a_mid, a_end);
    submatrix<T> a22(lhs, c_mid, c_end, a_mid, a_end);

    submatrix<T> b11(rhs,     0, b_mid,     0, c_mid);
    submatrix<T> b12(rhs, b_mid, b_end,     0, c_mid);
    submatrix<T> b21(rhs,     0, b_mid, c_mid, c_end);
    submatrix<T> b22(rhs, b_mid, b_end, c_mid, c_end);

    matrix<T> s1  = b12 - b22;
    matrix<T> s2  = a11 + a12;
    matrix<T> s3  = a21 + a22;
    matrix<T> s4  = b21 - b11;
    matrix<T> s5  = a11 + a22;
    matrix<T> s6  = b11 + b22;
    matrix<T> s7  = a12 - a22;
    matrix<T> s8  = b21 + b22;
    matrix<T> s9  = a11 - a21;
    matrix<T> s10 = b11 + b12;

    matrix<T> p1 = strassen(a11, submatrix{s1}, k);
    matrix<T> p2 = strassen(submatrix{s2}, b22, k);
    matrix<T> p3 = strassen(submatrix{s3}, b11, k);
    matrix<T> p4 = strassen(a22, submatrix{s4}, k);
    matrix<T> p5 = strassen(submatrix{s5}, submatrix{s6}, k);
    matrix<T> p6 = strassen(submatrix{s7}, submatrix{s8}, k);
    matrix<T> p7 = strassen(submatrix{s9}, submatrix{s10}, k);

    matrix<T> c11 = p5 + p4 - p2 + p6;
    matrix<T> c12 = p1 + p2;
    matrix<T> c21 = p3 + p4;
    matrix<T> c22 = p1 + p5 - p3 - p7;

    for (size_t i = 0; i < k; ++i) {
      for (size_t j = 0; j < k; ++j) {
        result(i, j) = c11(i, j);
        result(i, j + k) = c12(i, j);
        result(k + i, j) = c21(i, j);
        result(k + i, k + j) = c22(i, j);
      }
    }

    return result;
  }
#else
  matrix<T> linear(const matrix<T>& rhs) {
    return linear(*this, rhs, cols());
  }

  matrix<T> linear(const matrix<T>& lhs, const matrix<T>& rhs, std::size_t size) {
    matrix<T> result(size, size);
    T sum = T();

    for (size_t i = 0; i < size; ++i) {
      for (size_t j = 0; j < size; ++j) {
        for (size_t k = 0; k < size; ++k) {
          sum += lhs(i, k) * rhs(k, j);
        }
        result(i, j) = sum;
        sum = T();
      }
    }

    return result;
  }

  matrix<T> strassen(const matrix<T>& rhs) {
    return strassen(*this, rhs, cols());
  }

  matrix<T> strassen(const matrix<T>& lhs, const matrix<T>& rhs, std::size_t size) {
    if (size == 1)
      return linear(lhs, rhs, size);
    matrix<T> result(size, size);
    size_t k = size / 2;

    matrix<T> a11(k, k);
    matrix<T> a12(k, k);
    matrix<T> a21(k, k);
    matrix<T> a22(k, k);
    matrix<T> b11(k, k);
    matrix<T> b12(k, k);
    matrix<T> b21(k, k);
    matrix<T> b22(k, k);

    for (size_t i = 0; i < k; ++i) {
      for (size_t j = 0; j < k; ++j) {
        a11(i, j) = lhs(i, j);
        a12(i, j) = lhs(i, k + j);
        a21(i, j) = lhs(k + i, j);
        a22(i, j) = lhs(k + i, k + j);
        b11(i, j) = rhs(i, j);
        b12(i, j) = rhs(i, k + j);
        b21(i, j) = rhs(k + i, j);
        b22(i, j) = rhs(k + i, k + j);
      }
    }

    matrix<T> s1 = b12 - b22;
    matrix<T> s2 = a11 + a12;
    matrix<T> s3 = a21 + a22;
    matrix<T> s4 = b21 - b11;
    matrix<T> s5 = a11 + a22;
    matrix<T> s6 = b11 + b22;
    matrix<T> s7 = a12 - a22;
    matrix<T> s8 = b21 + b22;
    matrix<T> s9 = a11 - a21;
    matrix<T> s10 = b11 + b12;

    matrix<T> p1 = strassen(a11, s1, k);
    matrix<T> p2 = strassen(s2, b22, k);
    matrix<T> p3 = strassen(s3, b11, k);
    matrix<T> p4 = strassen(a22, s4, k);
    matrix<T> p5 = strassen(s5, s6, k);
    matrix<T> p6 = strassen(s7, s8, k);
    matrix<T> p7 = strassen(s9, s10, k);

    matrix<T> c11 = p5 + p4 - p2 + p6;
    matrix<T> c12 = p1 + p2;
    matrix<T> c21 = p3 + p4;
    matrix<T> c22 = p1 + p5 - p3 - p7;

    for (size_t i = 0; i < k; ++i) {
      for (size_t j = 0; j < k; ++j) {
        result(i, j) = c11(i, j);
        result(i, j + k) = c12(i, j);
        result(k + i, j) = c21(i, j);
        result(k + i, k + j) = c22(i, j);
      }
    }

    return result;
  }
#endif

private:
  template <typename Operator>
  matrix<T> arithmetic(const matrix<T>& rhs, Operator binary_operator) {
    matrix<T> result(rhs.rows(), rhs.cols());
    for (size_t i = 0; i < result.rows(); ++i) {
      for (size_t j = 0; j < result.cols(); ++j) {
        result(i, j) = binary_operator((*this)(i, j), rhs(i, j));
      }
    }
    return result;
  }

  std::size_t rows_, cols_;
  std::unique_ptr<T[]> matrix_;
};

template <typename T>
std::ostream& operator<<(std::ostream& out_stream, const matrix<T>& matrix) {
  for (size_t i = 0; i < matrix.rows(); ++i) {
    for (size_t j = 0; j < matrix.cols(); ++j) {
      out_stream << matrix(i, j) << ' ';
    }
    out_stream << std::endl;
  }
  return out_stream << std::endl;
}

template <typename T>
submatrix<T>::submatrix(const matrix<T>& source) noexcept
  : matrix_(source), left_(0U), right_(source.cols()), up_(0U), down_(source.rows()) {}

template <typename T>
submatrix<T>::submatrix(
  const matrix<T>& source,
  std::size_t left,
  std::size_t right,
  std::size_t up,
  std::size_t down
) noexcept
  : matrix_(source), left_(left), right_(right), up_(up), down_(down) {}

template <typename T>
submatrix<T>::submatrix(
  const submatrix<T>& sub_matrix,
  std::size_t left,
  std::size_t right,
  std::size_t up,
  std::size_t down
) noexcept
  : matrix_(sub_matrix.matrix_), left_(left), right_(right), up_(up), down_(down) {}

template <typename T>
template <typename Operator>
matrix<T> submatrix<T>::arithmetic(const submatrix<T>& rhs, Operator binary_operator) {
  matrix<T> result(rhs.rows(), rhs.cols());
  for (size_t i = 0; i < rhs.rows(); ++i) {
    for (size_t j = 0; j < rhs.cols(); ++j) {
      result(i, j) = binary_operator((*this)(i, j), rhs(i, j));
    }
  }
  return result;
}

#include "function_benchmarking.hpp"

void run_strassen_tests() {
//  {
//    matrix<int> m(5, 5);
//    m.randomize();
//
//    submatrix<int> sm(m, 1, 5, 1, 5);
//    std::cout << m << std::endl;
//    for (size_t i = 0; i < sm.rows(); ++i) {
//      for (size_t j = 0; j < sm.cols(); ++j) {
//        std::cout << sm(i, j) << ' ';
//      }
//      std::cout << std::endl;
//    }
//    std::cout << std::endl;
//
//    std::cout << sm + sm << std::endl;
//    std::cout << sm - sm << std::endl;
//
//    return;
//  }
  {
    std::chrono::system_clock::time_point begin, end;

    for (size_t size{1024}; size <= 1024; size *= 2) {
      matrix<int> a(size, size);
      matrix<int> b(size, size);
      a.randomize();
      b.randomize();

      begin = std::chrono::system_clock::now();
      auto linear = a.linear(b);
      end = std::chrono::system_clock::now();

      std::cout << "Linear " << size << "x" << size << ": ";
      std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms." << std::endl;

      begin = std::chrono::system_clock::now();
      auto strassen = a.strassen(b);
      end = std::chrono::system_clock::now();

      std::cout << "Strassen " << size << "x" << size << ": ";
      std::cout << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << " ms." << std::endl;

//      if (linear != strassen) {
//        std::cout << linear << std::endl;
//        std::cout << strassen << std::endl;
//        exit(-1);
//      }
    }
  }
}
