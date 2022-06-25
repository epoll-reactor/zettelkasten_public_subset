// Mar 2, 2022
#include <vector>
#include <iostream>
#include <cassert>


template <typename T>
struct matrix_row : public std::vector<T> {
  explicit matrix_row() noexcept : std::vector<T>() {}
  explicit matrix_row(const std::vector<T>& rhs) : std::vector<T>(rhs) {}
  explicit matrix_row(std::vector<T>&& rhs) : std::vector<T>(std::move(rhs)) {}
  explicit matrix_row(std::size_t size, T default_value) noexcept : std::vector<T>(size, default_value) {}
  explicit matrix_row(
    typename std::vector<T>::iterator begin,
    typename std::vector<T>::iterator end
  ) : std::vector<T>(begin, end) {}

  matrix_row<T> operator+(const matrix_row<T>& rhs) {
    assert(this->size() == rhs.size());
    matrix_row<T> result(this->size());
    for (std::size_t i{}; i < this->size(); ++i) {
      result[i] = (*this)[i] + rhs[i];
    }
    return result;
  }

  friend std::ostream& operator<<(std::ostream& out_stream, const matrix_row<T>& row) {
    for (std::size_t i{}; i < row.size(); ++i) {
      out_stream << row[i] << ' ';
    }
    return out_stream;
  }
  matrix_row<T> concat_h(matrix_row<T> rhs) {
    matrix_row<T> result(*this);
    result.insert(result.end(), rhs.begin(), rhs.end());
    return result;
  }
};

template <typename T>
class matrix {
public:
  explicit matrix(std::size_t rows, std::size_t cols, std::vector<T> input)
    : rows_(rows), cols_(cols) {
    data().reserve(rows);
    auto begin = input.begin();
    auto end = input.end();
    for (std::size_t i{}; i < rows; ++i) {
      end = begin + cols;
      data().emplace_back(begin, end);
      begin += cols;
    }
  }
  explicit matrix(std::size_t rows, std::size_t cols, T default_value)
    : rows_(rows), cols_(cols) {
    data().reserve(rows);
    for (std::size_t i{}; i < rows; ++i) {
      data().emplace_back(cols, default_value);
    }
  }
  explicit matrix(std::size_t rows, std::size_t cols)
    : rows_(rows), cols_(cols) {
    data().reserve(rows);
  }
  matrix(const matrix& rhs)
    : rows_(rhs.rows_), cols_(rhs.cols_), data_(rhs.data_) {}

  matrix_row<T>& operator[](std::size_t index) {
    return data()[index];
  }
  const matrix_row<T>& operator[](std::size_t index) const {
    return data()[index];
  }

  matrix<T> operator+(const matrix<T>& rhs) {
    return arithmetic(rhs, std::plus<>{});
  }
  matrix<T> operator-(const matrix<T>& rhs) {
    return arithmetic(rhs, std::minus<>{});
  }
  matrix<T> concat_h(const matrix<T>& rhs) {
    matrix<T> result(rows_, cols_ + rhs.cols_);
    for (std::size_t i{}; i < rows_; ++i) {
      result.data().push_back(data()[i].concat_h(rhs[i]));
    }
    return result;
  }
  matrix<T> concat_v(const matrix<T>& rhs) {
    matrix<T> result(*this);
    result.data().insert(
      result.data().end(),
      rhs.data().begin(),
      rhs.data().end()
    );
    result.rows_ += rhs.rows_;
    return result;
  }
  matrix<T> transpose() {
    matrix<T> result;
    for (std::size_t i{}; i < cols_; ++i) {
      for (std::size_t j{}; j < rows_; ++j) {
        result[i][j] = (*this)[j][i];
      }
    }
    return result;
  }
  void add_row(T default_value) {
    ++rows_;
    data().emplace_back(cols_, default_value);
  }
  void add_col(T default_value) {
    ++cols_;
    for (std::size_t i{}; i < rows_; ++i) {
      data()[i].push_back(default_value);
    }
  }

  std::ostream& octave(std::ostream& out_stream) {
    out_stream << '[';
    for (std::size_t i{}; i < rows_; ++i) {
      out_stream << data()[i];
      if (i != rows_ - 1) {
        out_stream << ';';
      }
    }
    return out_stream << ']' << std::endl;
  }
  friend std::ostream& operator<<(std::ostream& out_stream, const matrix<T>& rhs) {
    for (std::size_t i{}; i < rhs.rows_; ++i) {
      out_stream << rhs.data()[i] << std::endl;
    }
    return out_stream;
  }

