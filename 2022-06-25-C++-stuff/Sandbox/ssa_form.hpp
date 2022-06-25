// Mar 23, 2022
#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <fstream>
#include <memory>

/*****************************************************/
/* AST                                               */
/*****************************************************/

class ASTNumber;
class ASTSymbol;
class ASTAssign;
class ASTIf;
class ASTFor;
class ASTBinary;
class ASTNode;
class IRNode;
class IRBranch;
class IRAssignment;

class ASTVisitor {
public:
  virtual void Visit(ASTNumber *Expr) = 0;
  virtual void Visit(ASTSymbol *Expr) = 0;
  virtual void Visit(ASTAssign *Expr) = 0;
  virtual void Visit(ASTIf *Expr) = 0;
  virtual void Visit(ASTFor *Expr) = 0;
  virtual void Visit(ASTBinary *Expr) = 0;
};

class IRVisitor {
public:
  virtual void Visit(IRBranch *) = 0;
  virtual void Visit(IRAssignment *) = 0;
};

class ASTNode {
public:
  virtual ~ASTNode() = default;

  virtual std::string StringValue() { return ""; }

  virtual void Accept(ASTVisitor *Visitor) = 0;
};

class ASTNumber : public ASTNode {
public:
  int Value;

  ASTNumber(int value) : Value(value) {}

  std::string StringValue() override {
    return std::to_string(Value);
  }

  void Accept(ASTVisitor *visitor) override {
    visitor->Visit(this);
  }
};

class ASTSymbol : public ASTNode {
public:
  std::string name;

  ASTSymbol(const std::string &name) : name(name), SSAIndex(0) {}

  void SetSSAIndex(int index) { SSAIndex = index; };

  std::string StringValue() override {
    return name + "#" + std::to_string(SSAIndex);
  }

  void Accept(ASTVisitor *visitor) override {
    visitor->Visit(this);
  }

private:
  int SSAIndex;
};

class ASTAssign : public ASTNode {
public:
  ASTSymbol *varExp;
  ASTNode *expr;

  ASTAssign(ASTSymbol *varExp, ASTNode *expr): varExp(varExp), expr(expr) {}

  std::string varName() {
    return varExp->name;
  }

  void Accept(ASTVisitor *visitor) override {
    visitor->Visit(this);
  }
};

class ASTIf : public ASTNode {
public:
  ASTNode *conditionExp, *thenExp, *elseExp;

  ASTIf(ASTNode *conditionExp, ASTNode *thenExp, ASTNode *elseExp)
    : conditionExp(conditionExp), thenExp(thenExp), elseExp(elseExp)  {}

  void Accept(ASTVisitor *visitor) override {
    visitor->Visit(this);
  }
};

class ASTFor: public ASTNode {
public:
  ASTSymbol *index;
  ASTNode *start, *end, *step, *body;

  ASTFor(std::string indexName, ASTNode *start, ASTNode *end, ASTNode *step, ASTNode *body)
    : index(new ASTSymbol(indexName)), start(start), end(end), step(step), body(body) {}

  void Accept(ASTVisitor *visitor) override {
    visitor->Visit(this);
  }
};

class ASTBinary: public ASTNode {
public:
  char op;
  ASTNode *lhs, *rhs;

  ASTBinary(char op, ASTNode *lhs, ASTNode *rhs): op(op), lhs(lhs), rhs(rhs) {}

  std::string StringValue() override {
    return lhs->StringValue() + " " + op + " " + rhs->StringValue();
  }

  void Accept(ASTVisitor *visitor) override {
    visitor->Visit(this);
  }
};

/*****************************************************/
/* Basic CFG Block                                   */
/*****************************************************/

class CFGBlock {
public:
  CFGBlock(int TheIndex, std::string TheLabel)
    : Index(TheIndex), Label(std::move(TheLabel)) {}

  std::string StringValue();

  void AddStatement(std::unique_ptr<IRNode> &&);
  static void AddLink(CFGBlock *Pred, CFGBlock *Succ);

