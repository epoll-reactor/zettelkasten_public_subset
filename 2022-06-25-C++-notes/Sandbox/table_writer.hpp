// Nov 11, 2021
#include <bits/stdc++.h>

template <typename T>
struct is_builtin_string : std::disjunction<
  std::is_same<char*, std::decay_t<T>>,
  std::is_same<const char*, std::decay_t<T>>
> {};

template <typename T>
struct is_std_string : std::disjunction<
  std::is_same<std::string, std::decay_t<T>>,
  std::is_same<std::string_view, std::decay_t<T>>
> {};

size_t utf8_length(std::string string) {
  return std::count_if(string.begin(), string.end(), [](char c) {
    return (static_cast<unsigned char>(c) & 0xC0) != 0x80;
  });
}

template <typename T>
size_t relative_width(const T& t) noexcept {
  if constexpr (std::is_arithmetic_v<T>) {
    return floor(log10(t) + 1);
  }
  if constexpr (is_builtin_string<T>::value) {
    return strlen(t);
  }
  if constexpr (is_std_string<T>::value) {
    return utf8_length(t);
  }
  return 0;
}

template <size_t width_>
class table_writer {
public:
  table_writer(std::ostream& stream_tape)
    : stream_tape_(stream_tape) {
    stream_tape_.clear();
  }

  template <typename... Headers>
  table_writer& set_headers(Headers&&... headers) {
    static_assert(sizeof...(Headers) == width_);
    std::apply([head = &*headers_](auto&&... args) mutable {
      ((*head++ = args), ...);
    }, std::tuple(headers...));
    return *this;
  }

  template <typename... Records>
  table_writer& add_row(Records&&... records) {
    static_assert(sizeof...(Records) == width_);
    auto to_string = [](auto arg) {
      if constexpr (std::is_arithmetic_v<decltype(arg)>) {
        return std::to_string(arg);
      } else if constexpr (is_std_string<decltype(arg)>::value || is_builtin_string<decltype(arg)>::value) {
        return arg;
      } else {
        throw std::runtime_error("Cannot convert value to string");
      }
    };
    fields_.push_back({to_string(records)...});
    return *this;
  }

  void flush() {
    size_t max_widths[width_];
    compute_max_widths(max_widths);

    put_separator(max_widths);
    stream_tape_ << '|';
    for (size_t i = 0; i < width_; ++i) {
      stream_tape_ << std::setfill(' ') << std::setw(max_widths[i] + 1) << headers_[i] <<  ' ' << '|';
    }
    stream_tape_ << std::endl;
    put_separator(max_widths);
    for (size_t i = 0; i < fields_.size(); ++i) {
      stream_tape_ << '|';
      for (size_t j = 0; j < width_; ++j) {
        stream_tape_ << ' ' << std::setfill(' ') << std::setw(max_widths[j]) << fields_[i][j] << ' ' << "|";
      }
      stream_tape_ << std::endl;
    }
    put_separator(max_widths);
  }

private:
  void compute_max_widths(size_t (&max_widths)[width_]) noexcept {
    size_t* widths_ptr = max_widths;
    for (const auto& header : headers_) {
      *widths_ptr++ = relative_width(header);
    }
    widths_ptr = max_widths;
    for (const auto& field : fields_) {
      for (const auto& field_record : field) {
        if (size_t w = relative_width(field_record); *widths_ptr < w) {
          *widths_ptr = w;
        }
        ++widths_ptr;
      }
      widths_ptr = max_widths;
    }
  }

  void put_separator(size_t max_widths[width_]) {
    const size_t space_indent_width = 2;
    stream_tape_ << '+';
    for (size_t i = 0; i < width_; ++i) {
      stream_tape_ << std::setfill('-') << std::setw(max_widths[i] + space_indent_width) << '-' << '+';
    }
    stream_tape_ << std::endl;
  }

  std::ostream& stream_tape_;
  std::string headers_[width_];
  std::vector<std::array<std::string, width_>> fields_;
};

void table_writer_tests() {
  {
    table_writer<3>(std::cout)
      .set_headers("Album", "by", "Songwriter")
      .add_row("Music to be murdered by", "by", "Eminem")
      .add_row("Music to drive-by", "by", "Compton's Most Wanted")
      .add_row("The Sinister Urge", "by", "Rob Zombie")
      .add_row(1, 2, 3)
      .flush();
  } {
    table_writer<5>(std::cout)
      .set_headers("1", "2", "3", "4", "5")
      .add_row("_", "__", "___", "____", "_____")
      .add_row("-", "-", "-", "--------------------------------------------------------------------", "-")
      .add_row("_____", "____", "___", "__", "_")
      .flush();
  } {
    std::ostringstream stream;
    table_writer<3>(stream)
      .set_headers("Album", "by", "Songwriter")
      .add_row("Music to be murdered by", "by", "Eminem")
      .add_row("Music to drive-by", "by", "Compton's Most Wanted")
      .add_row("The Sinister Urge", "by", "Rob Zombie")
      .add_row(1, 2, 3)
      .flush();
    std::string output = stream.str();
    std::transform(output.begin(), output.end(), output.begin(), [](char c) {
      if (c == ' ') {
        return '.';
      } else {
        return c;
      }
    });
    std::cout << output << std::endl;
  } {
    table_writer<2>(std::cout)
      .set_headers("норм", "ок")
      .add_row("нормально", "лады")
      .add_row("ыыыыыыыыыыыыыыыыыыыыыы", "нормалды")
      .add_row("1", "2")
      .add_row("а", "б")
      .flush();
  }
}



















