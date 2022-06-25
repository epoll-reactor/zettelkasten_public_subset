#include <deque>
#include <condition_variable>
#include <mutex>
#include <charconv>
#include <filesystem>
#include <fstream>
#include <thread>
#include <iostream>


template <typename>
class log_message_proxy;
class log_message_dispatcher;

template <typename T, typename... Ts>
inline constexpr bool any_of = (std::is_same_v<T, Ts> || ... || false);

enum class format_flag : std::uint8_t {
  write_to_file     = 1 << 1,
  write_to_stdout   = 1 << 2,
  write_date_time   = 1 << 3,
  write_type        = 1 << 4
};

constexpr format_flag operator~(format_flag lhs) noexcept {
  return static_cast<format_flag>(~static_cast<std::underlying_type_t<format_flag>>(lhs));
}
constexpr format_flag operator|(format_flag lhs, format_flag rhs) noexcept {
  return static_cast<format_flag>(
    static_cast<std::underlying_type_t<format_flag>>(lhs) |
    static_cast<std::underlying_type_t<format_flag>>(rhs));
}
constexpr format_flag operator&(format_flag lhs, format_flag rhs) noexcept {
  return static_cast<format_flag>(
    static_cast<std::underlying_type_t<format_flag>>(lhs) &
    static_cast<std::underlying_type_t<format_flag>>(rhs));
}
constexpr format_flag operator|=(format_flag& lhs, format_flag rhs) noexcept {
  lhs = lhs | rhs;
  return lhs;
}
constexpr format_flag operator&=(format_flag& lhs, format_flag rhs) noexcept {
  lhs = lhs & rhs;
  return lhs;
}

enum class type_flag : std::uint8_t {
  undefined   = 0,
  debug       = 1,
  info        = 2,
  warning     = 3,
  critical    = 4
};

constexpr bool operator<(type_flag lhs, type_flag rhs) noexcept {
  return static_cast<std::underlying_type_t<type_flag>>(lhs) <
         static_cast<std::underlying_type_t<type_flag>>(rhs);
}
constexpr bool operator<=(type_flag lhs, type_flag rhs) noexcept {
  return static_cast<std::underlying_type_t<type_flag>>(lhs) <=
         static_cast<std::underlying_type_t<type_flag>>(rhs);
}
constexpr bool operator>(type_flag lhs, type_flag rhs) noexcept {
  return static_cast<std::underlying_type_t<type_flag>>(lhs) >
         static_cast<std::underlying_type_t<type_flag>>(rhs);
}
constexpr bool operator>=(type_flag lhs, type_flag rhs) noexcept {
  return static_cast<std::underlying_type_t<type_flag>>(lhs) >=
         static_cast<std::underlying_type_t<type_flag>>(rhs);
}
class log_message_node {
private:
  template <typename>
  friend class log_message_proxy;

public:
  static constexpr std::string_view to_string(type_flag flag) noexcept {
    using namespace std::string_view_literals;
    switch (flag) {
    case type_flag::debug:    return "[Debug   ]"sv;
    case type_flag::info:     return "[Info    ]"sv;
    case type_flag::warning:  return "[Warning ]"sv;
    case type_flag::critical: return "[Critical]"sv;
    default: return ""sv;
    }
  }

  static std::string get_current_time() {
    std::string buffer(80, 0);
    time_t now = std::time(nullptr);
    std::tm t;
    t = *std::localtime(&now);
    strftime(buffer.data(), buffer.size(), "[%Y-%m-%d %X] ", &t);
    return buffer;
  }
  void set_current_time() noexcept {
    m_time = get_current_time();
  }

  inline constexpr log_message_node& set_type_flags(type_flag flags) noexcept {
    m_type_flags = flags;
    return *this;
  }
  inline constexpr log_message_node& set_format_flags(format_flag flags) noexcept {
    m_format_flags = flags;
    return *this;
  }
  template <typename...>
  struct always_false : std::bool_constant<false>{};