  std::size_t rows() const noexcept {
    return rows_;
  }
  std::size_t cols() const noexcept {
    return cols_;
  }
  std::vector<matrix_row<T>>& data() noexcept {
    return data_;
  }
  const std::vector<matrix_row<T>>& data() const noexcept {
    return data_;
  }
private:
  template <typename Operator>
  matrix<T> arithmetic(const matrix<T>& rhs, Operator binary_operator) {
    matrix<T> result(rows_, cols_);
    assert(rows_ == rhs.rows_ && cols_ == rhs.cols_);
    for (std::size_t i{}; i < rows_; ++i) {
      matrix_row<T> row;
      row.reserve(result.cols_);
      for (std::size_t j{}; j < cols_; ++j) {
        row.push_back(binary_operator((*this)[i][j], rhs[i][j]));
      }
      result.data().push_back(std::move(row));
    }
    return result;
  }
  std::size_t rows_, cols_;
  std::vector<matrix_row<T>> data_;
};

template <typename T>
class submatrix {
public:
  explicit submatrix(const matrix<T>& source) noexcept
    : source_matrix_(source), left_(0U), right_(source.cols()), up_(0U), down_(source.rows()) {}
  explicit submatrix(
    const matrix<T>& source,
    std::size_t left,
    std::size_t right,
    std::size_t up,
    std::size_t down
  ) noexcept
    : source_matrix_(source), left_(left), right_(right), up_(up), down_(down) {}
  explicit submatrix(
    const submatrix<T>& sub_matrix,
    std::size_t left,
    std::size_t right,
    std::size_t up,
    std::size_t down
  ) noexcept
    : source_matrix_(sub_matrix.data()), left_(left), right_(right), up_(up), down_(down) {}

