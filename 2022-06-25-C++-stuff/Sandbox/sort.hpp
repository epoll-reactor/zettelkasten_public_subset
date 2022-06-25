// Nov 29, 2021
#ifndef SORT_HPP
#define SORT_HPP

#include <iterator>

/*! Complexity - O(n^2) in worst case.
 *
 *    [7, 3, 2, 5, 1] 7 > 3 -> swap
 *     ^  ^
 * 1: [3, 7, 2, 5, 1] 7 > 2 -> swap
 *        ^  ^
 * 2: [3, 2, 7, 5, 1] 7 > 5 -> swap
 *           ^  ^
 * 3: [3, 2, 5, 7, 1] 7 > 1 -> swap
 *              ^  ^
 * 4: [3, 2, 5, 1, 7] 3 > 2 -> swap
 *     ^  ^
 * 5: [2, 3, 5, 1, 7] 5 > 1 -> swap
 *           ^  ^
 * 6: [2, 3, 1, 5, 7] 3 > 1 -> swap, 5 and 7 are final sorted
 *        ^  ^
 * 7: [2, 1, 3, 5, 7] 2 > 1 -> swap, 3, 5 and 7 are final sorted
 *     ^  ^
 * 8: [1, 2, 3, 5, 7] 8 operations
 */
template <typename Iterator>
void bubble_sort(Iterator begin, Iterator end) {
  for (Iterator it_i = begin; it_i != end; ++it_i) {
    for (Iterator it_j = begin; it_j < it_i; ++it_j) {
      if (*it_i < *it_j) {
        std::iter_swap(it_i, it_j);
      }
    }
  }
}

/*! Complexity - O(n^2) in worst case, O(n) in best case.
 *
 *    [7, 3, 2, 5, 1] 7 > 3 -> swap
 *     ^  ^
 * 1: [3, 7, 2, 5, 1] 7 > 2 -> swap
 *        ^  ^
 * 2: [3, 2, 7, 5, 1] swap 3 2
 *     ^  ^
 * 3: [2, 3, 7, 5, 1] 7 > 5 -> swap
 *           ^  ^
 * 4: [2, 3, 5, 7, 1] 7 > 1 -> swap
 *              ^  ^
 * 5: [2, 3, 5, 1, 7] move 1 left until left element > 1
 *              ^
 * 6: [2, 3, 1, 5, 7]
 *           ^
 * 7: [2, 1, 3, 5, 7]
 *        ^
 * 8: [1, 2, 3, 5, 7] 8 operations
 */
template <typename Iterator>
void insertion_sort(Iterator begin, Iterator end) {
  std::iter_swap(begin, std::min_element(begin, end));
  for (Iterator it_i = begin; ++it_i < end; begin = it_i) {
    for (Iterator it_j = it_i; *it_j < *begin; --it_j, --begin) {
      std::iter_swap(begin, it_j);
    }
  }
}
/*! Complexity - O(n^2) in worst case.
 *
 *     [7, 3, 2, 5, 1] go through array, 7 > 1 -> swap, 4 operations + 1 swap
 *      ^           ^
 *  5: [1, 3, 2, 5, 7] go through array, 1 < 5 -> do nothing, 3 operations
 *
 *  8: [1, 3, 2, 5, 7] go through array, 3 > 2 -> swap, 2 operations + 1 swap
 *         ^  ^
 * 11: [1, 2, 3, 5, 7] go through array, check if all left > right, 2 operations
 *
 * 13: [1, 2, 3, 5, 7]
 */
template <typename Iterator>
void selection_sort(Iterator begin, Iterator end) {
  for (Iterator i = begin; i != end; ++i) {
    std::iter_swap(i, std::min_element(i, end));
  }
}
/*! Complexity - O(n^2) in worst case, O(nlogn) in average case, O(n) in best case.
 *
 *     [7, 3, 2, 5, 1] ...
 */
template <typename Iterator>
void quick_sort(Iterator begin, Iterator end) {
  if (end <= begin) {
    return;
  }
  Iterator pivot = begin;
  Iterator middle = begin + 1;
  for (Iterator i = begin + 1; i < end; ++i) {
    if (*i < *pivot) {
      std::iter_swap(i, middle);
      ++middle;
    }
  }
  std::iter_swap(begin, middle - 1);
  quick_sort(begin, middle - 1);
  quick_sort(middle, end);
}

namespace stupid_univer_quick_sort {

int partition(int* array, int low, int high) {
  int pivot = array[high];  // taking the last element as pivot
  int i = (low - 1);        // Index of smaller element

  for (int j = low; j < high; ++j) {
    if (array[j] <= pivot) {
      i++;
      std::swap(array[i], array[j]);
    }
  }
  std::swap(array[i + 1], array[high]);
  return i + 1;
}

void quick_sort(int* array, int low, int high) {
  if (low >= high) return;
  int part = partition(array, low, high);
  quick_sort(array, low, part - 1);
  quick_sort(array, part + 1, high);
}

void show(int* array, int size) {
  for (int i = 0; i < size; i++)
    std::cout << array[i] << " ";
  std::cout << "\n";
}

void test() {
  int array[] = { 10, 2, 9, 4, 5 };
  int size = sizeof(array) / sizeof(array[0]);

  quick_sort(array, 0, size - 1);
  show(array, size);
}

}

#endif //SORT_HPP