  template <typename T>
  constexpr log_message_node& push(const T& value) {
    if constexpr (any_of<std::decay_t<T>, std::string, std::string_view, char*, char>) {
      m_data += value;
    } else if constexpr (std::is_same_v<bool, std::decay_t<T>>) {
      m_data += (value) ? "true" : "false";
    } else if constexpr (std::is_arithmetic_v<std::decay_t<T>>) {
      std::array<char, 30> buf;
      std::to_chars_result res;

      if constexpr (std::is_same_v<std::decay_t<T>, float> || std::is_same_v<std::decay_t<T>, double>) {
        sprintf(buf.data(), "%f", value);
        m_data += buf.data();
      }
      else if constexpr (std::is_same_v<std::decay_t<T>, long double>) {
        sprintf(buf.data(), "%Lf", value);
        m_data += buf.data();
      } else {
        res = std::to_chars(buf.data(), buf.data() + buf.size(), value);
        if (res.ec == std::errc{}) {
          m_data += std::string_view(buf.data(), res.ptr - buf.data());
        }
      }
    } else {
      static_assert (always_false<T>{}, "Unsupported type for logging");
    }
    return *this;
  }

  inline const std::string& get_data() const noexcept {
    return m_data;
  }
  constexpr format_flag format_flags() const noexcept {
    return m_format_flags;
  }
  constexpr type_flag type_flags() const noexcept {
    return m_type_flags;
  }
  constexpr inline bool empty() const noexcept {
    return m_data.empty();
  }

private:
  log_message_node() = default;
  std::string m_data;
  std::string m_time;
  format_flag m_format_flags =
    format_flag::write_to_stdout |
    format_flag::write_date_time |
    format_flag::write_type;
  type_flag m_type_flags = type_flag::undefined;
};


template <typename On_Destructor_Callable>
class log_message_proxy {
  friend class log_message_dispatcher;
public:
  template <typename T>
  log_message_proxy& operator<<(T&& value) {
    m_node.push(value);
    return *this;
  }
  ~log_message_proxy() {
    if (!m_node.empty()) {
      m_node.set_current_time();
      m_callable(m_dispatcher, std::move(m_node));
    }
  }

private:
  log_message_proxy(log_message_dispatcher* dispatcher, On_Destructor_Callable callable)
    : m_dispatcher(dispatcher)
    , m_callable(callable)
  { }

  inline constexpr log_message_proxy& set_format_flags(format_flag fmt) {
    m_node.set_format_flags(fmt);
    return *this;
  }
  inline constexpr log_message_proxy& set_type_flags(type_flag fmt) {
    m_node.set_type_flags(fmt);
    return *this;
  }

  log_message_node m_node;
  log_message_dispatcher* m_dispatcher;
  On_Destructor_Callable m_callable;
};


class log_message_dispatcher {
public:
  enum class stop_policy : std::uint8_t {
    instant_stop,
    deferred_stop
  };
  using on_destructor_callable = void(*)(log_message_dispatcher*, log_message_node&&);
  using proxy = log_message_proxy<on_destructor_callable>;

  proxy debug() noexcept {
    return log_message_proxy<on_destructor_callable>(this, &log_message_dispatcher::proxy_callback)
      .set_format_flags(m_format_flags).set_type_flags(type_flag::debug);
  }
  proxy info() noexcept {
    return log_message_proxy<on_destructor_callable>(this, &log_message_dispatcher::proxy_callback)
      .set_format_flags(m_format_flags).set_type_flags(type_flag::info);
  }
  proxy warning() noexcept {
    return log_message_proxy<on_destructor_callable>(this, &log_message_dispatcher::proxy_callback)
      .set_format_flags(m_format_flags).set_type_flags(type_flag::warning);
  }
  proxy critical() noexcept {
    return log_message_proxy<on_destructor_callable>(this, &log_message_dispatcher::proxy_callback)
      .set_format_flags(m_format_flags).set_type_flags(type_flag::critical);
  }

  inline log_message_dispatcher& enable_write_to_stdout(bool enabled) noexcept {
    return set_format_flag(enabled, format_flag::write_to_stdout);
  }
  inline log_message_dispatcher& enable_write_date_time(bool enabled) noexcept {
    return set_format_flag(enabled, format_flag::write_date_time);
  }
  inline log_message_dispatcher& enable_write_log_type(bool enabled) noexcept {
    return set_format_flag(enabled, format_flag::write_type);
  }

