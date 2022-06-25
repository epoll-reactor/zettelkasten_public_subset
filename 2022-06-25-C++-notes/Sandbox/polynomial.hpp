// Jan 30, 2021
#include <iostream>
#include <vector>
#include <cassert>

template <typename T>
class Polynomial {
private:
  mutable std::vector<T> Range;

public:
  explicit Polynomial() {
    Range.push_back(0);
  }

  Polynomial(const Polynomial<T>& p) {
    Range.resize(p.Range.size());
    Range = p.Range;
  }

  explicit Polynomial(const std::vector<T>& input) {
    Range.resize(input.size());
    Range = input;
  }

  explicit Polynomial(const T& input) {
    Range.resize(1);
    Range[0] = input;
  }

  template <typename iter>
  Polynomial(iter first, iter second) {
    Range.resize(0);
    while (first != second) {
      Range.push_back(*first);
      first++;
    }
  }

  size_t Degree() const {
    return Range.size() - 2;
  }

  T const operator()(const T& x) const {
    T res = 0;
    for (size_t i = 0; i < Range.size(); i++) {
      res += T(T(Range[i]) * MyPow(x, T(i)));
    }
    return res;
  }

  friend std::ostream& operator<<(std::ostream& out, const Polynomial<T>& p) {
    if (p[p.Range.size() - 1] > T(0) || p[p.Range.size() - 1] < T(0)) {
      if (p[p.Range.size() - 1] == T(-1)) {
        out << "-";
      } else {
        if (p[p.Range.size() - 1] != T(1)) {
          out << p[p.Range.size() - 1];
        }
      }
      if (p.Range.size() - 1 > 0) {
        out << "x^" << p.Range.size() - 1;
      }
    }
    if (p.Range.size() - 1 > 0) {
      for (size_t i = p.Range.size() - 2; i > 0; --i) {
        if (p[i] != T(0)) {
          if (p[i] > T(0)) {
            out << "+";
          }
          if (p[i] > T(1) || p[i] < T(-1)) {
            out << p[i];
          }
          out << "x";
          if (i > 1) {
            out << "^" << i;
          }
        }
      }
      if (p[0] > T(0)) {
        out << "+";
        out << p[0];
      } else {
        if (p[0] < T(0)) {
          out << p[0];
        }
      }
    }
    return out;
  }

  Polynomial const operator+(const Polynomial& right) const {
    Polynomial result(this->Range);
    if (Range.size() >= right.Range.size()) {
      for (size_t i = 0; i < right.Range.size(); i++) {
        result[i] += right[i];
      }
    } else {
      result.Range.resize(right.Range.size());
      result.Range = right.Range;
      for (size_t i = 0; i < Range.size(); i++) {
        result[i] += right[i];
      }
    }
    return result;
  }

  Polynomial const operator/(const T& scalar) const {
    Polynomial result(this->Range);
    for (size_t i = 0; i < result.Range.size(); ++i) {
      result[i] /= scalar;
    }
    return result;
  }

  Polynomial const operator/=(const T& scalar) {
    *this = *this / scalar;
    return *this;
  }

  Polynomial const operator/(const Polynomial& right) const {
    Polynomial result(this->Range);
    Polynomial temp(this->Range);
    size_t resd = Range.size() - right.Range.size() + 1;
    size_t rrs = right.Range.size();
    result.Range.resize(resd);
    for (size_t i = 0; i < result.Range.size(); ++i) {
      result[resd - i - 1] = temp[temp.Range.size() - i - 1] / right[rrs - 1];
      for (size_t j = 0; j < rrs; j++) {
        temp[temp.Range.size() - j - i - 1] -= right[rrs - j - 1] * result[resd - i - 1];
      }
    }
    return result;
  }

  Polynomial const operator/=(const Polynomial& right) {
    *this = *this / right;
    return *this;
  }

  Polynomial const operator-(const Polynomial& right) const {
    Polynomial result(this->Range);
    if (Range.size() >= right.Range.size()) {
      for (size_t i = 0; i < right.Range.size(); i++) {
        result[i] -= right[i];
      }
    } else {
      result.Range.resize(right.Range.size());
      result.Range = right.Range;
      for (size_t i = 0; i < Range.size(); i++) {
        result[i] -= right[i];
      }
    }
    return result;
  }