  std::vector<std::unique_ptr<IRNode>> Statements;
  std::vector<CFGBlock *> Succs;
  std::vector<CFGBlock *> Preds;

  CFGBlock *Dominator;
  std::vector<CFGBlock *> DominatingBlocks;

private:
  int Index;
  std::string Label;
};

std::string CFGBlock::StringValue() {
  return "CFG#" + std::to_string(Index) + "(" + Label + ")";
}

void CFGBlock::AddStatement(std::unique_ptr<IRNode> &&Statement) {
  Statements.push_back(std::move(Statement));
}

void CFGBlock::AddLink(CFGBlock *Pred, CFGBlock *Succ) {
  if (std::find(Succ->Preds.begin(), Succ->Preds.end(), Pred) == Succ->Preds.end())
    Succ->Preds.push_back(Pred);
  if (std::find(Pred->Succs.begin(), Pred->Succs.end(), Succ) == Pred->Succs.end())
    Pred->Succs.push_back(Succ);
}

/*****************************************************/
/* Control Flow Graph                                */
/*****************************************************/

class CFG {
public:
  std::vector<CFGBlock *> BasicBlocks;
  std::set<CFGBlock *> GetDominanceFrontierForSubset(const std::set<CFGBlock*> &);
  void CommitAllChanges();
  void AddBlock(CFGBlock* Block);

private:
  void PredOrderDFS(CFGBlock *);
  void PostOrderDFS(CFGBlock *);
  void DomDFS(CFGBlock *);

  void ComputePredOrder();
  void ComputePostOrder();
  void ComputeDominatorTree();
  void ComputeBaseDominanceFrontier();
  std::set<CFGBlock *> GetMergedDominanceFrontierFromSubset(const std::set<CFGBlock*> &);

  std::vector<CFGBlock *> InPredOrder;
  std::vector<CFGBlock *> InPostOrder;
  std::map<CFGBlock *, int> VisitedMap;
  std::map<CFGBlock *, std::set<CFGBlock *>> DominanceFrontier;
};

void CFG::AddBlock(CFGBlock *Block) {
  BasicBlocks.push_back(Block);
}

void CFG::ComputePredOrder() {
  if (InPredOrder.empty()) {
    VisitedMap.clear();
    PredOrderDFS(BasicBlocks.front());
  }
}

void CFG::PredOrderDFS(CFGBlock *Block) {
  VisitedMap[Block] = 1;
  InPredOrder.push_back(Block);
  for (auto *Next : Block->Succs)
    if (!VisitedMap[Next])
      PredOrderDFS(Next);
}

void CFG::ComputePostOrder() {
  if (InPostOrder.empty()) {
    VisitedMap.clear();
    PostOrderDFS(BasicBlocks.front());
  }
}

void CFG::PostOrderDFS(CFGBlock *Block) {
  VisitedMap[Block] = 1;
  for (auto *Next : Block->Succs)
    if (!VisitedMap[Next])
      PostOrderDFS(Next);
  InPostOrder.push_back(Block);
}

void CFG::ComputeDominatorTree() {
  if (!InPredOrder.empty())
    ComputePredOrder();

  for (auto *It : InPredOrder) {
    VisitedMap.clear();
    VisitedMap[It] = 1;
    DomDFS(InPredOrder.front());
    for (auto *Jt : InPredOrder)
      if (!VisitedMap[Jt])
        Jt->Dominator = It;
  }

  for (auto *V : BasicBlocks) {
    if (CFGBlock *Dominator = V->Dominator)
      Dominator->DominatingBlocks.push_back(V);
  }
}

void CFG::DomDFS(CFGBlock *Block) {
  if (!VisitedMap[Block]) {
    VisitedMap[Block] = 1;
    for (auto *Next : Block->Succs)
      if (!VisitedMap[Next])
        DomDFS(Next);
  }
}

