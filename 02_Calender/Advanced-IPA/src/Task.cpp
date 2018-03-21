
#include "Task.hpp"

using namespace IPA;


Task::Task(EntryFunctionTy* entry, SymbolManager* sm)
{
	p_symbolManager = sm;
	SetEntryFunction(entry);
}

bool Task::Process()
{

	GeneratePathList();
	DetermineFunctionList();
	DetermineNonRFL();
	DetermineSemaphoreVariableList();
	DetermineSharedResourcesList();
	ShowNonRFL();
	ShowNonRFL("./dat/NonReentrantFunctionList.dat");
	ShowSRL();
	ShowSRL("./dat/SharedResourcesList.dat");

	return true;
}


std::string Task::getTaskName()
{
	return m_entryFunction->getName();
}


bool Task::SetEntryFunction(EntryFunctionTy* EntryFunction)
{
	m_entryFunction = EntryFunction; 
	return true;
}

EntryFunctionTy* Task::GetEntryFunction()
{
	return m_entryFunction;
}



bool Task::GeneratePathList()
{
	PathImpl pathImpl;
	m_pathList = pathImpl.GenerateGPL(m_entryFunction->getPathList());

	return true;
}

PathListTy* Task::getPathList()
{
	return m_pathList;
}



bool Task::DetermineSharedResourcesList()
{

	for(auto iter1 = m_FunctionList.begin();
			iter1 != m_FunctionList.end(); iter1++)
	{

		wFunction* currentFunc = *iter1;
		for(auto iter2 = currentFunc->getGlobalVariableList().begin();
				iter2 != currentFunc->getGlobalVariableList().end(); iter2++)
		{
			llvm::GlobalVariable* currentGV= *iter2;

			if(m_SRL.empty())
			{
				m_SRL.push_back(currentGV);	
			}
			else
			{

				bool GVFoundFlag = false;
				for(auto iter3 = m_SRL.begin();
						iter3 != m_SRL.end(); iter3++)
				{
					
					if(currentGV == *iter3)
					{
						GVFoundFlag = true;
						break;
					}
				}

				if(GVFoundFlag == false)
				{
					m_SRL.push_back(currentGV);
				}

			}
		}
	}

	for(auto iter1 = m_FunctionList.begin();
			iter1 != m_FunctionList.end(); iter1++)
	{

		wFunction* currentFunc = *iter1;
		for(auto iter2 = currentFunc->getStaticVariableList().begin();
				iter2 != currentFunc->getStaticVariableList().end(); iter2++)
		{
			llvm::GlobalVariable* currentGV= *iter2;

			if(m_SRL.empty())
			{
				m_SRL.push_back(currentGV);	
			}
			else
			{

				bool GVFoundFlag = false;
				for(auto iter3 = m_SRL.begin();
						iter3 != m_SRL.end(); iter3++)
				{
					
					if(currentGV == *iter3)
					{
						GVFoundFlag = true;
						break;
					}
				}

				if(GVFoundFlag == false)
				{
					m_SRL.push_back(currentGV);
				}

			}
		}
	}



	return true;
}



