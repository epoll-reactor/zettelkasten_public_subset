#include <cstddef>
#include <stdexcept>
#include <list>


namespace epoll_reactor {

template <typename T>
struct list_node final
{
  typedef T value_type;

  list_node(value_type value, list_node *prev, list_node *next) noexcept
    : value(value), prev(prev), next(next) {}

  value_type value;
  list_node *prev;
  list_node *next;
};

template <typename T, typename Allocator = std::allocator<list_node<T>>,
                      typename AllocatorTraits = std::allocator_traits<Allocator>>
class list;

template <typename T, typename Pointer, typename Reference>
struct list_iterator final : std::iterator<
  std::bidirectional_iterator_tag,
  T,
  std::ptrdiff_t,
  Pointer,
  Reference
> {
private:
  friend class list<T>;

  list_node<T> *node_;

public:
  list_iterator() noexcept
    : node_(nullptr) {}

  list_iterator(list_node<T> *node) noexcept
    : node_(node) {}

  list_iterator(const list_iterator &rhs) noexcept
    : node_(rhs.node_) {}

  list_iterator &operator=(const list_iterator &rhs) noexcept
    {
      node_ = rhs.node_;
      return *this;
    }

  list_iterator &operator++() noexcept
    {
      node_ = node_->next;
      return *this;
    }

  list_iterator &operator--() noexcept
    {
      node_ = node_->prev;
      return *this;
    }

  list_iterator operator++(int) noexcept
    {
      list_iterator temporary { *this };
      ++*this;
      return temporary;
    }

  list_iterator operator--(int) noexcept
    {
      list_iterator temporary { *this };
      --*this;
      return temporary;
    }

  list_iterator &operator+(size_t offset) noexcept
    {
      while (node_ && offset-- > 0U)
        {
          node_ = node_->next;
        }
      return *this;
    }

  list_iterator &operator-(size_t offset) noexcept
    {
      while (node_ && offset-- > 0U)
        {
          node_ = node_->prev;
        }
      return *this;
    }

  Reference operator*() noexcept
    {
      return node_->value;
    }

  Pointer operator->() noexcept
    {
      return &(node_->value);
    }

  bool operator==(const list_iterator &rhs) const noexcept
    {
      return node_ == rhs.node_;
    }

  bool operator!=(const list_iterator &rhs) const noexcept
    {
      return node_ != rhs.node_;
    }
};

template <typename T, typename Allocator,
                      typename AllocatorTraits>
class list final
{
public:
  typedef T                               value_type;
  typedef T *                             pointer;
  typedef const T *                       const_pointer;
  typedef const T &                       reference;
  typedef T &                             const_reference;
  typedef Allocator                       allocator_type;
  typedef AllocatorTraits                 allocator_traits;
  typedef size_t                          size_type;
  typedef list_iterator<value_type, value_type *, value_type &>
                                          iterator;
  typedef list_iterator<value_type, const value_type *, const value_type &>
                                          const_iterator;

private:
  allocator_type         allocator_;
  list_node<value_type> *head_;
  list_node<value_type> *tail_;
  size_t                 length_;

  template <typename... Args>
  [[nodiscard]] list_node<value_type> *create_node(Args &&...args)
    {
      list_node<value_type> *allocated = allocator_traits::allocate(allocator_, 1);
      allocator_traits::construct(allocator_, allocated, std::forward<Args>(args)...);
      return allocated;
    }

  template <typename InputIterator>
  [[nodiscard]] bool compare(InputIterator begin, InputIterator end) const noexcept
    {
      auto it1 { this->begin() };
      auto it2 { begin };

      while (it1 != this->end() && it2 != end)
        {
          if (*it1 == *it2)
            {
              ++it1;
              ++it2;
            }
          else
            {
              return false;
            }
        }
      return (it1 == this->end() && it2 == end);
    }

  template <typename _T, typename RHS>
  friend bool operator==(const list<_T> &lhs, const RHS &rhs) noexcept;

  template <typename _T, typename RHS>
  friend bool operator!=(const list<_T> &lhs, const RHS &rhs) noexcept;

public:
  list(std::initializer_list<value_type> list) noexcept
    : allocator_(), head_(nullptr), tail_(nullptr), length_(0U)
    {
      *this = list;
    }

