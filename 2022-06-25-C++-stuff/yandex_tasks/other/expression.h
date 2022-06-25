// Представим арифметическое выражение, содержащее числовые константы и операции
// сложения и умножения, в виде дерева. В листьях этого дерева будут находиться константы,
// а в промежуточных узлах — операции. Вам дан абстрактный базовый класс Expression,
// представляющий из себя такое дерево: https://clck.ru/CdtCp. Вам надо унаследовать от
// него классы-наследники для констант, операции сложения и операции умножения так, чтобы
// приведённый в функции main код (и аналогичные примеры) заработали.
//
// Функции базового класса Evaluate и ToString должны переопределяться в классах-наследниках.
// Evaluate должна вычислять выражение, а ToString возвращать его текстовую запись (как в примере).
// При умножении на сумму запись суммы должна браться в скобки. Никаких особых специальных правил
// оформления нулевых или единичных множителей писать не нужно.

#include <iostream>
#include <memory>
#include <string>

class Const_operation : public Expression {
public:
  Const_operation(int value)
    : value_(std::make_shared<int>(value)) { }
  int Evaluate() const override {
    return *value_;
  }
  std::string ToString() const override {
    return std::to_string(*value_);
  }

private:
  std::shared_ptr<int> value_;
};

class Sum_operation : public Expression {
public:
  Sum_operation(ExpressionPtr && lhs, ExpressionPtr && rhs)
  : lhs_(std::move(lhs)),
    rhs_(std::move(rhs)) { }
  int Evaluate() const override {
    return lhs_->Evaluate() + rhs_->Evaluate();
  }
  std::string ToString() const override {
    return
      lhs_->ToString() + " + " +
      rhs_->ToString();
  }

private:
  ExpressionPtr lhs_, rhs_;
};


class Product_operation : public Expression {
public:
  Product_operation(ExpressionPtr && lhs, ExpressionPtr && rhs)
    : lhs_(std::move(lhs)),
      rhs_(std::move(rhs)),
      lhs_braces_needed(std::dynamic_pointer_cast<Sum_operation>(lhs_)),
      rhs_braces_needed(std::dynamic_pointer_cast<Sum_operation>(rhs_))
  { }

  int Evaluate() const override {
    return lhs_->Evaluate() * rhs_->Evaluate();
  }
  std::string ToString() const override {
    std::string str;

    if (lhs_braces_needed) {
      str += "(" + lhs_->ToString() + ")";
    } else {
      str += lhs_->ToString();
    }
    str += " * ";

    if (rhs_braces_needed) {
      str += "(" + rhs_->ToString() + ")";
    } else {
      str += rhs_->ToString();
    }
    return str;
  }

private:
  ExpressionPtr lhs_, rhs_;
  bool lhs_braces_needed;
  bool rhs_braces_needed;
};

ExpressionPtr Sum(ExpressionPtr lhs, ExpressionPtr rhs) {
  return std::make_shared<Sum_operation>(
   std::move(lhs), std::forward<ExpressionPtr>(rhs));
}

ExpressionPtr Product(ExpressionPtr lhs, ExpressionPtr rhs) {
  return std::make_shared<Product_operation>(std::move(lhs), std::move(rhs));
}

ExpressionPtr Const(int lhs) {
  return std::make_shared<Const_operation>(std::move(lhs));
}
