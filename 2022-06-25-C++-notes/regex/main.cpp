#include <iostream>
#include <vector>
#include <cstring>

using std::string;
using std::vector;

class regex {
private:
  const char* unmatched;
public:
  regex(const char* text) : unmatched(text) { }

  bool match(const char* pattern) {
    std::cout << "\npattern: " << pattern << std::endl;
      if (pattern[0] == '\0') {
        return true;
      }
      if (pattern[0] != '\\' &&
          pattern[0] != '+' &&
          pattern[0] != '+' &&
          pattern[0] != '.' &&
          pattern[0] != '?' &&
          pattern[0] != '*') {
        return strstr(unmatched, pattern);
      }
      if (pattern[0] == '\\') {
        switch (pattern[1]) {
          case 'd':
            do {
              if (isdigit(*unmatched++)) {
                return true;
              }
            } while (*unmatched != '\0');
            break;

          case 'D':
            do {
              if (!isdigit(*unmatched++)) {
                return true;
              }
            } while (*unmatched != '\0');
            break;

          case 's':
            do {
              if (isspace(*unmatched++)) {
                return true;
              }
            } while (*unmatched != '\0');
            break;

          case 'S':
            do {
              if (!isspace(*unmatched++)) {
                return true;
              }
            } while (*unmatched != '\0');
            break;
        }
      }
//    return false;
      std::cout << "ret\n";
      return match(pattern+=2);
      //косяк с возвращением true и выходом из функции
  }
};

int main(void) {
  regex r("");
  std::cout << "matched: " << r.match("\\d\\s\\d") << std::endl;
}

//#include <iostream>
//#include <vector>
//#include <cstring>

//#define watch(x) (std::cout << #x << " is: " << x << std::endl)

//using std::string;
//using std::vector;

//class Regex {
//private:
//  static bool match_group(char, int);
//  static bool match_quantity(const char* regex, char* s, int quant, int c, int* len);
//public:
//  static bool match(const char* regex, char* str);
//};

//bool Regex::match(const char *regex, char *str) {
//  char c = regex[0];
//  if (regex[0] == '\0') {
//    return true;
//  }
//  if (regex[0] == '\\' and regex[0] != '\0') {
//    c = regex[1];
//    watch(c);
//    if (c != '\\' and c != '^' and c != '$' and c != '+' and c != '*' and c != '?') {
//      c |= 0x100;
//    }
//    regex++;
//  }
//  char q = regex[1];
//  if (q == '*' or q == '+' or q == '-' or q == '?') {
//    int len = strlen(str);
//    watch(regex);
//    watch(str);
//    watch(q);
//    return match_quantity(regex+2, str, q, c, &len);
//  } else if (match_group(*str, c)) {
//    return match(++regex, ++str);
//  }
//  return 0;
//}

//bool Regex::match_group(char c, int group) {
//  if ((group & 0xFF) == '.') {
//    group ^= 0x100;
//  }
//  if (group < 0x100) {
//    return c == group;
//  }
//  switch (group & 0xFF) {
//    case '.': return true;
//    case 'd': return isdigit(c);
//    case 'D': return not isdigit(c);
//    case 's': return isspace(c);
//    case 'S': return not isspace(c);
//    case 'w': return (isdigit(c) or isalpha(c));
//    case 'W': return not (isdigit(c) or isalpha(c));
//  }
//  return 0;
//}

//bool Regex::match_quantity(const char* regex, char* s, int quant, int c, int* len) {
//  if (quant == '?') {
//    if (match_group(*s, c)) {
//      ++len;
//      ++s;
//    }
//    return match(regex, s);
//  }

//  if (quant == '+' or quant == '*') {
//    char* p;
//    for (p = s; *p != '\0' && match_group(*p, c); ++p) {
//      ++len;
//    }

//    if (quant == '+' && p == s) {
//      return 0;
//    }

//    do {
//      if (match(regex, p)) {
//        return 1;
//      }
//    } while(p-- > s);
//  }
//  return 0;
//}


//int main(void) {
//  char text[] = "1";
//  std::cout << Regex::match("..", text) << std::endl;
//}






