#pragma once

#include <sstream>


enum class logflag : std::uint32_t
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

inline logflag operator | (logflag lhs, logflag rhs) noexcept
{
  return static_cast<logflag>(static_cast<std::uint32_t>(lhs) | static_cast<std::uint32_t>(rhs));
}

inline std::uint32_t operator |= (std::uint32_t& lhs, logflag rhs) noexcept
{
  return lhs |= static_cast<std::uint32_t>(rhs);
}

inline std::uint32_t operator &= (std::uint32_t& lhs, logflag rhs) noexcept
{
  return lhs &= static_cast<std::uint32_t>(rhs);
}

inline std::uint32_t operator ~ (logflag lhs) noexcept
{
  return ~static_cast<std::uint32_t>(lhs);
}

inline std::uint32_t operator & (std::uint32_t lhs, logflag rhs) noexcept
{
  return lhs & static_cast<std::uint32_t>(rhs);
}

class logger
{
public:
  logger()                          = delete;
  logger(const logger&)             = delete;
  logger(logger&&)                  = delete;
  logger& operator=(logger&&)       = delete;
  logger& operator=(const logger&)  = delete;

  explicit logger(logflag);
  explicit logger(std::string_view, logflag);
  ~logger();

  template <typename T>
  inline logger&  operator<<(T&& data) & = delete;
  template <typename T>
  inline logger&& operator<<(T&& data) &&;

private:
  inline std::string type()                     & noexcept;
  inline std::string colorize(std::string_view) &;
  inline std::string unixtime()           const &;
  inline std::string time()               const &;
  inline void        logging()                  &;

#if !defined(__linux__) && !defined(__FreeBSD__)
  constexpr
#endif
  inline std::string_view begin_color()   const & noexcept;

#if !defined(__linux__) && !defined(__FreeBSD__)
  constexpr
#endif
  inline std::string_view end_color()     const & noexcept;

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
inline logger&& logger::operator<<(T&& data) &&
{
  if ( (flags & logflag::newline         ) > 0) ss << '\n';

  if ( (flags & logflag::box_bracekts    ) > 0) ss << '[';
  if ( (flags & logflag::chevrons        ) > 0) ss << '<';
  if ( (flags & logflag::curly_brackets  ) > 0) ss << '{';
  if ( (flags & logflag::round_brackets  ) > 0) ss << '(';

  ss << data;

  if ( (flags & logflag::round_brackets  ) > 0) ss << ')';
  if ( (flags & logflag::curly_brackets  ) > 0) ss << '}';
  if ( (flags & logflag::chevrons        ) > 0) ss << '>';
  if ( (flags & logflag::box_bracekts    ) > 0) ss << ']';

  if ( (flags & logflag::spaces          ) > 0) ss << ' ';

  return std::move(*this);
}
