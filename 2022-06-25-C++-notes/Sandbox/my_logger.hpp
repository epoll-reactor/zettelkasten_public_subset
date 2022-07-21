#pragma once

#include <iostream>
#include <sstream>
#include <fstream>
#include <ctime>
#include <mutex>
#include <cstring>


enum class log_flag : std::uint32_t
{
  info                = (1 << 0),
  debug               = (1 << 1),
  warn                = (1 << 2),
  error               = (1 << 3),
  nostdout            = (1 << 4),
  spaces              = (1 << 5),
  newline             = (1 << 6),
  round_brackets      = (1 << 7),
  box_bracekts        = (1 << 8),
  curly_brackets      = (1 << 9),
  chevrons            = (1 << 10),
  unixtime            = (1 << 11),
  no_sync_with_stdio  = (1 << 12),
#if defined(__linux__) || defined(__FreeBSD__)
  red                 = (1 << 13),
  green               = (1 << 14),
  yellow              = (1 << 15),
  purple              = (1 << 16),
  gray                = (1 << 17)
#endif
};

inline log_flag operator | (log_flag lhs, log_flag rhs) noexcept {
  return static_cast<log_flag>(static_cast<std::uint32_t>(lhs) | static_cast<std::uint32_t>(rhs));
}
inline std::uint32_t operator |= (std::uint32_t& lhs, log_flag rhs) noexcept {
  return lhs |= static_cast<std::uint32_t>(rhs);
}
inline std::uint32_t operator &= (std::uint32_t& lhs, log_flag rhs) noexcept {
  return lhs &= static_cast<std::uint32_t>(rhs);
}
inline std::uint32_t operator ~ (log_flag lhs) noexcept {
  return ~static_cast<std::uint32_t>(lhs);
}
inline std::uint32_t operator & (std::uint32_t lhs, log_flag rhs) noexcept {
  return lhs & static_cast<std::uint32_t>(rhs);
}

class log_writer {
public:
  log_writer() = delete;
  log_writer(const log_writer&) = delete;
  log_writer(log_writer&&) = delete;
  log_writer& operator=(log_writer&&) = delete;
  log_writer& operator=(const log_writer&) = delete;

  explicit log_writer(log_flag);
  explicit log_writer(std::string_view, log_flag);
  ~log_writer();

  template <typename T>
  log_writer& operator<<(T&& data) & = delete;
  template <typename T>
  log_writer&& operator<<(T&& data) &&;

private:
  std::string type()                     & noexcept;
  std::string colorize(std::string_view) &;
  std::string unixtime()           const &;
  std::string time()               const &;
  void        logging()                  &;

#if !defined(__linux__) && !defined(__FreeBSD__)
  constexpr
#endif
  inline std::string_view begin_color() const & noexcept;

#if !defined(__linux__) && !defined(__FreeBSD__)
  constexpr
#endif
  inline std::string_view end_color() const & noexcept;

  static inline
  std::stringstream   ss;
  std::uint32_t       flags;
  std::string_view    path;

#if defined(__linux__) || defined(__FreeBSD__)
  static inline constexpr std::string_view red    = "\033[0;31m";
  static inline constexpr std::string_view green  = "\033[0;32m";
  static inline constexpr std::string_view yellow = "\033[0;33m";
  static inline constexpr std::string_view purple = "\033[0;35m";
  static inline constexpr std::string_view gray   = "\033[0;90m";
  static inline constexpr std::string_view eoc    = "\033[0;0m";
#endif
};

template<typename T>
log_writer&& log_writer::operator<<(T&& data) && {
  if ( (flags & log_flag::newline         ) > 0) ss << '\n';

  if ( (flags & log_flag::box_bracekts    ) > 0) ss << '[';
  if ( (flags & log_flag::chevrons        ) > 0) ss << '<';
  if ( (flags & log_flag::curly_brackets  ) > 0) ss << '{';
  if ( (flags & log_flag::round_brackets  ) > 0) ss << '(';

  ss << data;

  if ( (flags & log_flag::round_brackets  ) > 0) ss << ')';
  if ( (flags & log_flag::curly_brackets  ) > 0) ss << '}';
  if ( (flags & log_flag::chevrons        ) > 0) ss << '>';
  if ( (flags & log_flag::box_bracekts    ) > 0) ss << ']';

  if ( (flags & log_flag::spaces          ) > 0) ss << ' ';

  return std::move(*this);
}

log_writer::log_writer(log_flag log_flags)
  : flags   (static_cast<std::uint32_t>(log_flags))
  , path    ("")
{ }

log_writer::log_writer(std::string_view filename, log_flag log_flags)
  : flags   (static_cast<std::uint32_t>(log_flags))
  , path    (filename)
{ }

std::string log_writer::time() const & {
  long now = std::time(nullptr);

  std::string time(25, 0);
  std::strftime(&time[0], time.size(), "[%Y-%m-%d %H:%M:%S]", std::localtime(&now));

  return time;
}

std::string log_writer::unixtime() const & {
  return "[" + std::to_string(std::time(nullptr)) + "]";
}

