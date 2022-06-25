#include "logger.hpp"

#include <iostream>
#include <fstream>
#include <ctime>
#include <mutex>
#include <cstring>


logger::logger(logflag logflags)
  : flags   (static_cast<std::uint32_t>(logflags))
  , path    ("")
{ }

logger::logger(std::string_view filename, logflag logflags)
  : flags   (static_cast<std::uint32_t>(logflags))
  , path    (filename)
{ }

std::string logger::time() const &
{
  long now = std::time(nullptr);

  std::string time(25, 0);
  std::strftime(&time[0], time.size(), "[%Y-%m-%d %H:%M:%S]", std::localtime(&now));

  return time;
}

std::string logger::unixtime() const &
{
  return "[" + std::to_string(std::time(nullptr)) + "]";
}

#if !defined(__linux__) && !defined(__FreeBSD__)
constexpr
#endif
std::string_view logger::begin_color() const & noexcept
{
#if defined(__linux__) || defined(__FreeBSD__)
  if ( (flags & logflag::red     ) > 0) return red;
  if ( (flags & logflag::green   ) > 0) return green;
  if ( (flags & logflag::purple  ) > 0) return purple;
  if ( (flags & logflag::yellow  ) > 0) return yellow;
  if ( (flags & logflag::gray    ) > 0) return gray;
#endif
  return "";
}

#if !defined(__linux__) && !defined(__FreeBSD__)
constexpr
#endif
std::string_view logger::end_color() const & noexcept
{
#if defined(__linux__) || defined(__FreeBSD__)
  if ( (flags & logflag::red     ) > 0) return eoc;
  if ( (flags & logflag::green   ) > 0) return eoc;
  if ( (flags & logflag::purple  ) > 0) return eoc;
  if ( (flags & logflag::yellow  ) > 0) return eoc;
  if ( (flags & logflag::gray    ) > 0) return eoc;
#endif
  return "";
}

std::string logger::type() & noexcept
{
  static const logflag all_except_green  = logflag::gray | logflag::red   | logflag::purple | logflag::yellow;
  static const logflag all_except_gray   = logflag::red  | logflag::green | logflag::purple | logflag::yellow;
  static const logflag all_except_yellow = logflag::red  | logflag::green | logflag::purple | logflag::gray;
  static const logflag all_except_red    = logflag::gray | logflag::green | logflag::purple | logflag::yellow;

  auto any_of = [&](auto... args) -> bool { return (static_cast<bool>(flags & args) | ... ); };

  std::string flaglist;

  if ( (flags & logflag::info ) > 0) { flaglist += "[ INFO   ]"; if (!any_of(all_except_green  )) { flags |= logflag::green;  } }
  if ( (flags & logflag::debug) > 0) { flaglist += "[ DEBUG  ]"; if (!any_of(all_except_gray   )) { flags |= logflag::gray;   } }
  if ( (flags & logflag::warn ) > 0) { flaglist += "[ WARN   ]"; if (!any_of(all_except_yellow )) { flags |= logflag::yellow; } }
  if ( (flags & logflag::error) > 0) { flaglist += "[ ERROR  ]"; if (!any_of(all_except_red    )) { flags |= logflag::red;    } }

  return flaglist;
}

std::string logger::colorize(std::string_view text) &
{
  return std::string { begin_color() } + std::string { text } + std::string { end_color() };
}

void logger::logging() &
{
  std::string source;
  (((flags & logflag::unixtime) > 0))
    ? source = unixtime() + ' ' + type() + ' ' + ss.str() + '\n'
    : source =     time() + ' ' + type() + ' ' + ss.str() + '\n';

  if (!((flags & logflag::no_sync_with_stdio) > 0))
  {
    std::ios::sync_with_stdio(false);
  }
  if (!((flags & logflag::nostdout) > 0))
  {
    std::cout << colorize(source) << std::flush;
  }

  if (!path.empty())
  {
    static std::ofstream ofs(path.data(), std::ios::app);
    ofs.write(source.c_str(), source.size());
  }
  ss.str("");
  ss.clear();
}

logger::~logger()
{
  static std::mutex mtx;
  std::lock_guard<decltype(mtx)> guard(mtx);
  if (ss.rdbuf()->in_avail())
    logging();
}
