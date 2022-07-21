#include "llvm/Support/DataTypes.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/ExecutionEngine/ExecutionEngine.h"
#include "llvm/ExecutionEngine/GenericValue.h"
#include "llvm/IR/IRBuilder.h"

#include <vector>
#include <cstdio>
#include <string>

int main()
{
  llvm::LLVMContext Ctx;
  llvm::Module *Module = new llvm::Module("asdf", Ctx);
  llvm::IRBuilder<> Builder(Ctx);

  llvm::FunctionType *Signature = llvm::FunctionType::get(Builder.getVoidTy(), false);
  llvm::Function *Func = llvm::Function::Create(Signature, llvm::Function::ExternalLinkage, "main", Module);
  llvm::BasicBlock *Entry = llvm::BasicBlock::Create(Ctx, "entry", Func);
  Builder.SetInsertPoint(Entry);

  llvm::Value *S = Builder.CreateGlobalStringPtr("hello world!\n");
  Builder.CreateRet(S);

  std::string Result;

  for (const auto &F : Module->getFunctionList()) {
    llvm::raw_string_ostream Stream(Result);
    Stream << F;
    Stream << '\n';
    Stream.flush();
  }

  printf("%s\n", Result.c_str());
}