#if !defined(__linux__) && !defined(__FreeBSD__)
constexpr
#endif
std::string_view log_writer::begin_color() const & noexcept {
#if defined(__linux__) || defined(__FreeBSD__)
  if ((flags & log_flag::red     ) > 0) return red;
  if ((flags & log_flag::green   ) > 0) return green;
  if ((flags & log_flag::purple  ) > 0) return purple;
  if ((flags & log_flag::yellow  ) > 0) return yellow;
  if ((flags & log_flag::gray    ) > 0) return gray;
#endif
  return "";
}

#if !defined(__linux__) && !defined(__FreeBSD__)
constexpr
#endif
std::string_view log_writer::end_color() const & noexcept {
#if defined(__linux__) || defined(__FreeBSD__)
  if ((flags & log_flag::red     ) > 0) return eoc;
  if ((flags & log_flag::green   ) > 0) return eoc;
  if ((flags & log_flag::purple  ) > 0) return eoc;
  if ((flags & log_flag::yellow  ) > 0) return eoc;
  if ((flags & log_flag::gray    ) > 0) return eoc;
#endif
  return "";
}

std::string log_writer::type() & noexcept {
  static const log_flag all_except_green  = log_flag::gray | log_flag::red   | log_flag::purple | log_flag::yellow;
  static const log_flag all_except_gray   = log_flag::red  | log_flag::green | log_flag::purple | log_flag::yellow;
  static const log_flag all_except_yellow = log_flag::red  | log_flag::green | log_flag::purple | log_flag::gray;
  static const log_flag all_except_red    = log_flag::gray | log_flag::green | log_flag::purple | log_flag::yellow;

  auto any_of = [&](auto... args) -> bool { return (static_cast<bool>(flags & args) | ... ); };

  std::string flaglist;

  if ( (flags & log_flag::info ) > 0) { flaglist += "[ INFO   ]"; if (!any_of(all_except_green  )) { flags |= log_flag::green;  } }
  if ( (flags & log_flag::debug) > 0) { flaglist += "[ DEBUG  ]"; if (!any_of(all_except_gray   )) { flags |= log_flag::gray;   } }
  if ( (flags & log_flag::warn ) > 0) { flaglist += "[ WARN   ]"; if (!any_of(all_except_yellow )) { flags |= log_flag::yellow; } }
  if ( (flags & log_flag::error) > 0) { flaglist += "[ ERROR  ]"; if (!any_of(all_except_red    )) { flags |= log_flag::red;    } }

  return flaglist;
}

std::string log_writer::colorize(std::string_view text) & {
  return std::string { begin_color() } + std::string { text } + std::string { end_color() };
}

void log_writer::logging() & {
  std::string source;
  (((flags & log_flag::unixtime) > 0))
    ? source = unixtime() + ' ' + type() + ' ' + ss.str() + '\n'
    : source =     time() + ' ' + type() + ' ' + ss.str() + '\n';

  if (!((flags & log_flag::no_sync_with_stdio) > 0)) {
    std::ios::sync_with_stdio(false);
  }
  if (!((flags & log_flag::nostdout) > 0)) {
    std::cout << colorize(source) << std::flush;
  }

  if (!path.empty()) {
    std::ofstream ofs(path.data(), std::ios::app);
    ofs.write(source.c_str(), source.size());
  }
  ss.str("");
  ss.clear();
}

log_writer::~log_writer() {
  static std::mutex mtx;
  std::lock_guard<decltype(mtx)> guard(mtx);
  if (ss.rdbuf()->in_avail())
    logging();
}

// --------------------------------

#include <thread>
#include <vector>

void log_pitsa()
{
  log_writer(log_flag::error) << "GET";
  log_writer(log_flag::debug | log_flag::red) << "GET";
  log_writer(log_flag::debug) << "GET";
  log_writer(log_flag::warn | log_flag::gray) << "GET";
  log_writer(log_flag::warn) << "GET";
  log_writer(log_flag::info | log_flag::gray) << "GET";
  log_writer(log_flag::info) << "GET";
  log_writer(log_flag::debug) << "GET";
  log_writer(log_flag::info ) << "GET";
  log_writer(log_flag::warn ) << "GET";
  using namespace std::chrono_literals;
  std::this_thread::sleep_for(1s);
}

void single_log()
{
//  log_writer("/home/krot/pizzalog.txt", log_flag::warn | log_flag::unixtime | log_flag::nostdout) << "000000";
  log_writer(log_flag::warn | log_flag::unixtime /*| log_flag::no_sync_with_stdio*/) << "pitsaaaaaa";
//  log_writer("/home/krot/pizzalog.txt", log_flag::warn | log_flag::unixtime) << "pitsaaaaaa";
}

void thread_log()
{
  std::vector<std::thread> threads;

  const std::size_t NUM_THREADS = 100;

  for (std::size_t i = 0; i < NUM_THREADS; i++)
    threads.push_back(std::thread(single_log));

  for (std::size_t i = 0; i < NUM_THREADS; i++)
    threads[i].join();
}


void cout_pitsa()
{
  static std::mutex mtx;
  std::lock_guard<std::mutex> guard(mtx);
  std::cout << "я хочу питсу" << std::endl;
}

#include <testSpeed.hpp>

void run()
{
  constexpr std::size_t mln = 1'000'000;
  log_pitsa();
  thread_log();
  single_log();

  testFuncSpeed(mln, single_log);
//  testFuncSpeed(mln, log_pitsa);
//  testFuncSpeed(mln / 1000, thread_log);
}