void CFG::ComputeBaseDominanceFrontier() {
  if (!DominanceFrontier.empty())
    return;

  for (auto *Block : BasicBlocks)
    DominanceFrontier[Block];

  for (auto *X : InPostOrder) {
    for (auto *Y : X->Succs)
      if (Y->Dominator != X)
        DominanceFrontier.at(X).insert(Y);

    for (auto *Z : X->DominatingBlocks)
      for (auto *Y : DominanceFrontier.at(Z))
        if (Y->Dominator != X)
          DominanceFrontier.at(X).insert(Y);
  }
}

std::set<CFGBlock *> CFG::GetMergedDominanceFrontierFromSubset(const std::set<CFGBlock*> &Subset) {
  if (DominanceFrontier.empty())
    ComputeBaseDominanceFrontier();

  std::set<CFGBlock *> MergedDF;

  for (auto *V : Subset) {
    std::set<CFGBlock *> DF = DominanceFrontier.at(V);
    MergedDF.insert(DF.begin(), DF.end());
  }

  return MergedDF;
}

std::set<CFGBlock *> CFG::GetDominanceFrontierForSubset(const std::set<CFGBlock*> &Subset) {
  std::set<CFGBlock *> Result;
  std::set<CFGBlock *> DFP = GetMergedDominanceFrontierFromSubset(Subset);
  bool HasChanged = true;

  while (HasChanged) {
    HasChanged = false;
    DFP.insert(Subset.begin(), Subset.end());
    DFP = GetMergedDominanceFrontierFromSubset(DFP);
    if (Result != DFP) {
      Result = DFP;
      HasChanged = true;
    }
  }

  return Result;
}

void CFG::CommitAllChanges() {
  ComputePredOrder();
  ComputePostOrder();
  ComputeDominatorTree();
  ComputeBaseDominanceFrontier();
}

/*****************************************************/
/* IR statements                                     */
/*****************************************************/

enum StatementType { ASSIGN = 0, BRANCH = 1, PHI = 2 };

class IRNode {
public:
  virtual ~IRNode() = default;

  StatementType Type;

  IRNode(StatementType TheType) : Type(TheType) {}

  virtual std::string Dump() = 0;
  virtual void Accept(IRVisitor *) = 0;
};

class IRAssignment : public IRNode {
public:
  IRAssignment(ASTSymbol *TheVariable, ASTNode *TheRHS)
    : IRNode(ASSIGN), Variable(TheVariable), RHS(TheRHS) {}

  std::string Dump() override {
    return Variable->StringValue() + " = " + RHS->StringValue();
  }

  void Accept(IRVisitor *Visitor) override {
    Visitor->Visit(this);
  }

  ASTSymbol *Variable;
  ASTNode *RHS;
};

class IRBranch : public IRNode {
public:

  std::string Dump() override {
    std::string Result = "Branch";

    if (IsConditional)
      Result += " (" + Condition->StringValue() + ")";

    Result += " on true to " + FirstBranchBlock->StringValue();

    if (IsConditional)
      Result += ", on false to " + SecondBranchBlock->StringValue();

    return Result;
  }

  IRBranch(ASTNode *Cond, CFGBlock *TrueBranch, CFGBlock *FalseBranch)
    : IRBranch(Cond, TrueBranch, FalseBranch, true) {}

  IRBranch(CFGBlock *Block)
    : IRBranch(nullptr, Block, nullptr, false) {}

  void Accept(IRVisitor *Visitor) override {
    Visitor->Visit(this);
  }

  bool IsConditional;
  CFGBlock *FirstBranchBlock;
  CFGBlock *SecondBranchBlock;
  ASTNode *Condition;

private:
  IRBranch(ASTNode *Cond, CFGBlock *FirstBranch,
           CFGBlock *SecondBranch, bool IsCond)
    : IRNode(BRANCH), IsConditional(IsCond),
      FirstBranchBlock(FirstBranch), SecondBranchBlock(SecondBranch),
      Condition(Cond) {}
};

class IRPhiNode : public IRNode {
public:
  IRPhiNode(std::unique_ptr<ASTSymbol> &&Var, std::map<CFGBlock *, ASTSymbol *> VarMap)
    : IRNode(PHI), Variable(std::move(Var)), BlockToVarMap(VarMap) {}