  const T& get_element(std::size_t index_1, std::size_t index_2) const {
    return data()[up_ + index_1][left_ + index_2];
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
  const matrix<T>& data() const noexcept {
    return source_matrix_;
  }
private:
  template <typename Operator>
  matrix<T> arithmetic(const submatrix<T>& rhs, Operator binary_operator) {
    matrix<T> result(rows(), cols());
    for (std::size_t i{}; i < result.rows(); ++i) {
      matrix_row<T> row;
      row.reserve(result.cols());
      for (std::size_t j{}; j < result.cols(); ++j) {
        row.push_back(binary_operator(get_element(i, j), rhs.get_element(i, j)));
      }
      result.data().push_back(std::move(row));
    }
    return result;
  }
  const matrix<T>& source_matrix_;
  std::size_t left_, right_, up_, down_;
};

/// T(n) = Θ(n^3)
template <typename T>
matrix<T> matrix_naive_multiply(const matrix<T>& lhs, const matrix<T>& rhs, T default_value) {
  matrix<T> result(lhs.rows(), rhs.cols(), default_value);
  for (std::size_t i{}; i < result.rows(); ++i) {
    for (std::size_t j{}; j < result.cols(); ++j) {
      T& element = result.data()[i][j];
      for (std::size_t k{}; k < lhs.cols(); ++k) {
        element += lhs[i][k] * rhs[k][j];
      }
    }
  }
  std::cout << result << std::endl;
  return result;
}

///        Θ(1)              , n = 1
///       /
/// T(n) =
///       \
///        7T(n/2) + Θ(n^2)  , n > 1
/// <=>
/// Θ(n^lg7) ~= Θ(n^2.8)
template <typename T, T default_value>
matrix<T> matrix_strassen_multiply(const submatrix<T>& lhs, const submatrix<T>& rhs) {
  std::size_t lhs_row = lhs.rows();
  std::size_t lhs_col = lhs.cols();
  std::size_t rhs_col = rhs.cols();
  switch (lhs_row * lhs_col * rhs_col) {
    case 0:
      return matrix<T>(0, 0);
    case 1:
      return matrix<T>(1, 1, lhs.get_element(0, 0) * rhs.get_element(0, 0));
    default:
      std::size_t a_mid = lhs_row / 2;
      std::size_t b_mid = rhs_col / 2;
      std::size_t c_mid = lhs_col / 2;
      std::size_t a_end = a_mid   * 2;
      std::size_t b_end = b_mid   * 2;
      std::size_t c_end = c_mid   * 2;
      submatrix<T> a11(lhs,     0, c_mid,     0, a_mid);
      submatrix<T> a12(lhs, c_mid, c_end,     0, a_mid);
      submatrix<T> a21(lhs,     0, c_mid, a_mid, a_end);
      submatrix<T> a22(lhs, c_mid, c_end, a_mid, a_end);
      submatrix<T> b11(rhs,     0, b_mid,     0, c_mid);
      submatrix<T> b12(rhs, b_mid, b_end,     0, c_mid);
      submatrix<T> b21(rhs,     0, b_mid, c_mid, c_end);
      submatrix<T> b22(rhs, b_mid, b_end, c_mid, c_end);
      matrix<T> sum1  = b12 - b22;
      matrix<T> sum2  = a11 + a22;
      matrix<T> sum3  = a21 + a22;
      matrix<T> sum4  = b21 - b11;
      matrix<T> sum5  = a11 + a22;
      matrix<T> sum6  = b11 + b22;
      matrix<T> sum7  = a12 - a22;
      matrix<T> sum8  = b21 + b22;
      matrix<T> sum9  = a11 - a21;
      matrix<T> sum10 = b11 + b12;
      matrix<T> p1 = matrix_strassen_multiply<T, default_value>( a11, sum1 );
      matrix<T> p2 = matrix_strassen_multiply<T, default_value>(sum2,  b22 );
      matrix<T> p3 = matrix_strassen_multiply<T, default_value>(sum3,  b11 );
      matrix<T> p4 = matrix_strassen_multiply<T, default_value>( a22, sum4 );
      matrix<T> p5 = matrix_strassen_multiply<T, default_value>(sum5, sum6 );
      matrix<T> p6 = matrix_strassen_multiply<T, default_value>(sum7, sum8 );
      matrix<T> p7 = matrix_strassen_multiply<T, default_value>(sum9, sum10);
      matrix<T> c11 = p5 + p4 - p2 + p6;
      matrix<T> c12 = p1 + p2;
      matrix<T> c21 = p3 + p4;
      matrix<T> c22 = p5 + p1 - p3 - p7;
      matrix<T> c = (c11.concat_h(c12)).concat_v(c21.concat_h(c22));
      if (a_end != lhs_row) {
        c.add_row(default_value);
        for (std::size_t i{}; i < b_end; ++i) {
          for (std::size_t j{}; j < c_end; ++j) {
            c[a_end][i] += lhs.get_element(a_end, j) * rhs.get_element(j, i);
          }
        }
        ++a_end;
      }
      if (b_end != rhs_col) {
        c.add_col(default_value);
        for (std::size_t i{}; i < a_end; ++i) {
          for (std::size_t j{}; j < c_end; ++j) {
            c[i][b_end] += lhs.get_element(i, j) * rhs.get_element(j, b_end);
          }
        }
        ++b_end;
      }
      if (c_end != lhs_col) {
        for (std::size_t i{}; i < a_end; ++i) {
          for (std::size_t j{}; j < c_end; ++j) {
            c[i][j] += lhs.get_element(i, c_end) * rhs.get_element(c_end, j);
          }
        }
      }
      return c;
  }
}

template <typename T, T default_value = T()>
matrix<T> matrix_strassen_multiply(const submatrix<T>& lhs, const matrix<T>& rhs) {
  return matrix_strassen_multiply<T, default_value>(lhs, submatrix<T>(rhs));
}
template <typename T, T default_value = T()>
matrix<T> matrix_strassen_multiply(matrix<T> lhs, const submatrix<T>& rhs) {
  return matrix_strassen_multiply<T, default_value>(submatrix<T>(lhs), rhs);
}
template <typename T, T default_value = T()>
matrix<T> matrix_strassen_multiply(matrix<T> lhs, matrix<T> rhs) {
  return matrix_strassen_multiply<T, default_value>(submatrix<T>(lhs), submatrix<T>(rhs));
}

void run_strassen_old_tests() {
  matrix<int> m1(2, 2, 10);
  matrix<int> m2(2, 2, 20);
  std::cout << matrix_strassen_multiply(m1, m2) << std::endl;
}
