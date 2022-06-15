#include <bits/stdc++.h>

struct array_info {
  array_info(int begin, int end, int sum)
    : begin_(begin), end_(end), sum_(sum) {}

  friend bool operator<(const array_info& lhs, const array_info& rhs) {
    return lhs.sum_ < rhs.sum_;
  }

  int begin_;
  int end_;
  int sum_;
};

/// Максимальный, пересекающий mid подмассив.
/// T(n) = Θ(n)
array_info max_crossing_subarray(const std::vector<int>& array, int low, int mid, int high) {
  int lsum = 0, rsum = 0, sum = 0;
  int lmax = 0, rmax = 0;
  /// Θ(n / 2)
  for (int i = mid; (i--) > low; ) {
    sum += array.at(i);
    if (i == mid - 1 || sum > lsum) {
      lsum = sum;
      lmax = i;
    }
  }
  sum = 0;
  /// Θ(n / 2)
  for (int i = mid; i < high; ++i) {
    sum += array.at(i);
    if (i == mid || sum > rsum) {
      rsum = sum;
      rmax = i + 1;
    }
  }
  return array_info(lmax, rmax, lsum + rsum);
}

/// Максимальный подмассив.
///
///      / Θ(1)                                          , n = 1
/// T(n)
///      \ Θ(1) + 2T(n/2) + Θ(n) + Θ(1) = 2T(n/2) + Θ(n) , n > 1
array_info max_subarray(const std::vector<int>& array, int low, int high) {
  /// Θ(1)
  if (low == high - 1)
    return array_info(low, high, array[low]);
  int mid = (low + high) / 2;
  /// T(n/2)
  auto _1 = max_subarray(array, low, mid);
  /// T(n/2)
  auto _2 = max_subarray(array, mid, high);
  /// Θ(n)
  auto _3 = max_crossing_subarray(array, low, mid, high);
  /// Θ(1)
  return std::max(std::max(_1, _2), _3);
}

array_info max_subarray(const std::vector<int>& array) {
  return max_subarray(array, 0, array.size());
}

int main() {
  auto i = max_subarray({1, 2, 3, 4, -5, -6, -7, 8, 9, 0});
  std::cout << i.begin_ << ", " << i.end_ << ", " << i.sum_ << std::endl;
}