  list() noexcept
    : allocator_(), head_(nullptr), tail_(nullptr), length_(0U) {}

  list(const list &rhs)
    : allocator_(), head_(nullptr), tail_(nullptr), length_(0U)
    {
      assign(rhs.begin(), rhs.end());
    }

  list(list &&rhs) noexcept
    : allocator_(std::move(rhs.allocator_)), head_(rhs.head_)
    , tail_(rhs.tail_), length_(rhs.length_)
    {
      rhs.head_ = nullptr;
      rhs.tail_ = nullptr;
      rhs.length_ = 0U;
    }

  list &operator=(std::initializer_list<value_type> list)
    {
      assign(list.begin(), list.end());
      return *this;
    }

  list &operator=(const list &rhs)
    {
      if (this == std::addressof(rhs))
        {
          return *this;
        }
      assign(rhs.begin(), rhs.end());
      return *this;
    }

  list &operator=(list &&rhs)
    {
      if (this == std::addressof(rhs))
        {
          return *this;
        }
      clear();
      allocator_ = std::move(rhs.allocator_);
      head_ = rhs.head_;
      tail_ = rhs.tail_;
      length_ = rhs.length_;
      rhs.head_ = nullptr;
      rhs.tail_ = nullptr;
      rhs.length_ = 0U;
      return *this;
    }

  ~list()
    {
      clear();
    }

  [[nodiscard]] size_t size() const noexcept
    {
      return length_;
    }

  [[nodiscard]] bool empty() const noexcept
    {
      return length_ == 0U;
    }

  [[nodiscard]] iterator begin() noexcept
    {
      return iterator(head_);
    }

  [[nodiscard]] const_iterator begin() const noexcept
    {
      return const_iterator(head_);
    }

  [[nodiscard]] iterator end() noexcept
    {
      return iterator(tail_ ? tail_->next : head_);
    }

  [[nodiscard]] const_iterator end() const noexcept
    {
      return const_iterator(tail_ ? tail_->next : head_);
    }

  [[nodiscard]] value_type &front() noexcept
    {
      return *begin();
    }

  [[nodiscard]] const value_type &front() const noexcept
    {
      return *begin();
    }

  [[nodiscard]] value_type &back() noexcept
    {
      size_type offset { size() - 1U };
      return *(begin() + offset);
    }

  [[nodiscard]] const value_type &back() const noexcept
    {
      size_type offset { size() - 1U };
      return *(begin() + offset);
    }

  template <typename _T>
  void push_back(_T &&value)
    {
      if (length_ == 0U)
        {
          head_ = create_node(std::forward<_T>(value), nullptr, nullptr);
          tail_ = head_;
        }
      else
        {
          tail_->next = create_node(std::forward<_T>(value), tail_, nullptr);
          tail_ = tail_->next;
        }
      ++length_;
    }

  template <typename _T>
  void push_front(_T &&value)
    {
      if (length_ == 0U)
        {
          tail_ = create_node(std::forward<_T>(value), nullptr, nullptr);
          head_ = tail_;
        }
      else
        {
          head_->prev = create_node(std::forward<_T>(value), nullptr, head_);
          head_ = head_->prev;
        }
      ++length_;
    }

  void pop_back()
    {
      list_node<value_type> *old_ptr { tail_ };
      tail_ = tail_->prev;
      if (length_ == 1)
        {
          head_ = nullptr;
        }
      else
        {
          tail_->next = nullptr;
        }
      --length_;
      allocator_traits::deallocate(allocator_, old_ptr, 1);
    }

  void pop_front()
    {
      list_node<value_type> *old_ptr { head_ };
      head_ = head_->next;
      if (length_ == 1)
        {
          tail_ = nullptr;
        }
      else
        {
          head_->prev = nullptr;
        }
      --length_;
      allocator_traits::deallocate(allocator_, old_ptr, 1);
    }

  void clear()
    {
      while (length_ > 0U)
        {
          pop_back();
        }
    }