bool Task::DetermineSemaphoreVariableList()
{

	std::cout << "SemVal Analysis in " <<  getTaskName() << std::endl;


	for(auto iter1 = m_pathList->begin();
			iter1 != m_pathList->end(); iter1++)
	{
		Path* currentPath = *iter1;
		
		for(auto iter2 = currentPath->begin();
				iter2 != currentPath->end(); iter2++)
		{
			wBasicBlock* currentBasicBlock = *iter2;
	
			bool IsUncountedFlag = false;

			for(auto iter3 = p_symbolManager->getPUFL().begin();
					iter3 != p_symbolManager->getPUFL().end(); iter3++)
			{

				std::string comp_string = *(*iter3) + "%0";

				if(*(currentBasicBlock->getName()) == comp_string)
				{
#ifdef TM_DEBUG
					std::cout << "Found Uncounted Function: " << comp_string << std::endl;
#endif
					IsUncountedFlag = true;
					break;
				}
				else
				{
					IsUncountedFlag = false;
				}

			}

			//wFunction* parentFunction = currentBasicBlock->getParent();	
		
	
			//std::cout << "[CHECK] " << *(currentBasicBlock->getName()) << std::endl;

			if(IsUncountedFlag == true)
			{
				continue;
			}

			
			llvm::BasicBlock* raw_currentBasicBlock = currentBasicBlock->getBasicBlock();

			for(auto iter4 = raw_currentBasicBlock->begin();
					iter4 != raw_currentBasicBlock->end(); iter4++)
			{
				
				llvm::Instruction& currentInst = *iter4;

				
				if(!strcmp(currentInst.getOpcodeName(), "call"))
				{
					
					llvm::Value* _calleeF 
						= currentInst.getOperand(currentInst.getNumOperands() - 1);

					llvm::Function* calleeF 
						= reinterpret_cast<Function*>(_calleeF);

					if(calleeF->getName().str() == std::string("pthread_mutex_init"))
					{
						llvm::Value* semValue = currentInst.getOperand(0);
						std::cout << "Defined SemValue: " << semValue->getName().str() << std::endl;


						if(m_DSVL.empty())
						{
							m_DSVL.push_back(semValue);
							
						}
						else
						{

							bool FoundSemValFlag = false;
							for(auto iter5 = m_DSVL.begin();
									iter5 != m_DSVL.end(); iter5++)
							{
								if(semValue == *iter5)
								{
									FoundSemValFlag = true;
									break;
								}
								else
								{
									/* empty */
								}

							}
							
							if(FoundSemValFlag == false)
							{
								m_DSVL.push_back(semValue);
							}
							else
							{
								/* empty */
							}
						
						}
					
					}



					if(calleeF->getName().str() == std::string("pthread_mutex_lock"))
					{
						llvm::Value* semValue = currentInst.getOperand(0);
						std::cout << "Locked SemValue: " << semValue->getName().str() << std::endl;


						if(m_LSVL.empty())
						{
							m_LSVL.push_back(semValue);
							
						}
						else
						{

							bool FoundSemValFlag = false;
							for(auto iter5 = m_LSVL.begin();
									iter5 != m_LSVL.end(); iter5++)
							{
								if(semValue == *iter5)
								{
									FoundSemValFlag = true;
									break;
								}
								else
								{
									/* empty */
								}
							}

							if(FoundSemValFlag == false)
							{
								m_LSVL.push_back(semValue);
							}
							else
							{
								/* empty */
							}
						
						}
					}


					if(calleeF->getName().str() == std::string("pthread_mutex_unlock"))
					{
						llvm::Value* semValue = currentInst.getOperand(0);
						std::cout << "Unlocked SemValue: " << semValue->getName().str() << std::endl;


						if(m_USVL.empty())
						{

							m_USVL.push_back(semValue);
							
						}
						else
						{

							bool FoundSemValFlag = false;
							for(auto iter5 = m_USVL.begin();
									iter5 != m_USVL.end(); iter5++)
							{
								if(semValue == *iter5)
								{
									FoundSemValFlag = true;
									break;
								}


								else
								{
									/* empty */
								}
							}

							if(FoundSemValFlag == false)
							{
								m_USVL.push_back(semValue);
							}
							else

							{
								/* empty */
							}
						}
						
					}


				}
				
			}
	
		}

	}

	if(m_DSVL.empty())
	{
		std::cout << "Defined SemValue: None" << std::endl;
	}
	if(m_LSVL.empty())
	{
		std::cout << "Locked SemValue: None" << std::endl;
	}
	if(m_USVL.empty())
	{
		std::cout << "Unlocked SemValue: None" << std::endl;
	}



	std::cout << std::endl;

	return true;
}

bool Task::DetermineFunctionList()
{

	for(auto iter1 = m_pathList->begin();
			iter1 != m_pathList->end(); iter1++)
	{
		Path* currentPath = *iter1;
		
		for(auto iter2 = currentPath->begin();
				iter2 != currentPath->end(); iter2++)
		{
			wBasicBlock* currentBasicBlock = *iter2;	
			wFunction* parentFunction = currentBasicBlock->getParent();	
		
			if(m_FunctionList.empty())
			{
				m_FunctionList.push_back(parentFunction);
			}

			else
			{
				bool FunctionFoundFlag = false;
				for(auto iter3 = m_FunctionList.begin();
						iter3 != m_FunctionList.end(); iter3++)
				{
					if(parentFunction == *iter3)
					{
						FunctionFoundFlag = true;
						break;
				 	}
				
				}
	
				if(FunctionFoundFlag == false)
				{
					m_FunctionList.push_back(parentFunction);
				}
			}

		}

	}

	return true;
}

