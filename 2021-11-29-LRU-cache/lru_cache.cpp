#include <iostream>
#include <unordered_map>
#include <list>
#include <cassert>

class cache_error : public std::exception {
public:
  explicit cache_error(const char* what)
    : error_(what) {}

  const char* what() const noexcept override {
    return error_.what();
  }

private:
  std::runtime_error error_;
};

template <typename T>
class lru_cache {
public:
  using value_type = T;

  explicit lru_cache(uint64_t cache_size)
    : cache_size_ (cache_size)
    , page_map_   ()
    , cache_      ()
    , hits_       ()
    , page_faults_()
  {}

  // push value to cache
  void refer(value_type page) {
    if (!page_map_.contains(page)) {
      // if page wasn't found in cache, increase count of page faults
      ++page_faults_;
      // delete last used page if the cache is full
      if (cache_.size() == cache_size_) {
        value_type last_page = cache_.back();
        cache_.pop_back();
        page_map_.erase(last_page);
      }
    } else {
      // the requested page exists in cache
      ++hits_;
      // page present in cache, so remove it from current position to move it to front
      cache_.erase(page_map_[page]);
    }
    cache_.push_front(page);
    page_map_[page] = cache_.begin();
  }

  bool exists(const value_type& value) const {
    return page_map_.contains(value);
  }

  const value_type& get(const value_type& value) {
    auto page = page_map_.find(value);
    if (page == page_map_.end()) {
      throw cache_error("value isn't exists in cache");
    }
    auto& [cached, iterator] = *page;
    // move accessed element to front of cache line
    cache_.splice(cache_.begin(), cache_, iterator);
    return cached;
  }

  uint64_t hits() const noexcept {
    return hits_;
  }

  uint64_t page_faults() const noexcept {
    return page_faults_;
  }

private:
  uint64_t            cache_size_; // total size of the cache
  std::unordered_map<value_type, typename std::list<value_type>::iterator>
                      page_map_;   // hash map with pages and their addresses
  std::list<T>        cache_;      // cache linked list
  uint64_t            hits_;
  uint64_t            page_faults_;
};

int main() {
  uint64_t expected_hits   = 2;
  uint64_t expected_faults = 4;
  lru_cache<uint64_t> cache(4);
  cache.refer(1); //         fault, not present in cache yet
  cache.refer(2); //         fault, not present in cache yet
  cache.refer(5); //         fault, not present in cache yet
  cache.refer(1); //         hit, already present in cache
  cache.refer(4); //         fault, not present in cache yet
  cache.refer(5); //         hit, already present in cache

  // 1: cache [1]            page fault, put to front
  // 2: cache [2, 1]         page fault, put to front
  // 3: cache [5, 2, 1]      page fault, put to front
  // 4: cache [1, 5, 2]      page hit, erase accessed element and put it to front
  // 5: cache [4, 1, 5, 2]   page fault, put to front
  // 6: cache [5, 4, 1, 2]   page hit, erase accessed element and put it to front

  try {
    cache.get(5); // cache: [5, 4, 1, 2], move 5 to front -> [5, 4, 1, 2]
    cache.get(4); // cache: [5, 4, 1, 2], move 4 to front -> [4, 5, 1, 2]
    cache.get(1); // cache: [4, 5, 1, 2], move 1 to front -> [1, 4, 5, 2]
  } catch (...) {
    assert(0 && "5 exists, should not throw");
  }
  try {
    cache.get(123);
    assert(0 && "123 doesn't exists, should throw");
  } catch (...) {}

  assert( cache.hits() == expected_hits);
  assert( cache.page_faults() == expected_faults);
  assert( cache.exists(5));
  assert(!cache.exists(6));
}