  template <typename InputIterator>
  void assign(InputIterator begin, InputIterator end)
    {
      if (begin == end)
        {
          return;
        }
      list_node<value_type> *head_watcher { head_ };

      while (head_watcher && begin != end)
        {
          head_watcher->value = *begin;
          head_watcher = head_watcher->next;
          ++begin;
        }

      if (begin != end)
        {
          do
            {
              push_back(*begin);
            }
          while (++begin != end);
        }
      else
        {
          while (head_watcher)
            {
              list_node<value_type> *to_erase { head_watcher };
              head_watcher = head_watcher->next;
              erase(iterator(to_erase));
            }
        }
    }

  void erase(iterator position)
    {
      if (!position.node_->prev)
        {
          pop_front();
        }
      else if (!position.node_->next)
        {
          pop_back();
        }
      else
        {
          position.node_->prev->next = position.node_->next;
          position.node_->next->prev = position.node_->prev;
          allocator_traits::deallocate(allocator_, position.node_, 1);
          --length_;
        }
    }

  template <typename _T>
  void insert(iterator position, _T &&value)
    {
      list_node<value_type> *insertion_point { position.node_ };
      if (position == end())
        {
          push_back(std::forward<_T>(value));
          return;
        }
      if (!insertion_point)
        {
          push_front(std::forward<_T>(value));
          return;
        }
      list_node<value_type> *allocated { create_node(std::forward<_T>(value), insertion_point, insertion_point->next) };
      if (insertion_point->next)
        {
          insertion_point->next->prev = allocated;
        }
      else
        {
          tail_ = allocated;
        }
      insertion_point->next = allocated;
      ++length_;
    }

  void sort() noexcept
    {
      if (empty())
        {
          return;
        }
      list_node<value_type> *head_watcher { head_ };
      while (head_watcher)
        {
          for (list_node<value_type> *runner { head_watcher->next }; runner; runner = runner->next)
            {
              if (head_watcher->value > runner->value)
                {
                  std::swap(head_watcher->value, runner->value);
                }
            }
          head_watcher = head_watcher->next;
        }
    }
};

template <typename T, typename RHS>
bool operator==(const list<T> &lhs, const RHS &rhs) noexcept
  {
    return lhs.compare(rhs.begin(), rhs.end());
  }

template <typename T, typename RHS>
bool operator!=(const list<T> &lhs, const RHS &rhs) noexcept
  {
    return !(lhs == rhs);
  }

} // namespace epoll-reactor

#include <cassert>

//#define USE_STD

#ifdef USE_STD
  template <typename T>
  using test_list = std::list<T>;
#else
template <typename T>
  using test_list = epoll_reactor::list<T>;
#endif

void list_range_test()
  {
    {
      test_list<int> empty;
      assert(empty.begin() == empty.end());
    }
    test_list<int> my_list { 1, 2, 3, 4, 5 };
    std::list<int> std_list { 1, 2, 3, 4, 5 };
    std::list<int> my_output, std_output;
    test_list<int>::iterator my_list_begin;
    std::list<int>::iterator std_list_begin;
    for (int i : my_list)
      {
        my_output.push_back(i);
      }
    for (int i : std_list)
      {
        std_output.push_back(i);
      }
    assert(my_output == std_output);
  }

void list_equality_test()
  {
    struct comparable
      {
        int value;

        comparable(int value_)
          : value(value_) {}

        bool operator==(const comparable &rhs) const
          {
            return value == rhs.value;
          }

        bool operator!=(const comparable &rhs) const
          {
            return value != rhs.value;
          }
      };
    epoll_reactor::list l1 { comparable{1}, comparable{2} };
    epoll_reactor::list l2 { comparable{1}, comparable{2} };
    assert(  l1 == l2);
    assert(!(l1 != l2));
  }

void list_assign_function_test()
  {
    {
      test_list<int> l1, l2;
      l1.assign(l2.begin(), l2.end());
      assert(l1 == l2);
    } {
      test_list<int> l1 { 1 };
      test_list<int> l2 { 2 };
      l1.assign(l2.begin(), l2.end());
      assert(l1 == l2);
    } {
      test_list<int> l { 1, 2, 3 };
      test_list<int> to_assign { 4, 5, 6, 7, 8, 9 };
      assert(l.size() == 3);
      l.assign(to_assign.begin(), to_assign.end());
      assert(l == test_list<int>({ 4, 5, 6, 7, 8, 9 }));
    } {
      test_list<int> l { 4, 5, 6, 7, 8, 9, 10, 11 };
      test_list<int> to_assign { 1, 2, 3 };
      l.assign(to_assign.begin(), to_assign.end());
      assert(l == test_list<int>({ 1, 2, 3 }));
    }
  }

