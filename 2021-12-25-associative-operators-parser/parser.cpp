#include <bits/stdc++.h>

int main() {
  enum struct AstType { Number, Operator };
  struct Node {
    virtual ~Node() noexcept = default;
    virtual void Print() = 0;
  };
  struct Number : Node {
    int Value;
    
    Number(int V) : Value(V) {}

    void Print() override { std::cout << Value; }
  };
  struct Binary : Node {
    char Op;
    std::unique_ptr<Node> Left, Right;

    Binary(char O, std::unique_ptr<Node> L, std::unique_ptr<Node> R)
        : Op(O), Left(std::move(L)), Right(std::move(R)) {}

    void Print() override {
      std::cout << "(";
      Left->Print();
      std::cout << " " << Op << " ";
      Right->Print();
      std::cout << ")";
    }
  };

  struct Parser {
    std::string_view Input;
    unsigned PeekIndex;

    Parser(std::string_view I) : Input(I), PeekIndex(0) { Additive()->Print(); }

    std::unique_ptr<Node> Additive() {
      auto Expression = Multiplicative();
      while (true) {
        if (MatchOperator('+')) {
          Expression = std::make_unique<Binary>('+', std::move(Expression), Multiplicative());
          continue;
        }
        break;
      }
      return Expression;
    }

    std::unique_ptr<Node> Multiplicative() {
      auto Expression = Term();
      while (true) {
        if (MatchOperator('*')) {
          Expression = std::make_unique<Binary>('*', std::move(Expression), Multiplicative());
          continue;
        }
        break;
      }
      return Expression;
    }

    std::unique_ptr<Node> Term() {
      char Token = Peek();
      if (!isdigit(Token))
        throw std::runtime_error("Number expected, got: " + std::string{Token});
      return std::make_unique<Number>(Token - '0');
    }

    char Peek() { return Input.at(PeekIndex++); }

    bool MatchOperator(char Op) {
      if (PeekIndex >= Input.length())
        return false;
      if (Input.at(PeekIndex) == Op) {
        ++PeekIndex;
        return true;
      }
      return false;
    }
  } parser("1+2*3+4+5+6*7+9+1*9*9+9*9");
}
