// Nov 26, 2021
#include <iostream>
#include <thread>
#include <chrono>

enum struct direction { up, down, left, right };

class area {
public:
  area(std::string name = "p", char fill = '?', size_t pos_x = 0, size_t pos_y = 0)
    : name_ ( name)
    , fill_ ( fill)
    , pos_x_(pos_x)
    , pos_y_(pos_y) {}

  ~area() {
    std::cout << "area(" << name_ << ", " << fill_ << ")" << std::endl;
  }

  friend std::ostream& operator<<(std::ostream& stream, const area& a) {
    return stream << a.fill_;
  }

  void move(enum direction d) {
    switch (d) {
      case direction::up:    { --pos_y_; break; }
      case direction::down:  { ++pos_y_; break; }
      case direction::left:  { --pos_x_; break; }
      case direction::right: { ++pos_x_; break; }
      default: { break; }
    }
  }

  size_t& pos_x()             noexcept { return pos_x_; }
  const size_t& pos_x() const noexcept { return pos_x_; }
  size_t& pos_y()             noexcept { return pos_y_; }
  const size_t& pos_y() const noexcept { return pos_x_; }

private:
  std::string name_;
  char        fill_;
  size_t      pos_x_;
  size_t      pos_y_;
};

class planet {
public:
  planet(size_t size = 10, char border = '#')
    : size_  (size)
    , border_(border)
    , area_  (nullptr) {}

  void populate(area* area) {
    area_ = area;
    area->pos_x() = size_ / 2;
    area->pos_y() = size_ / 2;
  }

  void move(enum direction d) {
    std::cout << area_->pos_y() << std::endl;
    switch (d) {
      case direction::up: {
        if (area_->pos_y() > 0) {
          area_->move(direction::up);
        }
        break;
      }
      case direction::down: {
        if (area_->pos_y() < size_ - 2) {
          area_->move(direction::down);
        };
        break;
      }
      case direction::left: {
        if (area_->pos_x() > 0) {
          area_->move(direction::left);
        };
        break;
      }
      case direction::right: {
        if (area_->pos_y() < size_ - 2) {
          area_->move(direction::right);
        };
        break;
      }
      default: { break; }
    }
  }

  friend std::ostream& operator<<(std::ostream& stream, const planet& p) {
    const size_t size = p.size_;
    const area* area = p.area_;
    for (size_t i = 0; i < size; ++i) { // <- height
      for (size_t j = 0; j < size; ++j) { // <- width
        if (i == 0 || j == 0 || i == size - 1 || j == size - 1) {
          stream << p.border_;
        }
        else if (area && i == area->pos_y() && j == area->pos_x()) {
          stream << *area;
        }
        else {
          stream << ' ';
        }
      }
      stream << std::endl;
    }
    return stream;
  }

private:
  size_t size_;
  char   border_;
  area*  area_;
};

void univer_run() {
  area a;
  planet p(40, 'x');
  p.populate(&a);
  for (int i = 0; i < 10; ++i) {
    p.move(direction::right);
    std::cout << p << std::endl;
    using namespace std::chrono_literals;
    std::this_thread::sleep_for(0.3s);
    std::cout << "\033[2J\033[1;1H";
  }
}