  std::string Dump() override {
    std::string Arguments;
    for (auto [Block, Symbol] : BlockToVarMap)
      Arguments += Block->StringValue() + ":" + Symbol->StringValue() + ", ";

    if (!Arguments.empty()) {
      Arguments.pop_back();
      Arguments.pop_back();
    }

    return Variable->StringValue() + " = φ(" + Arguments + ")";
  }

  void Accept(IRVisitor *) override {}

  std::unique_ptr<ASTSymbol> Variable;
  std::map<CFGBlock *, ASTSymbol *> BlockToVarMap;
};

/*****************************************************/
/* Variable Search Visitor                           */
/*****************************************************/

/// This used for getting all variables in AST or IR statement recursively.
class VariableSearchVisitor : public ASTVisitor, public IRVisitor {
public:
  void Visit(ASTNumber *) override {}
  void Visit(ASTIf *) override {}
  void Visit(ASTFor *) override {}
  void Visit(ASTSymbol *) override;
  void Visit(ASTAssign *) override;
  void Visit(ASTBinary *) override;
  void Visit(IRBranch *) override;
  void Visit(IRAssignment *) override;
  std::set<ASTSymbol *> AllVarsUsedInStatement(IRNode *);

private:
  std::set<ASTSymbol *> Variables;
};

std::set<ASTSymbol *> VariableSearchVisitor::AllVarsUsedInStatement(IRNode *Stmt) {
  Variables.clear();
  Stmt->Accept(this);
  return Variables;
}

void VariableSearchVisitor::Visit(IRBranch *Stmt) {
  if (Stmt->Condition)
    Stmt->Condition->Accept(this);
}
void VariableSearchVisitor::Visit(IRAssignment *Stmt) {
  Stmt->RHS->Accept(this);
}
void VariableSearchVisitor::Visit(ASTSymbol *Stmt) {
  Variables.insert(Stmt);
}
void VariableSearchVisitor::Visit(ASTAssign *Stmt) {
  Variables.insert(Stmt->varExp);
  Stmt->expr->Accept(this);
}
void VariableSearchVisitor::Visit(ASTBinary *Stmt) {
  Stmt->lhs->Accept(this);
  Stmt->rhs->Accept(this);
}

/*****************************************************/
/* SSA builder                                       */
/*****************************************************/

class SSABuilder {
public:
  SSABuilder(CFG *Graph) : CFGraph(Graph) {}

  void RenameVarToSSAForm(std::string VarName);

private:
  int Counter;
  std::vector<int> Stack;
  VariableSearchVisitor VarSearcher;

  void TraverseBlockWithVar(CFGBlock *Block, std::string VarName);

  CFG *CFGraph;
};

void SSABuilder::RenameVarToSSAForm(std::string VarName) {
  Counter = 0;
  Stack.clear();
  TraverseBlockWithVar(CFGraph->BasicBlocks.front(), VarName);
}

void SSABuilder::TraverseBlockWithVar(CFGBlock *Block, std::string VarName) {
  for (const auto &Stmt : Block->Statements) {
    if (Stmt->Type == PHI) {
      IRPhiNode *Phi = static_cast<IRPhiNode *>(Stmt.get());
      if (Phi->Variable->name == VarName) {
        Phi->Variable->SetSSAIndex(Counter);
        Stack.push_back(Counter++);
      }
    }
    else {
      auto Vars = VarSearcher.AllVarsUsedInStatement(Stmt.get());
      for (auto *Var : Vars)
        if (Var->name == VarName)
          Var->SetSSAIndex(Stack.back());
    }

    if (Stmt->Type == ASSIGN) {
      IRAssignment *Assignment = static_cast<IRAssignment *>(Stmt.get());
      if (Assignment->Variable->name == VarName) {
        Assignment->Variable->SetSSAIndex(Counter);
        Stack.push_back(Counter++);
      }
    }
  }

  for (auto *Succ : Block->Succs) {
    for (const auto &Stmt : Succ->Statements) {
      if (Stmt->Type == PHI) {
        IRPhiNode *Phi = static_cast<IRPhiNode *>(Stmt.get());
        if (Phi->BlockToVarMap.count(Block) && Phi->BlockToVarMap[Block]->name == VarName)
          Phi->BlockToVarMap[Block]->SetSSAIndex(Stack.back());
      }
    }
  }

  for (auto *Child : Block->DominatingBlocks)
    TraverseBlockWithVar(Child, VarName);

  for (const auto &Stmt : Block->Statements) {
    if (Stmt->Type == ASSIGN) {
      IRAssignment *Assignment = static_cast<IRAssignment *>(Stmt.get());
      if (Assignment->Variable->name == VarName)
        Stack.pop_back();
    }
  }
}

