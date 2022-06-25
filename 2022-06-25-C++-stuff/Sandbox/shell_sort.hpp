// Jan 29, 2022
#include <iostream>
#include <vector>
#include <deque>
#include <cmath>
#include <ctime>
#include <random>
#include <iomanip>
#include <fstream>

template <typename Iterator>
void sort_assert(Iterator begin, Iterator end) {
  if (!std::is_sorted(begin, end)) {
    std::cout << "Array of size " << std::distance(begin, end)
              << " isn't sorted!";
    exit(-1);
  }
}

uint64_t shell_sort_shell(uint64_t *array, uint64_t size) {
  uint64_t temporary, j, total_swaps = 0;
  for (uint64_t gap = size / 2; gap > 0; gap /= 2) {
    for (uint64_t i = gap; i < size; ++i) {
      temporary = array[i];
      for (j = i; j >= gap; j -= gap) {
        if (temporary < array[j - gap]) {
          array[j] = array[j - gap];
          ++total_swaps;
        } else break;
      }
      ++total_swaps;
      array[j] = temporary;
    }
  }
  sort_assert(array, array + size);
  return total_swaps;
}

uint64_t shell_sort_hibbard(uint64_t *array, uint64_t size) {
  uint64_t temporary, j, total_swaps = 0;
  for (uint64_t gap = 1; gap < size; gap += (std::pow(2, gap) - 1)) {
    for (uint64_t i = gap; i < size; ++i) {
      temporary = array[i];
      for (j = i; j >= gap; j -= gap) {
        if (temporary < array[j - gap]) {
          array[j] = array[j - gap];
          ++total_swaps;
        } else break;
      }
      ++total_swaps;
      array[j] = temporary;
    }
  }
  sort_assert(array, array + size);
  return total_swaps;
}

uint64_t shell_sort_sedgewick(uint64_t *array, uint64_t size) {
  uint64_t temporary, j, total_swaps = 0;
  for (uint64_t gap = 1; gap < size; gap += (std::pow(4, gap) + 3 * std::pow(2, gap - 1) + 1)) {
    for (uint64_t i = gap; i < size; ++i) {
      temporary = array[i];
      for (j = i; j >= gap; j -= gap) {
        if (temporary < array[j - gap]) {
          array[j] = array[j - gap];
          ++total_swaps;
        } else break;
      }
      ++total_swaps;
      array[j] = temporary;
    }
  }
  sort_assert(array, array + size);
  return total_swaps;
}

uint64_t shell_sort_own_1(uint64_t *array, uint64_t size) {
  uint64_t temporary, j, total_swaps = 0;
  for (uint64_t gap = size; gap > 0; gap--) {
    for (uint64_t i = gap; i < size; ++i) {
      temporary = array[i];
      for (j = i; j >= gap; j -= gap) {
        if (temporary < array[j - gap]) {
          array[j] = array[j - gap];
          ++total_swaps;
        } else break;
      }
      ++total_swaps;
      array[j] = temporary;
    }
  }
  sort_assert(array, array + size);
  return total_swaps;
}

template <typename F>
void run_test(F &&sort_function, std::string_view label, std::vector<uint64_t> &&range) {
  uint64_t total_swaps = sort_function(range.data(), range.size());
  std::ofstream file(std::string(label) + ".txt", std::ios::app);
  file << range.size() << " " << total_swaps << std::endl;
}

std::vector<uint64_t> create_descending_order_range(uint64_t size) {
  std::vector<uint64_t> result(size);
  static uint64_t digit{size - 1};
  std::generate(result.begin(), result.end(), [&]{
    return digit--;
  });
  digit = size - 1;
  return result;
}

std::vector<uint64_t> create_random_order_range(uint64_t size) {
  std::random_device rd;
  std::mt19937 gen(rd());
  std::uniform_int_distribution<uint64_t> distrib(0, 0xFFFFFFFFFFFFFFFFULL);

  std::vector<uint64_t> result(size);
  for (uint64_t &i : result)
    i = distrib(gen);
  return result;
}

void shell_sort_tests() {
  for (uint64_t test_size = 1024; test_size < 40'000; test_size += 4096) {
    std::cout << "Test for size " << test_size << std::endl;
    run_test(shell_sort_shell, "shell_sort_shell_descending", create_descending_order_range(test_size));
    run_test(shell_sort_shell, "shell_sort_shell_random", create_random_order_range(test_size));

    run_test(shell_sort_hibbard, "shell_sort_hibbard_descending", create_descending_order_range(test_size));
    run_test(shell_sort_hibbard, "shell_sort_hibbard_random", create_random_order_range(test_size));

    run_test(shell_sort_sedgewick, "shell_sort_sedgewick_descending", create_descending_order_range(test_size));
    run_test(shell_sort_sedgewick, "shell_sort_sedgewick_random", create_random_order_range(test_size));

    run_test(shell_sort_own_1, "shell_sort_own_1_descending", create_descending_order_range(test_size));
    run_test(shell_sort_own_1, "shell_sort_own_1_random", create_random_order_range(test_size));
  }
}
