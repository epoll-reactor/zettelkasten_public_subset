// Apr 6, 2022
#include <iostream>

class process {
public:
  process(std::size_t pid = 0U, std::size_t num_threads = 1U)
    : pid_        (pid)
    , num_threads_(num_threads) {}

        std::size_t &pid()               { return pid_; }
  const std::size_t &pid()         const { return pid_; }
        std::size_t &num_threads()       { return num_threads_; }
  const std::size_t &num_threads() const { return num_threads_; }

private:
  std::size_t pid_;
  std::size_t num_threads_;
};

std::ostream &operator<<(std::ostream &out_stream, const process &p) {
  return
    out_stream << "process(PID = " << p.pid()
               << ", NUM_THREADS = " << p.num_threads()
               << ")";
}

class process_queue {
public:
  ~process_queue() {
    clear();
  }

  void push_front(process p) {
    if (size_ == 0U) {
      tail_ = new process_node{p, nullptr, nullptr};
      head_ = tail_;
    } else {
      head_->prev = new process_node{p, nullptr, head_};
      head_ = head_->prev;
    }
    ++size_;
  }

  void push_back(process p) {
    if (size_ == 0U) {
      head_ = new process_node{p, nullptr, nullptr};
      tail_ = head_;
    } else {
      tail_->next = new process_node{p, tail_, nullptr};
      tail_ = tail_->next;
    }
    ++size_;
  }

  void pop_front() {
    process_node *old_pointer = head_;
    head_ = head_->next;
    if (size_ == 1U) {
      tail_ = nullptr;
    } else {
      head_->prev = nullptr;
    }
    --size_;
    delete old_pointer;
  }

  void pop_back() {
    process_node *old_pointer = tail_;
    tail_ = tail_->prev;
    if (size_ == 1U) {
      head_ = nullptr;
    } else {
      tail_->next = nullptr;
    }
    --size_;
    delete old_pointer;
  }

  void print() {
    std::cout << "Process queue:\n";
    process_node *walker = head_;
    while (walker) {
      std::cout << "  " << walker->p << std::endl;
      walker = walker->next;
    }
  }

private:
  void clear() {
    while (size_ > 0U) {
      pop_back();
    }
  }

  struct process_node {
    process p;
    process_node *prev{nullptr};
    process_node *next{nullptr};
  } *head_{nullptr}, *tail_{nullptr};
  std::size_t size_{0U};
};

void run_process_queue_tests() {
  process_queue q;
  q.push_back(process{1U, 2U});
  q.push_back(process{3U, 4U});
  q.push_back(process{5U, 6U});
  q.print();
  q.pop_front();
  q.print();
}
