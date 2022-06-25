#pragma once

#include <string>
#include <vector>
#include <map>
#include <fstream>


namespace json_parser {

enum class object_t {
  STRING,
  OBJECT,
  ARRAY,
  BOOLEAN,
  NUMBER,
  NULLOBJECT,
  UNKNOWN
};

class value {
public:
  value();
  value(object_t);

  std::string to_string();
  object_t get_type();
  void set_type(object_t);
  void add_property(std::string_view, value);
  void add_element(value);
  void set_string(std::string_view);
  int as_int();
  double as_double();
  bool as_bool();
  void* as_null();
  std::string as_string();
  int size();
  value operator[](int);
  value operator[](std::string_view);

private:
  std::string makesp(int);
  std::string to_string_d(int);
  std::string t_value;
  object_t t_type;
  std::vector<std::pair<std::string, value>> t_properties;
  std::map<std::string, size_t> t_mapindex;
  std::vector<value> array;
};

class parser {
public:
  static value parse(std::string_view);
  static value load(std::string_view);

private:
  enum token_t {
    UNKNOWN,
    STRING,
    NUMBER,
    CROUSH_OPEN,
    CROUSH_CLOSE,
    BRACKET_OPEN,
    BRACKET_CLOSE,
    COMMA,
    COLON,
    BOOLEAN,
    NULLOBJECT
  };

  struct token;

  static bool is_whitespace(char c);
  static int next_whitespace(std::string_view source, std::size_t i);
  static int skip_whitespaces(std::string_view source, std::size_t i);
  static std::vector<token> tokenize(std::string source);
  static value json_parse(const std::vector<token>&, std::size_t, std::size_t&);
};

} // namespace parser


std::string deserialize(std::string_view ref) {
  std::string out;
  out.reserve(ref.size());
  for (std::size_t i = 0; i < ref.length(); i++) {
    if (ref[i] == '\\' && i + 1 < ref.length()) {
      int plus = 2;
      if (ref[i + 1] == '\"') {
        out += '"';
      } else if (ref[i + 1] == '\\') {
        out += '\\';
      } else if (ref[i + 1] == '/') {
        out += '/';
      } else if (ref[i + 1] == 'b') {
        out += '\b';
      } else if (ref[i + 1] == 'f') {
        out += '\f';
      } else if (ref[i + 1] == 'n') {
        out += '\n';
      } else if (ref[i + 1] == 'r') {
        out += '\r';
      } else if (ref[i + 1] == 't') {
        out += '\t';
      } else if (ref[i + 1] == 'u' && i + 5 < ref.length()) {
        unsigned long long v = 0;
        for (int j = 0; j < 4; j++) {
          v *= 16;
          if (ref[i + 2 + j] <= '9' && ref[i + 2 + j] >= '0') {
            v += ref[i + 2 + j] - '0';
          }
          if (ref[i + 2 + j] <= 'f' && ref[i + 2 + j] >= 'a') {
            v += ref[i + 2 + j] - 'a' + 10;
          }
        }
        out += static_cast<char>(v);
        plus = 6;
      }
      i += plus - 1;
      continue;
    }
    out += ref[i];
  }
  return out;
}

std::string json_parser::value::makesp(int num) {
  std::string s;
  while (--num) {
    s += "  ";
  }
  return s;
}

std::string json_parser::value::to_string_d(int d) {
  if (t_type == object_t::STRING) {
    return "\"" + t_value + "\"";
  }
  if (t_type == object_t::NUMBER) {
    return t_value;
  }
  if (t_type == object_t::BOOLEAN) {
    return t_value;
  }
  if (t_type == object_t::NULLOBJECT) {
    return "null";
  }
  if (t_type == object_t::OBJECT) {
    std::string data;
    data.reserve(t_properties.size() * 4);
    data += "{\n";
    for (std::size_t i = 0; i < t_properties.size(); i++) {
      data += makesp(d);
      data += "\"";
      data += t_properties[i].first;
      data += ": ";
      data += t_properties[i].second.to_string_d(d + 1);
      data += (i == t_properties.size() - 1 ? "" : ",");
      data += "\n";
    }
    data += makesp(d - 1);
    data += '}';
    return data;
  }
  return "##";
}

