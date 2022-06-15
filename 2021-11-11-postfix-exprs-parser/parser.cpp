#include <iostream>

class parser {
public:
  parser() {
    std::cin >> lookahead_;
  }

  void expr() {
    term();
    while (true) {
      switch (lookahead_) {
      	case '+': {
      	  match('+'); term(); std::cout << '+'; break;
      	}
      	case '-': {
      	  match('-'); term(); std::cout << '-'; break;
      	}
      	default: return;
      }
    }
  }

private:
  void term() {
    if (std::isdigit(lookahead_)) {
      std::cout << lookahead_;
      match(lookahead_);
    } else {
      throw std::runtime_error("term: syntax error");
    }
  }

  void match(int t) {
    if (lookahead_ == t) {
      std::cin >> lookahead_;
    } else {
      throw std::runtime_error("match: syntax error");
    }
  }

  char lookahead_;
};

int main() {
  while (true) {
    std::cout << ">>> ";
    parser parse;
    parse.expr();
    std::cout << std::endl;
  }
}