void list_move_assign_test()
  {
    test_list<int> l { 1, 2, 3 };
    test_list<int> to_assign { 4, 5, 6, 7, 8, 9 };
    to_assign = std::move(l);
    assert(l.size() == 0U);
    assert(to_assign.size() == 3);
    assert(to_assign.front() == 1);
    assert(to_assign.back() == 3);
  }

void list_insert_test()
  {
    {
      test_list<int> l { 1, 2, 3 };
      test_list<int> assertion { 1, 4, 2, 3 };
      l.insert(l.begin(), 4);
      assert(l == assertion);
    } {
      test_list<int> l { 1, 2, 3 };
      test_list<int> assertion { 1, 2, 4, 3 };
      l.insert(l.begin() + 1, 4);
      assert(l == assertion);
    } {
      test_list<int> l { 1, 2, 3 };
      test_list<int> assertion { 1, 2, 3, 4 };
      l.insert(l.end(), 4);
      assert(l == assertion);
    }
  }

void list_copy_assign_test()
  {
    test_list<int> l { 1, 2, 3 };
    test_list<int> to_assign { 4, 5, 6, 7, 8, 9 };
    to_assign = l;
    assert(l.size() == 3);
    assert(to_assign.size() == 3);
    assert(to_assign.front() == 1);
    assert(to_assign.back() == 3);
  }

void list_copy_ctor_test()
{
  test_list<int> l { 1, 2, 3 };
  test_list<int> copied { l };

  assert(l.size() == 3);
  assert(l.front() == 1);
  assert(l.back() == 3);

  assert(copied.size() == 3);
  assert(copied.front() == 1);
  assert(copied.back() == 3);
}

void list_move_ctor_test()
{
  test_list<int> l { 1, 2, 3 };
  test_list<int> moved { std::move(l) };

  assert(l.size() == 0U);

  assert(moved.size() == 3);
  assert(moved.front() == 1);
  assert(moved.back() == 3);
}

void list_basic_insertion_test()
  {
    test_list<int> l;
    assert(l.size() == 0U);
    l.push_back(1); // 1
    assert(l.size() == 1);
    assert(l.front() == 1);
    assert(l.back() == 1);
    assert(l.size() == 1);
    l.pop_back(); //
    assert(l.size() == 0U);
    l.push_front(1); // 1
    assert(l.size() == 1);
    assert(l.front() == 1);
    assert(l.back() == 1);
    l.push_front(2); // 2, 1
    assert(l.size() == 2);
    assert(l.front() == 2);
    assert(l.back() == 1);
    l.push_front(3); // 3, 2, 1
    assert(l.front() == 3);
    assert(l.back() == 1);
  }

void list_compare_tests()
  {
    test_list<int> l1 { 1, 2, 3 };
    test_list<int> l2 { 1, 2, 3 };
    assert(l1 == l2);
    l2.pop_back();
    assert(l1 != l2);
  }

void list_sort_tests()
  {
    {
      test_list<int>{}.sort();
    } {
      test_list<int> l { 3, 1, 2, 10 };
      test_list<int> sorted { 1, 2, 3, 10 };
      l.sort();
      assert(l == sorted);
    }
  }

void list_erase_tests()
  {
    test_list<int> l { 3, 1, 2, 10 };
    test_list<int> erased { 2, 10 };
    l.erase(l.begin());
    l.erase(l.begin());
    assert(l == erased);
    assert(erased.size() == 2);
  }

int main()
  {
    list_range_test();
    list_assign_function_test();
    list_insert_test();
    list_copy_assign_test();
    list_move_assign_test();
    list_copy_ctor_test();
    list_move_ctor_test();
    list_equality_test();
    list_basic_insertion_test();
    list_compare_tests();
    list_sort_tests();
    list_erase_tests();
  }