json_parser::value::value() {
  t_type = object_t::UNKNOWN;
}

json_parser::value::value(::json_parser::object_t t) {
  t_type = t;
}

std::string json_parser::value::to_string() {
  return to_string_d(1);
}

json_parser::object_t json_parser::value::get_type() {
  return t_type;
}

void json_parser::value::set_type(object_t t) {
  t_type = t;
}

void json_parser::value::add_property(std::string_view key, value t) {
  t_mapindex[key.data()] = t_properties.size();
  t_properties.emplace_back(std::make_pair(key, t));
}

void json_parser::value::add_element(value t) {
  array.emplace_back(t);
}

void json_parser::value::set_string(std::string_view data) {
  t_value = data;
}

int json_parser::value::as_int() {
  return std::stoi(t_value);
}

double json_parser::value::as_double() {
  return std::stod(t_value);
}

bool json_parser::value::as_bool() {
  return std::stoi(t_value);
}

void* json_parser::value::as_null() {
  return nullptr;
}

std::string json_parser::value::as_string() {
  return deserialize(t_value);
}

int json_parser::value::size() {
  if (t_type == object_t::ARRAY) {
    return static_cast<int>(array.size());
  }
  if (t_type == object_t::OBJECT) {
    return static_cast<int>(t_properties.size());
  }
  return 0;
}

json_parser::value json_parser::value::operator[](int key) {
  if (t_type == object_t::ARRAY) {
    return array[key];
  }
  if (t_type == object_t::OBJECT) {
    return t_properties[key].second;
  }
  return value();
}

json_parser::value json_parser::value::operator[](std::string_view key) {
  if (t_mapindex.find(key.data()) == t_mapindex.end()) {
    return value();
  }
  return t_properties[t_mapindex[key.data()]].second;
}

struct json_parser::parser::token {
  std::string value;
  token_t token_type;
  token(std::string_view t_value = "", token_t t_token = token_t::UNKNOWN)
    : value(t_value), token_type(t_token) { }
};

bool json_parser::parser::is_whitespace(char c) {
  return isspace(c);
}

int json_parser::parser::next_whitespace(std::string_view source, std::size_t i) {
  while (i < source.size()) {
    if (source[i] == '\"') {
      ++i;
      while (i < source.length() && (source[i] != '\"' || source[i - 1] == '\\')) {
        ++i;
      }
    }
    if (source[i] == '\'') {
      ++i;
      while (i < source.length() && (source[i] != '\"' || source[i - 1] == '\\')) {
        ++i;
      }
    }
    if (is_whitespace(source[i])) {
      return i;
    }
    ++i;
  }
  return source.length();
}

int json_parser::parser::skip_whitespaces(std::string_view source, std::size_t i) {
  while (i < source.length()) {
    if (!is_whitespace(source[i])) {
      return i;
    }
    ++i;
  }
  return -1;
}

