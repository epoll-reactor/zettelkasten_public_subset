#include "thread_pool.hpp"


//thread_pool::thread_pool(std::size_t threads_count)
//    : threads_waiting(0)
//    , is_terminate(false)
//    , is_pause(false)
//{
//  if (threads_count == 0) {
//    threads_count = std::thread::hardware_concurrency();
//  }
//  threads.reserve(threads_count);
//  std::generate_n(std::back_inserter(threads), threads_count, [this]{
//    return std::thread{thread_task, this};
//  });
//}

//thread_pool::~thread_pool() {
//  clear();
//  is_terminate = true;
//  jobs_avaliable.notify_all();
//  for (auto& thread : threads) {
//    if (thread.joinable())
//      thread.join();
//  }
//}

//std::size_t thread_pool::thread_count() {
//  return threads.size();
//}

//std::size_t thread_pool::waiting_jobs() {
//  std::unique_lock<decltype(jobs_mutex)> local_lock(jobs_mutex);
//  return jobs.size();
//}

//std::vector<std::thread::id> thread_pool::ids() {
//  std::vector<std::thread::id> id_s;
//  std::transform(
//    threads.begin(), threads.end(), id_s.begin(), [](auto& thread) {
//      return thread.get_id();
//    }
//  );
//  return id_s;
//}

//void thread_pool::clear() {
//  std::lock_guard<decltype(jobs_mutex)> local_lock(jobs_mutex);
//  while (!jobs.empty()) {
//    jobs.pop();
//  }
//}

//void thread_pool::pause(bool state) {
//  is_pause = state;
//  if (!is_pause) {
//    jobs_avaliable.notify_all();
//  }
//}

//void thread_pool::wait() {
//  while (threads_waiting != threads.size());
//}

//void thread_pool::thread_task(thread_pool* pool) {
//  while (true) {
//    if (pool->is_terminate)
//      break;
//    std::unique_lock<decltype(jobs_mutex)> local_lock(pool->jobs_mutex);
//    if (pool->jobs.empty() || pool->is_pause) {
//      ++pool->threads_waiting;
//      pool->jobs_avaliable.wait(local_lock, [&]{
//        return pool->is_terminate || !(pool->jobs.empty() || pool->is_pause);
//      });
//      --pool->threads_waiting;
//    }
//    if (pool->is_terminate) {
//      break;
//    }
//    auto job = std::move(pool->jobs.front());
//    pool->jobs.pop();
//    local_lock.unlock();
//    job();
//  }
//}