/*****************************************************/
/* CFG Builder                                       */
/*****************************************************/

class CFGBuilder : public ASTVisitor {
public:
  CFGBuilder();

  void Visit(ASTNumber *) override {}
  void Visit(ASTSymbol *) override {}
  void Visit(ASTFor *) override {}
  void Visit(ASTAssign *) override;
  void Visit(ASTIf *) override;
  void Visit(ASTBinary *) override;

  void Traverse(ASTNode *);
  void CommitSSAFormBuilding();

  CFG *GetCFG() {
    return CFGraph.get();
  }

  std::map<std::string, std::set<CFGBlock *>> BlocksForVariable;

private:
  CFGBlock *CreateBlock(std::string Label);
  void CreateBranch(CFGBlock *Target);
  void CreateConditionalBranch(ASTNode *Condition, CFGBlock *ThenBlock, CFGBlock *ElseBlock);

  void InsertPhiNodes();
  void BuildSSAForm();

  std::unique_ptr<CFG> CFGraph;

  CFGBlock *CurrentBlock;
};

CFGBlock* CFGBuilder::CreateBlock(std::string Label) {
  int NextIndex = static_cast<int>(CFGraph->BasicBlocks.size());
  auto *Block = new CFGBlock(NextIndex, Label);
  CFGraph->AddBlock(Block);
  return Block;
}

CFGBuilder::CFGBuilder()
  : CFGraph(new CFG), CurrentBlock(CreateBlock("Entry")) {}

void CFGBuilder::CreateBranch(CFGBlock *Target) {
  CFGBlock::AddLink(CurrentBlock, Target);
  CurrentBlock->AddStatement(std::make_unique<IRBranch>(Target));
}

void CFGBuilder::CreateConditionalBranch(ASTNode *Condition, CFGBlock *ThenBlock, CFGBlock *ElseBlock) {
  CFGBlock::AddLink(CurrentBlock, ThenBlock);
  CFGBlock::AddLink(CurrentBlock, ElseBlock);
  CurrentBlock->AddStatement(std::make_unique<IRBranch>(Condition, ThenBlock, ElseBlock));
}

void CFGBuilder::Traverse(ASTNode *Expr) {
  Expr->Accept(this);
}

void CFGBuilder::Visit(ASTAssign *Expr) {
  BlocksForVariable[Expr->varExp->name].insert(CurrentBlock);
  CurrentBlock->AddStatement(std::make_unique<IRAssignment>(Expr->varExp, Expr->expr));
}

void CFGBuilder::Visit(ASTIf *Expr) {
  CFGBlock *ThenBlock = CreateBlock("Then");
  CFGBlock *ElseBlock = CreateBlock("Else");
  CFGBlock *MergeBlock = CreateBlock("IfContinue");

  CreateConditionalBranch(Expr->conditionExp, ThenBlock, ElseBlock);

  CurrentBlock = ThenBlock;
  Traverse(Expr->thenExp);
  CreateBranch(MergeBlock);

  CurrentBlock = ElseBlock;
  Traverse(Expr->elseExp);
  CreateBranch(MergeBlock);

  CurrentBlock = MergeBlock;
}

void CFGBuilder::Visit(ASTBinary *Expr) {
  Traverse(Expr->lhs);
  Traverse(Expr->rhs);
}