  log_message_dispatcher& enable_write_to_file(bool enabled, std::filesystem::path file_path = "/tmp/log.txt") noexcept {
    if (enabled) {
      m_file_path = std::move(file_path);
      std::error_code ec;
      std::filesystem::create_directory(m_file_path.parent_path(), ec);
      m_file_stream.reset(new std::ofstream(m_file_path, std::ios::out | std::ios::app));
    }
    return set_format_flag(enabled, format_flag::write_to_file);
  }

  constexpr log_message_dispatcher& set_log_level(type_flag level) noexcept {
    m_log_level = level;
    return *this;
  }
  constexpr type_flag log_level() const noexcept {
    return m_log_level;
  }

  log_message_dispatcher(stop_policy policy = stop_policy::deferred_stop)
    : m_stop_policy(policy)
  {
    start();
  }

  void start() {
    if (!m_running) {
      m_stop_requested = false;
      m_running = true;
      m_worker = std::thread([this] {
        std::deque<log_message_node> current_nodes;
        while (m_running) {
          std::unique_lock lock(m_locker);
          m_notifier.wait(lock, [this]{
            return m_stop_requested || !m_queue.empty();
          });
          if (m_stop_requested && m_stop_policy == stop_policy::instant_stop) {
            return;
          }
          current_nodes = std::move(m_queue);
          m_queue.clear();
          lock.unlock();
          for (auto& node : current_nodes) {
            write_node(node);
          }
          current_nodes.clear();
          if (m_stop_requested && m_stop_policy == stop_policy::deferred_stop) {
            return;
          }
        }
      });
    }
  }

  void stop() {
    if (m_running) {
      m_stop_requested = true;
      m_notifier.notify_one();
      m_worker.join();
      m_running = false;
    }
  }

  ~log_message_dispatcher() {
    stop();
  }

  void push(log_message_node&& node) {
    if (m_stop_requested) {
      return;
    }
    std::unique_lock lock(m_locker);
    m_queue.push_back(std::forward<log_message_node>(node));
    lock.unlock();
    m_notifier.notify_one();
  }

private:
  static void proxy_callback(log_message_dispatcher* d, log_message_node&& node) {
    d->push(std::forward<log_message_node>(node));
  }

  inline constexpr log_message_dispatcher& set_format_flag(bool enabled, format_flag fmt) {
    if (enabled) {
      m_format_flags |= fmt;
    } else {
      m_format_flags &= ~fmt;
    }
    return *this;
  }

  void write_node(log_message_node& node) {
    std::string buffer;
    auto fmt = node.format_flags();
    auto has_format_flag = [fmt](format_flag f) -> bool {
      return static_cast<std::underlying_type_t<format_flag>>(fmt & f) != 0;
    };
    auto type = node.type_flags();
    if (m_log_level > type) {
      return;
    }
    if (!has_format_flag(format_flag::write_to_file) &&
        !has_format_flag(format_flag::write_to_stdout)) {
      return;
    }
    if (has_format_flag(format_flag::write_type)) {
      buffer += log_message_node::to_string(type);
    }
    if (has_format_flag(format_flag::write_date_time)) {
      buffer += log_message_node::get_current_time();
    }
    buffer += node.get_data();
    buffer += "\n";
    if (has_format_flag(format_flag::write_to_file)) {
      m_file_stream->write(buffer.data(), buffer.size());
      m_file_stream->flush();
    }
    if (has_format_flag(format_flag::write_to_stdout)) {
      std::cout.write(buffer.data(), buffer.size()).flush();
    }
  }

  std::deque<log_message_node> m_queue;
  std::condition_variable m_notifier;
  std::mutex m_locker;
  std::thread m_worker;
  format_flag m_format_flags =
    format_flag::write_to_stdout |
    format_flag::write_date_time |
    format_flag::write_type;
  type_flag m_log_level;
  std::filesystem::path m_file_path;
  std::unique_ptr<std::ofstream> m_file_stream;
  stop_policy m_stop_policy;
  bool m_running = false;
  bool m_stop_requested = false;
};

void run() {
  log_message_dispatcher dispatcher;
  dispatcher
    .set_log_level(type_flag::debug)
    .enable_write_date_time(true)
    .enable_write_to_file(true)
    .critical();
  dispatcher.warning() << "123" << 1 << 2 << 3;
  dispatcher.warning() << 124;
  dispatcher.warning() << "123";
  dispatcher.warning() << "123";
  dispatcher.warning() << "123";
  dispatcher.warning() << "Nu ladna ne pizda";
}