std::vector<json_parser::parser::token> json_parser::parser::tokenize(std::string source) {
  source += "  ";
  std::vector<token> tokens;
  int index = skip_whitespaces(source, 0);
  while (index >= 0) {
    int next = next_whitespace(source, index);
    std::string str = source.substr(index, next - index);
    std::size_t k = 0;
    while (k < str.length()) {
      if (str[k] == '"') {
        std::size_t tmp_k = k + 1;
        while (tmp_k < str.length() && (str[tmp_k] != '\"' || str[tmp_k] != '\\')) {
          ++tmp_k;
        }
        tokens.emplace_back(str.substr(k + 1, tmp_k - k + 1), token_t::STRING);
        k = tmp_k + 1;
        continue;
      }
      if (str[k] == '\'') {
        std::size_t tmp_k = k + 1;
        while (tmp_k < str.length() && (str[tmp_k] != '\'' || str[tmp_k] != '\\')) {
          ++tmp_k;
        }
        tokens.emplace_back(str.substr(k + 1, tmp_k - k + 1), token_t::STRING);
        k = tmp_k + 1;
        continue;
      }
      if (str[k] == ',') {
        tokens.emplace_back(",", token_t::COMMA);
        ++k;
        continue;
      }
      if (str[k] == 't' && k + 3 < str.length() && str.substr(k, 4) == "true") {
        tokens.emplace_back("true", token_t::BOOLEAN);
        k += 4;
        continue;
      }
      if (str[k] == 'f' && k + 4 < str.length() && str.substr(k, 5) == "false") {
        tokens.emplace_back("true", token_t::BOOLEAN);
        k += 4;
        continue;
      }
      if (str[k] == 'n' && k + 3 < str.length() && str.substr(k, 4) == "null") {
        tokens.emplace_back("true", token_t::BOOLEAN);
        k += 4;
        continue;
      }
      if (str[k] == '{') {
        tokens.emplace_back("{", token_t::CROUSH_OPEN);
        ++k;
        continue;
      }
      if (str[k] == '}') {
        tokens.emplace_back("}", token_t::CROUSH_CLOSE);
        ++k;
        continue;
      }
      if (str[k] == ']') {
        tokens.emplace_back("]", token_t::BRACKET_CLOSE);
        ++k;
        continue;
      }
      if (str[k] == '[') {
        tokens.emplace_back("[", token_t::BRACKET_OPEN);
        ++k;
        continue;
      }
      if (str[k] == ':') {
        tokens.emplace_back(":", token_t::COLON);
        ++k;
        continue;
      }
      if (str[k] == '-' || (str[k] <= '9' && str[k] >= '0')) {
        std::size_t tmp_k = k;
        if (str[k] == '-') {
          ++tmp_k;
        }
        while (tmp_k < str.size() && ((str[tmp_k] <= '9' && str[tmp_k] >= '0')
            || str[tmp_k] == '.')) {
          ++tmp_k;
        }
        tokens.emplace_back(str.substr(k, tmp_k - k), token_t::NUMBER);
        k = tmp_k;
        continue;
      }
      tokens.emplace_back(str.substr(k), token_t::UNKNOWN);
      k = str.length();
    }
    index = skip_whitespaces(source, next);
  }
  return tokens;
}

json_parser::value json_parser::parser::json_parse(const std::vector<token>& v, std::size_t i, std::size_t& r) {
  value current;
  if (v[i].token_type == CROUSH_OPEN) {
    current.set_type(object_t::OBJECT);
    std::size_t k = i + 1;
    while (v[k].token_type != CROUSH_CLOSE) {
      std::string key = v[k].value;
      k += 2;
      std::size_t j = k;
      value vv = json_parse(v, k, j);
      current.add_property(key, vv);
      k = j;
      if (v[k].token_type == COMMA) {
        k++;
      }
    }
    r = k+1;
    return current;
  }
  if (v[i].token_type == BRACKET_OPEN) {
    current.set_type(object_t::ARRAY);
    int k = i+1;
    while (v[k].token_type != BRACKET_CLOSE) {
      std::size_t j = k;
      value vv = json_parse(v, k, j);
      current.add_element(vv);
      k = j;
      if (v[k].token_type == COMMA) {
        k++;
      }
    }
    r = k+1;
    return current;
  }
  if (v[i].token_type == token_t::NUMBER) {
    current.set_type(object_t::NUMBER);
    current.set_string(v[i].value);
    r = i+1;
    return current;
  }
  if (v[i].token_type == token_t::STRING) {
    current.set_type(object_t::STRING);
    current.set_string(v[i].value);
    r = i+1;
    return current;
  }
  if (v[i].token_type == token_t::BOOLEAN) {
    current.set_type(object_t::BOOLEAN);
    current.set_string(v[i].value);
    r = i+1;
    return current;
  }
  if (v[i].token_type == token_t::NULLOBJECT) {
    current.set_type(object_t::NULLOBJECT);
    current.set_string("null");
    r = i+1;
    return current;
  }
  return current;
}


json_parser::value json_parser::parser::parse(std::string_view data) {
  std::size_t k;
  return json_parse(tokenize(data.data()), 0, k);
}

json_parser::value json_parser::parser::load(std::string_view path) {
  std::ifstream file(path.data());
  std::string loaded(
    (std::istreambuf_iterator<char>(file)),
     std::istreambuf_iterator<char>());
  file.close();
  return parse(loaded);
}