void CFGBuilder::CommitSSAFormBuilding() {
  CFGraph->CommitAllChanges();
  InsertPhiNodes();
  BuildSSAForm();
}

void CFGBuilder::InsertPhiNodes() {
  for (const auto &[VariableName, AssignedBlocks] : BlocksForVariable) {
    std::set<CFGBlock *> DominanceFrontier = CFGraph->GetDominanceFrontierForSubset(AssignedBlocks);
    for (auto *Block : DominanceFrontier) {
      std::map<CFGBlock *, ASTSymbol *> ToVarMap;
      for (auto *Predecessor : Block->Preds)
        ToVarMap[Predecessor] = new ASTSymbol(VariableName);
      auto Phi = std::make_unique<IRPhiNode>(
        std::make_unique<ASTSymbol>(VariableName), ToVarMap);
      Block->Statements.insert(Block->Statements.begin(), std::move(Phi));
    }
  }
}

void CFGBuilder::BuildSSAForm() {
  SSABuilder Builder(CFGraph.get());
  for (auto Pair : BlocksForVariable) {
    Builder.RenameVarToSSAForm(Pair.first);
  }
}

/*****************************************************/
/* CFG Printer                                       */
/*****************************************************/
std::string CFGToDot(CFG *CFGraph) {
  std::string OutGraph;

  OutGraph += "digraph G {\n";
  OutGraph += "  node[shape=box];\n";

  for (auto *Block : CFGraph->BasicBlocks) {
    for (auto *Successor : Block->Succs) {
      auto Dump = [](CFGBlock *Block) {
        std::string Result;

        Result += Block->StringValue();
        Result += "\n";
        for (const auto &Stmt : Block->Statements)
          Result += Stmt->Dump() + "\n";

        return Result;
      };

      OutGraph += "\t\"";
      OutGraph += Dump(Block);
      OutGraph += "\" -> \"";
      OutGraph += Dump(Successor);
      OutGraph += "\"\n";
    }
  }

  return OutGraph + "}\n";
}

void run_ssa_form_tests() {
  std::vector<ASTNode *> Expressions;
  Expressions.push_back(new ASTAssign(new ASTSymbol("first"), new ASTNumber(1)));
  Expressions.push_back(new ASTAssign(new ASTSymbol("second"), new ASTNumber(2)));
  Expressions.push_back(new ASTAssign(new ASTSymbol("third"), new ASTNumber(3)));
  Expressions.push_back(
    new ASTIf(
      // Condition.
      new ASTBinary(
        '+',
        new ASTBinary(
          '+',
          new ASTNumber(2002),
          new ASTNumber(3003)
        ),
        new ASTNumber(4004)
      ),
      // Then body.
      new ASTAssign(new ASTSymbol("first"), new ASTSymbol("second")),
      // Else body.
      new ASTIf(
        // Condition.
        new ASTBinary(
          '/',
          new ASTBinary(
            '*',
            new ASTNumber(5005),
            new ASTNumber(6006)
          ),
          new ASTNumber(7007)
        ),
        // Then body.
        new ASTAssign(new ASTSymbol("first"), new ASTSymbol("second")),
        // Else body.
        new ASTAssign(new ASTSymbol("second"), new ASTSymbol("third"))
      )
    )
  );
  Expressions.push_back(new ASTAssign(new ASTSymbol("first"), new ASTSymbol("third")));
  Expressions.push_back(new ASTAssign(new ASTSymbol("second"), new ASTSymbol("second")));
  Expressions.push_back(new ASTAssign(new ASTSymbol("third"), new ASTSymbol("first")));

  CFGBuilder Builder;
  for (auto *Expr : Expressions)
    Builder.Traverse(Expr);

  Builder.CommitSSAFormBuilding();

  std::ofstream("CFG.gv") << CFGToDot(Builder.GetCFG());
  system("dot -Tjpg CFG.gv -o CFG.jpg && sxiv CFG.jpg");
  std::remove("CFG.gv");
}
