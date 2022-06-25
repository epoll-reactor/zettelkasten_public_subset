// Mar 3, 2021
template <typename T>
class row {
public:
  explicit row(T* begin, T* end) noexcept : begin_(begin), end_(end) {}

  struct iterator {
    using value_type        = T;
    using reference         = T&;
    using const_reference   = const T&;
    using pointer           = T*;
    using const_pointer     = const T*;
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;

    iterator(T* position) noexcept : position_(position) {}
    iterator operator++() noexcept {
      ++position_;
      return *this;
    }
    iterator operator++(int) noexcept {
      return iterator(position_++);
    }
    T& operator *() noexcept { return *position_; }
    T* operator->() noexcept { return  position_; }
    bool operator==(const iterator& rhs) const noexcept { return position_ == rhs.position_; }
    bool operator!=(const iterator& rhs) const noexcept { return position_ != rhs.position_; }
  private:
    T* position_;
  };

  struct const_iterator {
    using value_type        = T;
    using reference         = T&;
    using const_reference   = const T&;
    using pointer           = T*;
    using const_pointer     = const T*;
    using iterator_category = std::forward_iterator_tag;
    using difference_type   = std::ptrdiff_t;

    const_iterator(const T* position) noexcept : position_(position) {}
    const_iterator operator++() noexcept {
      ++position_;
      return *this;
    }
    const_iterator operator++(int) noexcept {
      return const_iterator(position_++);
    }
    const T& operator *() const noexcept { return *position_; }
    const T* operator->() const noexcept { return  position_; }
    bool operator==(const_iterator rhs) const noexcept { return position_ == rhs.position_; }
    bool operator!=(const_iterator rhs) const noexcept { return position_ != rhs.position_; }
  private:
    const T* position_;
  };

  iterator begin()       { return       iterator(begin_); }
  const_iterator begin() const { return const_iterator(begin_); }
  iterator   end()       { return       iterator(  end_); }
  const_iterator   end() const { return const_iterator(  end_); }

private:
  T* begin_;
  T* end_;
};
