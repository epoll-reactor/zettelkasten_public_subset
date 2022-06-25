// Jun 19, 2021
#ifndef SANDBOX_EXAM_3_HPP
#define SANDBOX_EXAM_3_HPP

#include <bits/stdc++.h>

enum struct push_policy : uint8_t {
  sort,
  not_sort
};
/*!
 * Sequence container that implements classical
 * linked list.
 */
template <typename T>
class list {
  struct node_ {
    T payload;
    node_* next = nullptr;
  };

public:
  /*!
   * Create an empty list
   * \pre size_ == 0, head_ == nullptr
   * \post size_ == 0, head_ == nullptr
   */
  list() = default;
  /*!
   * Destroy list. All elements contained inside are cleared.
   */
  ~list() {
    clear();
  }
  /*!
   * Destroy list. All elements contained inside are cleared.
   */
  void clear() noexcept {
    node_* walker = head_;
    while (walker != nullptr) {
      node_* killer = walker;
      advance(walker);
      delete killer;
    }
  }
  /*!
   * Push element to list with selected sort policy.
   */
  void push(const T& value, push_policy policy) {
    if (policy == push_policy::sort) {
      push_sorted(value);
    } else {
      push_not_sort(value);
    }
  }
  /*!
   * Move head_ to selected count of nodes.
   * If count > available nodes, it's user responsibility.
   */
  void advance(size_t count) {
    while (count-- > 0) {
      advance(head_);
    }
  }
  /*!
   * Remove selected node.
   * \pre node != nullptr
   * \post node setted to node->next
   */
  void pop(node_*& node) {
    if (!node) {
      return;
    }
    node_* killer = node;
    advance(node);
    delete killer;
    --size_;
  }

  /*!
   * Remove heading element.
   */
  void pop() {
    pop(head_);
  }

  /*!
   * Return head list internal data.
   * \throws std::runtime_error if no elements present.
   */
  const T& front() const {
    if (!head_) {
      throw std::runtime_error("Empty list");
    }
    return head_->payload;
  }
  /*!
   * Print to stdout all contents of container.
   */
  void print() {
    node_* walker = head_;
    while (walker != nullptr) {
      std::cout << walker->payload << "\n";
      advance(walker);
    }
    std::cout << std::endl;
  }

private:
  /*!
   * Push element to list.
   * \pre head_ can be setted up or nullptr either.
   * \post new element with `value` data is pushed in sorted order.
   */
  void push_sorted(const T& value) {
    node_* element = new node_{value};
    if (!head_ || head_->payload > element->payload) { /// По возрастанию
//    if (!head_ || head_->payload <= element->payload) { /// По убыванию
      ++size_;
      element->next = head_;
      head_ = element;
      return;
    }
    node_* current = head_;
    while (has_next(current) && current->next->payload <= element->payload) { /// По возрастанию
//    while (has_next(current) && current->next->payload > element->payload) { /// По убыванию
      advance(current);
    }
    element->next = current->next;
    current->next = element;
    ++size_;
  }
  /*!
   * Push element to list.
   * \pre head_ can be setted up or nullptr either.
   * \post new element with `value` data is pushed in not sorted order.
   */
  void push_not_sort(const T& value) {
    node_* element = new node_{value};
    if (!head_) {
      ++size_;
      element->next = head_;
      head_ = element;
      return;
    }
    node_* current = head_;
    while (has_next(current)) {
      advance(current);
    }
    element->next = current->next;
    current->next = element;
  }
  /*!
   * Validate pointer.
   * \return true if node->next is present, else otherwise.
   */
  static bool has_next(node_* node) noexcept {
    if (!node) {
      return false;
    }
    if (!node->next) {
      return false;
    }
    return static_cast<bool>(node->next);
  }
  /*!
   * Advance pointer by one. Absence of node->next is function user responsibility.
   */
  static void advance(node_*& node) noexcept {
    node = node->next;
  }

  size_t size_ { 0 };
  node_* head_ { nullptr };
};

template <typename Delimiter>
list<std::string> split(std::string data, Delimiter&& delim)
{
  list<std::string> splitted;
  size_t pos = 0;
  while (pos != std::string::npos) {
    pos = data.find_first_not_of(delim);
    if (pos == std::string::npos) {
      return splitted;
    }
    data = data.substr(pos);
    pos = data.find_first_of(delim);
    splitted.push(data.substr(0, pos), push_policy::not_sort);
    data = data.substr(pos == std::string::npos ? 0 : pos);
  }
  return splitted;
}

class student {
public:
  student(
    std::string imie,
    std::string nazwisko,
    int64_t index,
    float ocena
  ) : imie_(imie)
    , nazwisko_(nazwisko)
    , index_(index)
    , ocena_(ocena) {}

  std::string imie() const { return imie_; }
  std::string nazwisko() const { return nazwisko_; }
  int64_t     index() const { return index_; }
  float       ocena() const { return ocena_; }
    
private:
  std::string imie_;
  std::string nazwisko_;
  int64_t index_;
  float ocena_;
};

std::ostream& operator<<(std::ostream& ostream, const student& s) {
  return ostream << "(" << s.nazwisko()
                 << ", " << s.imie()
                 << ", " << s.index()
                 << ", " << s.ocena() << ")";
}
bool operator<(const student& lhs, const student& rhs) noexcept {
  return (lhs.index() - rhs.index()) < 0;
}
bool operator>(const student& lhs, const student& rhs) noexcept {
  return (lhs.index() - rhs.index()) > 0;
}
bool operator<=(const student& lhs, const student& rhs) noexcept {
  return (lhs.index() - rhs.index()) <= 0;
}
bool operator>=(const student& lhs, const student& rhs) noexcept {
  return (lhs.index() - rhs.index()) >= 0;
}

student parse_student(const std::string& line) {
  list<std::string> parsed = split(line, ' ');
  std::string nazwisko = parsed.front();
  parsed.advance(1);
  std::string imie = parsed.front();
  parsed.advance(1);
  size_t index = std::stoull(parsed.front());
  parsed.advance(1);
  float ocena = std::stof(parsed.front());
  return {std::move(imie), std::move(nazwisko), index, ocena};
}

void run_exam_3() {
  std::ofstream gra_out("gra.txt");
  std::ofstream sid_out("sid.txt");

  gra_out << "Kowalski Jan 123456 4.5\n";
  gra_out << "Tomaszewski Maciej 000000 4.8\n";

  sid_out << "Nazwisko Imie 111222 3.9\n";
  sid_out << "Matuszewski Jan 333444 4.1\n";

  gra_out.close();
  sid_out.close();

  std::ifstream gra("gra.txt");
  std::ifstream sid("sid.txt");

  list<student> all_students;

  std::string line;
  while (std::getline(gra, line)) {
    all_students.push(parse_student(line), push_policy::sort);
  }
  while (std::getline(sid, line)) {
    all_students.push(parse_student(line), push_policy::sort);
  }

  all_students.print();

  std::remove("gra.txt");
  std::remove("sid.txt");
}

#endif //SANDBOX_EXAM_3_HPP