bool Task::DetermineNonRFL()
{

	for(auto iter1 = m_pathList->begin();
			iter1 != m_pathList->end(); iter1++)
	{
		Path* currentPath = *iter1;
		
		for(auto iter2 = currentPath->begin();
				iter2 != currentPath->end(); iter2++)
		{
			wBasicBlock* currentBasicBlock = *iter2;	
			wFunction* parentFunction = currentBasicBlock->getParent();	
		
			if(m_NonRFL.empty())
			{
				m_NonRFL.push_back(parentFunction);
			}

			else
			{
				bool FunctionFoundFlag = false;
				for(auto iter3 = m_NonRFL.begin();
						iter3 != m_NonRFL.end(); iter3++)
				{
					if(parentFunction == *iter3)
					{
						FunctionFoundFlag = true;
						break;
					}
					
				}
				if(FunctionFoundFlag == false)
				{
					if(parentFunction->isReentrant())
					{
						m_NonRFL.push_back(parentFunction);
					}
				}
			}
		}

	}

	return true;
}


bool Task::ShowFunctionList()
{
#ifdef TASK_DEBUG
	std::cout << "Function" << std::endl;
	std::cout << "Num: " << m_FunctionList.size() << std::endl;
#endif


	for(auto iter1 = m_FunctionList.begin();
			iter1 != m_FunctionList.end(); iter1++)
	{
		wFunction* currentFunc = *iter1;
		//std::cout << currentFunc->getName() << " ";
		std::cout << currentFunc->getFunction()->getName().str() << " ";


	}
	std::cout << std::endl;

	return true;
}

bool Task::ShowNonRFL()
{
	
	std::cout << "Non-Reentrant Func in " << getTaskName() << std::endl;

	for(auto iter1 = m_NonRFL.begin();
			iter1 != m_NonRFL.end(); iter1++)
	{
		wFunction* currentFunc = *iter1;
		std::cout << currentFunc->getFunction()->getName().str() << " ";

	}
	std::cout << std::endl;
	std::cout << std::endl;
	
	return true;
}

bool Task::ShowNonRFL(const char* dat)
{
	std::ofstream fout;
	fout.open(dat, std::ofstream::out | std::ofstream::app);
	
	fout << "#Task: " << getTaskName() << std::endl;

	for(auto iter1 = m_NonRFL.begin();
			iter1 != m_NonRFL.end(); iter1++)
	{
		wFunction* currentFunc = *iter1;
		fout << currentFunc->getFunction()->getName().str() << " ";
	}
	fout << std::endl;
	fout.close();
	
	return true;
}


bool Task::ShowSRL()
{
	
	std::cout << "Shared Resources in " << getTaskName() << std::endl;

	for(auto iter1 = m_SRL.begin();
			iter1 != m_SRL.end(); iter1++)
	{
		llvm::Value* currentGV = *iter1;
		std::cout << currentGV->getName().str() << " ";

	}
	std::cout << std::endl;
	std::cout << std::endl;
	
	return true;
}

bool Task::ShowSRL(const char* dat)
{
	std::ofstream fout;
	fout.open(dat, std::ofstream::out | std::ofstream::app);
	
	fout << "#Task: " << getTaskName() << std::endl;

	for(auto iter1 = m_SRL.begin();
			iter1 != m_SRL.end(); iter1++)
	{
		llvm::Value* currentCV = *iter1;
		fout << currentCV->getName().str() << " ";
	}
	fout << std::endl;
	fout.close();
	
	return true;
}


/*
TaskCreator::TaskCreator(Task* task, EntryFunctionTy* entry)
{
	task->SetEntryFunction(entry)
	task->GeneratePathList();

}

TaskCreator::~TaskCreator()
{

}
*/










