#include <iostream>
#include <vector>
#include <string>
#include <unordered_map>
#include <iomanip>


enum class token_table {
  t_variable,
  t_const,
  t_identifier,
  t_equal,
  t_digit,
  t_string_literal,

  t_open_brace,
  t_close_brace,
  t_open_curly,
  t_close_curly,
  t_open_square,
  t_close_square,

  t_slash,
  t_backslash,
  t_colon,
  t_semicolon,
  t_star,

  t_plus,
  t_minus,
};

using token_element = std::pair<token_table, std::string_view>;
using token_t = std::vector<token_element>;

const token_t known_tokens = {
  {token_table::t_variable,     "variable"},
  {token_table::t_const,        "const"},
  {token_table::t_equal,        "="},
  {token_table::t_open_brace,   "("},
  {token_table::t_close_brace,  ")"},
  {token_table::t_open_curly,   "{"},
  {token_table::t_close_curly,  "}"},
  {token_table::t_open_square,  "["},
  {token_table::t_close_square, "]"},
  {token_table::t_slash,        "/"},
  {token_table::t_backslash,    "\\"},
  {token_table::t_colon,        ":"},
  {token_table::t_semicolon,    ";"},
  {token_table::t_star,         "*"},
  {token_table::t_plus,         "+"},
  {token_table::t_minus,        "-"},
};

/* Debug */ std::ostream& operator<<(std::ostream& stream, const token_element& token) {
  switch (token.first) {
  case token_table::t_variable:       stream << "[variable]: ";           break;
  case token_table::t_const:          stream << "[const]: ";              break;
  case token_table::t_identifier:     stream << "[identifier]: ";         break;
  case token_table::t_digit:          stream << "[digit]: ";              break;
  case token_table::t_equal:          stream << "[equal]: ";              break;
  case token_table::t_string_literal: stream << "[string_literal]: ";     break;
  case token_table::t_open_brace:     stream << "[open brace]: ";         break;
  case token_table::t_close_brace:    stream << "[close brace]: ";        break;
  case token_table::t_open_curly:     stream << "[open curly brace]: ";   break;
  case token_table::t_close_curly:    stream << "[close curly brace]: ";  break;
  case token_table::t_open_square:    stream << "[open square brace]: ";  break;
  case token_table::t_close_square:   stream << "[close curly brace]: ";  break;
  case token_table::t_slash:          stream << "[slash]: ";              break;
  case token_table::t_backslash:      stream << "[backslash]: ";          break;
  case token_table::t_colon:          stream << "[colon]: ";              break;
  case token_table::t_semicolon:      stream << "[semicolon]: ";          break;
  case token_table::t_star:           stream << "[star]: ";               break;
  case token_table::t_plus:           stream << "[plus]: ";               break;
  case token_table::t_minus:          stream << "[minus]: ";              break;
  }
  return stream << token.second;
}

class lexer {
public:
  lexer(std::string_view data)
    : is_first_quote_pasted(0)
    , is_open_quote(0)
    , quotes_pasted(0)
    , open_quote_i(0)
    , first_digit_i(0)
    , digit_sequence_length(0)
    , parsed_tokens()
    , source_data(data) { }

  token_t process() {
    for (size_t i = 0; i < source_data.size(); ++i) {
      process_string_literal(i);
      if (is_first_quote_pasted && quotes_pasted % 2 == 0) {
        continue;
      }
      process_keyword(i);
      process_digit(i);
    }
    return parsed_tokens;
  }
private:
  void process_keyword(size_t& i) {
    for (auto& token : known_tokens) {
      if (token.second == source_data.substr(i, token.second.size())) {
        parsed_tokens.emplace_back(token.first, token.second);
        i += (token.second.size() - 1);
      }
    }
  }
  void process_string_literal(size_t i) {
    if (is_open_quote && source_data[i] == '"') {
      if (quotes_pasted % 2 == 0) {
        parsed_tokens.emplace_back(
          token_table::t_string_literal,
          source_data.substr(open_quote_i + 1, i - open_quote_i - 1)
        );
      }
      is_open_quote = false;
      open_quote_i = i;
      ++quotes_pasted;
    }
    if (!is_open_quote && source_data[i] == '"') {
      if (!is_first_quote_pasted) {
        is_first_quote_pasted = true;
      }
      is_open_quote = true;
      open_quote_i = i;
    }
  }
  void process_digit(size_t& i) {
    if (!isdigit(source_data[i])) {
      return;
    }
    if (first_digit_i == 0) {
      first_digit_i = i;
    }
    if (digit_sequence_length == 0) {
      while (true) {
        if (isdigit(source_data[i])) {
          ++i;
          ++digit_sequence_length;
        } else {
          break;
        }
      }
      parsed_tokens.emplace_back(
        token_table::t_digit,
        source_data.substr(first_digit_i, digit_sequence_length)
      );
      digit_sequence_length = 0;
      first_digit_i = 0;
      --i;
    }
  }

  bool      is_first_quote_pasted;
  bool      is_open_quote;
  size_t    quotes_pasted;
  size_t    open_quote_i;

  size_t    first_digit_i;
  size_t    digit_sequence_length;

  token_t   parsed_tokens;
  std::string_view source_data;
};

int main() {
  constexpr std::string_view source_data = R"(
    [\:?:\w+\,[0-9]{3}("dimasik228+++")]
  )";
  lexer lex(source_data);
  for (auto& lexeme : lex.process()) {
    std::cout << lexeme << std::endl;
  }
}
