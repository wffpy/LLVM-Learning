#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Verifier.h"
#include "vector"
// #include "llvm/IR/LLVMContext.h"

using namespace llvm;

static LLVMContext &Context = getGlobalContext();
static Module *ModuleOb = new Module("my compiler", Context);
static std::vector<std::string> FunArgs;

Function * createFunc(IRBuilder<> &Builder, std::string Name){
    Type *u32Ty = Type::getInt32Ty(Context);
    Type *vecTy = VectorType::get(u32Ty, 4);
    Type *ptrTy = vecTy->getPointerTo(0);
    FunctionType *funcType = 
        FunctionType::get(Builder.getInt32Ty(), ptrTy, false);
    Function *fooFunc = 
        Function::Create(funcType, Function::ExternalLinkage,Name, ModuleOb);
    
    return fooFunc;
}

void setFuncArgs(Function *fooFunc, std::vector<std::string> FunArgs) {
    unsigned Idx = 0;
    Function::arg_iterator AI, AE;
    for (AI = fooFunc->arg_begin(), AE = fooFunc->arg_end(); AI != AE; ++AI, ++Idx) {
        AI->setName(FunArgs[Idx]);
    }
}

BasicBlock *createBB(Function *fooFunc, std::string Name) {
    return BasicBlock::Create(Context, Name, fooFunc);
}

Value *createArith(IRBuilder<> &Builder, Value *L, Value *R) {
    return Builder.CreateMul(L, R, "multmp");
}

Value *getInsertElememt(IRBuilder<> &Builder, Value *Vec, Value *val, Value *Index) {
    return Builder.CreateInsertElement(Vec, val, Index);
}

Value *getExtractElement (IRBuilder<> &Builder, Value *Vec, Value *Index) {
    return Builder.CreateExtractElement(Vec,Index);
}

int main(int argc, char const *argv[])
{
    /* code */
    FunArgs.push_back("a");
    static IRBuilder<> Builder(Context);
    Function *fooFunc = createFunc(Builder, "foo");
    setFuncArgs(fooFunc, FunArgs);
    llvm::Value *Vec = &*(fooFunc->arg_begin());
    BasicBlock *entry = createBB(fooFunc, "entry");
    Builder.SetInsertPoint(entry);

    for (unsigned int i =0; i<4; i++)
        Value *V = getInsertElememt(Builder, Vec, Builder.getInt32((i + 1)*10), Builder.getInt32(i));
    
    SmallVector<Value *, 4> V;
    for (unsigned int i = 0; i<4; i++)
        V[i] = getExtractElement(Builder, Vec, Builder.getInt32(i));
    
    Builder.CreateRet(Builder.getInt32(0));
    verifyFunction(*fooFunc);
    ModuleOb->dump();
    return 0;
}
