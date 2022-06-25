#ifndef TASKQUEUE_H
#define TASKQUEUE_H

#include <iostream>

#include <atomic>
#include <future>
#include <mutex>
#include <thread>

#include <sstream>
#include <tuple>
#include <vector>
#include <deque>


class TaskQueue {
    enum class stop_policy {
        wait_for_queue_completion,
        stop_after_current_task
    };

public:
    using exception_callback_t = std::function<void(std::exception_ptr)>;

    explicit TaskQueue(std::size_t num_workers = std::thread::hardware_concurrency());

    TaskQueue(const TaskQueue&) = delete;
    TaskQueue(TaskQueue&&) = delete;
    TaskQueue& operator=(const TaskQueue&) = delete;
    TaskQueue& operator=(TaskQueue&&) = delete;

    TaskQueue& set_exception_handler(exception_callback_t handler);

    TaskQueue& set_num_workers(std::size_t num);

    void start();
    void stop();

    template <typename Function, typename... Args>
    bool push_void_task(Function&& fn, Args&&... args);

    template <typename Function, typename... Args, typename Invoke_Task_Type =
        std::invoke_result_t<Function, Args...>>
    std::pair<bool, std::future<Invoke_Task_Type>> push_future_task(Function&& fn, Args&&... args);

    void wait_for_completion();

    ~TaskQueue();

private:
    void set_default_exception_handler();
    void init_workers(std::size_t num);

    std::condition_variable m_notifier;
    std::deque<std::function<void()>> m_tasks;
    exception_callback_t m_on_exception;
    std::mutex m_locker;
    std::vector<std::thread> m_workers;
    std::size_t m_num_workers;
    stop_policy m_stop_policy = stop_policy::stop_after_current_task;
    std::atomic<bool> m_running;
    std::atomic<bool> m_stop_requested;
};

inline TaskQueue::TaskQueue(std::size_t num_workers)
    : m_num_workers(num_workers)
    , m_running(false)
    , m_stop_requested(false)
{
    set_default_exception_handler();
}

inline TaskQueue& TaskQueue::set_exception_handler(TaskQueue::exception_callback_t handler)
{
    m_on_exception = std::move(handler);
    return *this;
}

inline TaskQueue& TaskQueue::set_num_workers(std::size_t num)
{
    if (m_running)
        throw std::runtime_error("Attempt to change number of workers while running");
    m_num_workers = num;
    return *this;
}

inline void TaskQueue::start()
{
    if (m_running)
        return;
    m_running = true;
    init_workers(m_num_workers);
}

inline void TaskQueue::stop()
{
    if (!m_running)
        return;
    m_stop_requested = true;
    m_running = false;
    m_notifier.notify_all();
    for (auto& w : m_workers)
        w.join();
    m_workers.clear();
    m_stop_requested = false;
}

template<typename Function, typename... Args>
bool TaskQueue::push_void_task(Function&& fn, Args&&... args)
{
    if (m_stop_requested)
        return false;
    std::unique_lock<decltype (m_locker)> lock(m_locker);
    m_tasks.emplace_back([this, fn = std::forward<Function>(fn), tp = std::make_tuple(std::forward<Args>(args)...)]() mutable {
        try {
            std::apply(std::forward<Function>(fn), std::forward<decltype (tp)>(tp));
        }
        catch (...) {
            m_on_exception(std::current_exception());
        }
    });
    m_locker.unlock();
    m_notifier.notify_one();
    return true;
}

inline void TaskQueue::wait_for_completion()
{
    if (!m_running)
        return;
    m_stop_policy = stop_policy::wait_for_queue_completion;
    stop();
    m_stop_policy = stop_policy::stop_after_current_task;
    start();
}

inline TaskQueue::~TaskQueue()
{
    stop();
}

void TaskQueue::set_default_exception_handler()
{
    m_on_exception = [](std::exception_ptr ex_ptr) -> void
    {
    if (!ex_ptr) {
        return;
    }
    std::ostringstream stream;
    stream << "[TaskQueue] Exception from thread (" << std::this_thread::get_id() << "): ";
    try {
        std::rethrow_exception(ex_ptr);
    }
    catch (std::exception& ex) {
        stream << ex.what();
    }
    catch (...) {
        stream << "Unknown exception";
    }
    std::cerr << stream.str() << std::endl;
    };
}

inline void TaskQueue::init_workers(std::size_t num)
{
    for (std::size_t i = 0; i < num; ++i)
    {
        m_workers.emplace_back([this]() {
            while (true)
            {
                std::unique_lock<decltype (m_locker)> locker(m_locker);
                m_notifier.wait(locker, [this]{
                    return !m_tasks.empty() || m_stop_requested;
                });
                if (m_stop_requested)
                    if (m_tasks.empty() || m_stop_policy == stop_policy::stop_after_current_task)
                        return;
                auto task = std::move(m_tasks.front());
                m_tasks.pop_front();
                locker.unlock();
                task();
            }
        });
    }
}

template<typename Function, typename... Args, typename Invoke_Task_Type>
inline std::pair<bool, std::future<Invoke_Task_Type>> TaskQueue::push_future_task(Function&& fn, Args&&... args)
{
    if (m_stop_requested)
    {
        return { false, std::future<Invoke_Task_Type>() };
    }
    auto result_promise = std::make_shared<std::promise<Invoke_Task_Type>>();
    auto result_future = result_promise->get_future();
    std::unique_lock<decltype (m_locker)> lock(m_locker);

    m_tasks.emplace_back([fn = std::forward<Function>(fn),
                          tp = std::make_tuple(std::forward<Args>(args)...),
                          promise = std::move(result_promise)]() mutable {
        if constexpr (std::is_void_v<Invoke_Task_Type>)
        {
            try {
                std::apply(std::forward<Function>(fn), std::forward<decltype (tp)>(tp));
                promise->set_value();
            }
            catch (...) {
                promise->set_exception(std::current_exception());
            }
        }
        else {
            try {
                promise->set_value(std::apply(std::forward<Function>(fn), std::forward<decltype (tp)>(tp)));
            }
            catch (...) {
                promise->set_exception(std::current_exception());
            }
        }
    });
    lock.unlock();
    m_notifier.notify_one();
    return { true, std::move(result_future) };
}


void run()
{
    TaskQueue pool;

    for (size_t i = 0; i < 10; i++)
    {
        auto result = pool.push_future_task([&i]() mutable -> void {
            std::cout << i << std::endl;
            using namespace std::chrono_literals;
            std::this_thread::sleep_for(0.5s);
        });
    }

    pool.start();
    pool.wait_for_completion();
}

#endif // TASKQUEUE_H
























