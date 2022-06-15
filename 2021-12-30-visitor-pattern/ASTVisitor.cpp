#include <iostream>
#include <vector>

//  virtual void Visit(const class ASTBinaryOperator *) const = 0;
//  virtual void Visit(const class ASTBooleanLiteral *) const = 0;
//  virtual void Visit(const class ASTBreakStmt *) const = 0;
//  virtual void Visit(const class ASTCompoundStmt *) const = 0;
//  virtual void Visit(const class ASTContinueStmt *) const = 0;
//  virtual void Visit(const class ASTDoWhileStmt *) const = 0;
//  virtual void Visit(const class ASTFloatingPointLiteral *) const = 0;
//  virtual void Visit(const class ASTForStmt *) const = 0;
//  virtual void Visit(const class ASTIfStmt *) const = 0;
//  virtual void Visit(const class ASTIntegerLiteral *) const = 0;
//  virtual void Visit(const class ASTNode *) const = 0;
//  virtual void Visit(const class ASTReturnStmt *) const = 0;
//  virtual void Visit(const class ASTStringLiteral *) const = 0;
//  virtual void Visit(const class ASTUnaryOperator *) const = 0;
//  virtual void Visit(const class ASTVarDecl *) const = 0;
//  virtual void Visit(const class ASTWhileStmt *) const = 0;

class ASTNode {
public:
  virtual ~ASTNode() noexcept = default;
  virtual void Accept(class ASTVisitor *) = 0;
};

class ASTBinaryOperator : public ASTNode {
public:
  ASTBinaryOperator(ASTNode *lhs, ASTNode *rhs) : LHS(lhs), RHS(rhs) {}

  void Accept(class ASTVisitor *) override;

  ASTNode *LHS, *RHS;
};

class ASTBooleanLiteral : public ASTNode {
public:
  ASTBooleanLiteral(bool boolean) : Boolean(boolean) {}

  void Accept(class ASTVisitor *) override;

  bool Boolean;
};

class ASTVisitor {
public:
  virtual ~ASTVisitor() noexcept = default;
  virtual void Visit(const class ASTNode *) = 0;
  virtual void Visit(const class ASTBinaryOperator *) = 0;
  virtual void Visit(const class ASTBooleanLiteral *) = 0;
};

void ASTBinaryOperator::Accept(class ASTVisitor* V) {
  V->Visit(this);
}

void ASTBooleanLiteral::Accept(class ASTVisitor* V) {
  V->Visit(this);
}

class ASTPrintVisitor : public ASTVisitor {
public:
  void Visit(const ASTBinaryOperator *Binary) override {
    std::cout << "Binary:\n";
    ++Indent;
    PrintIndent();
    Visit(Binary->LHS);
    PrintIndent();
    Visit(Binary->RHS);
    --Indent;
  }

  void Visit(const ASTBooleanLiteral *Boolean) override {
    std::cout << "Boolean " << std::boolalpha << Boolean->Boolean << std::endl;
  }

  void Visit(const ASTNode *Node) override {
    if (auto Binary = dynamic_cast<const ASTBinaryOperator *>(Node))
      Visit(Binary);
    if (auto Boolean = dynamic_cast<const ASTBooleanLiteral *>(Node))
      Visit(Boolean);
  }

  void PrintIndent() { std::cout << std::string(Indent, ' '); }

  int Indent = 0;
};

int main() {
  (new ASTBinaryOperator(
    new ASTBinaryOperator(
      new ASTBinaryOperator(
        new ASTBinaryOperator(
          new ASTBinaryOperator(
            new ASTBinaryOperator(
              new ASTBooleanLiteral(true),
              new ASTBooleanLiteral(true)
            ),
            new ASTBooleanLiteral(true)
          ),
          new ASTBinaryOperator(
            new ASTBooleanLiteral(true),
            new ASTBooleanLiteral(true)
          )
        ),
        new ASTBooleanLiteral(true)
      ),
      new ASTBooleanLiteral(true)
    ),
    new ASTBooleanLiteral(true)
  ))->Accept(new ASTPrintVisitor);
}