  Polynomial const operator+(const T& scalar) const {
    Polynomial result(this->Range);
    result[result.Range.size() - 1] += scalar;
    return result;
  }

  Polynomial const operator-(const T& scalar) const {
    Polynomial result(this->Range);
    result[result.Range.size() - 1] -= scalar;
    return result;
  }

  Polynomial const operator+=(const Polynomial& right) {
    *this = *this + right;
    return *this;
  }

  Polynomial const operator-=(const Polynomial& right) {
    *this = *this - right;
    return *this;
  }

  Polynomial const operator+=(const T& scalar) {
    *this = *this + Polynomial(scalar);
    return *this;
  }

  Polynomial const operator-=(const T& scalar) {
    *this = *this - Polynomial(scalar);
    return *this;
  }

  Polynomial const operator*(const T& scalar) const {
    Polynomial result(this->Range);
    result.Range[result.Range.size() - 1] *= scalar;
    return result;
  }

  Polynomial const operator*=(const T& scalar) {
    *this = *this * scalar;
    return *this;
  }

  Polynomial const operator*(const Polynomial& other) const {
    Polynomial result(this->Range);
    result.Range.resize(Range.size() + other.Range.size() + 1);
    for (size_t i = 0; i < Range.size(); ++i) {
      for (size_t j = 0; j < other.Range.size(); ++j) {
        result[i + j] += Range[i] * other[j];
      }
    }
    return result;
  }

  Polynomial const operator*=(const Polynomial& other) {
    *this = *this * other;
    return *this;
  }

  Polynomial<T> const operator,(const T& k) const {
    int i;
    Polynomial tmp(Range);
    switch (k) {
      case 0:break;
      case 1:
        for (i = 0; i < tmp.Range.size(); i++) {
          tmp[i] = tmp[i + 1] * (i + 1);
        }
        tmp[tmp.Range.size()] = 0;
        break;
      default:tmp = (tmp, 1, k - 1);
    }
    return tmp;
  }

  const T& operator[](const size_t& i) const {
    return Range[i];
  }

  T& operator[](const size_t& i) {
    return Range[i];
  }

  typename std::vector<T>::iterator begin() {
    return Range.begin();
  }

  typename std::vector<T>::iterator end() {
    return Range.end();
  }

  typename std::vector<T>::const_iterator begin() const {
    return Range.begin();
  }

  typename std::vector<T>::const_iterator end() const {
    return Range.end();
  }

  friend bool operator==(const Polynomial<T>& pL, const Polynomial<T>& pR) {
    if (pL.Range.size() != pR.Range.size()) {
      std::cout << "Range size error: " << pL.Range.size() << ", " << pR.Range.size() << "\n";
      return false;
    }

    for (size_t i = 0; i < pL.Range.size(); i++) {
      if (pL[i] != pR[i]) {
        std::cout << pL[i] << " != " << pR[i] << std::endl;
        return false;
      }
    }
    return true;
  }

  friend bool operator==(const Polynomial<T>& pL, const T& scalar) {
    return (pL == Polynomial(scalar));
  }

  friend bool operator!=(const Polynomial<T>& pL, const Polynomial<T>& pR) {
    return !(pL == pR);
  }

  friend bool operator!=(const Polynomial<T>& pL, const T& scalar) {
    return !(pL == Polynomial(scalar));
  }

  T MyPow(const T& a, const T& n) const {
    if (n == T(0)) {
      return T(1);
    } else {
      return a * MyPow(a, n - T(1));
    }
  }
};

void run_polynomial_tests() {
  std::vector<int> range_1 = {1, 2};
  Polynomial<int> poly_1(range_1.begin(), range_1.end());

  std::vector<int> range_2 = {3, 4};
  Polynomial<int> poly_2(range_2.begin(), range_2.end());

  std::cout << "Assert" << Polynomial<int>({4, 12, 8}) << std::endl;
  std::cout << poly_1 << " x " << poly_2 << " = " << poly_1 * poly_2 << std::endl;
  assert((poly_1 * poly_2) == Polynomial<int>({4, 12, 8}));
}
