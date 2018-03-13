
#ifndef WRAPPER
#define WRAPPER

#include <list>
#include <llvm/IR/Function.h>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/Instruction.h>
#include <iostream>
#include <llvm/IRReader/IRReader.h>
#include <llvm/IR/LLVMContext.h>
#include <llvm/Bitcode/BitcodeWriter.h>
#include <llvm/Bitcode/BitcodeReader.h>
#include "IRcodeTextDataSet.hpp"
#include "wFunction.hpp"
#include "wBasicBlock.hpp"
#include "wInstruction.hpp"

using namespace llvm;

class IRcodeData;
class IRcodeTextDataSet;

typedef std::unique_ptr<Module> IRmodule;
typedef std::list<GlobalVariable* > GlobalVariableList;
typedef std::list<GlobalVariable* > StaticVariableList;
typedef std::list<Function* > FunctionList;

class IRcodeData
{

private:
	IRcodeTextDataSet* wtask;
	std::unique_ptr<Module>* IRmodule;
	
	class Data
	{
	public:
		Function *f;
		BasicBlock *bb;
		Instruction *inst;

		Data(Function *f, BasicBlock *bb, Instruction *inst):f(f), bb(bb), inst(inst){}
	};
	

public:

	IRcodeData(std::unique_ptr<Module> &m);
	~IRcodeData();
	IRcodeTextDataSet* getIRcodeTextDataSet();
	std::unique_ptr<Module>* getIRmodule();

	/* [preprocess] BasicBlock split */
	bool Preprocess1();
	bool Preprocess2();
	bool Preprocess3();

	bool Preprocess0();


	/* Create GlovalVariables Info */
	void ShowGlobalVariables(std::unique_ptr<Module> &m);
	GlobalVariable *popGlobalVariable(std::unique_ptr<Module> &m, int number);


	/* Create Wrapper class */
	
	bool ShowPathsInEachFunc();

	/* [debug] show IRcodeData element */
	bool DebugWrapperModule();

private:
	GlobalVariableList m_gvl;
	StaticVariableList m_svl;
	FunctionList m_fl;

public:
	GlobalVariableList& getGlobalVariableList();
	StaticVariableList& getStaticVariableList();
	FunctionList& getFunctionList();


public:
	bool setStaticVariableList();
	bool setGlobalVariableList();
	bool setFunctionList();

private:
	bool splitBBModuleOnce(std::unique_ptr<Module> &m);
	int splitBBModuleChecker(std::unique_ptr<Module> &m);

private:
	bool BasicBlockBranchAndFunctionCallImpl();
	bool makePathsInEachFunction();



};


#endif
