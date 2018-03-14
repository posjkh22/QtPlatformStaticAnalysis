
#include "Task.hpp"

using namespace IPA;


Task::Task(EntryFunctionTy* entry)
{
	SetEntryFunction(entry);
	GeneratePathList();
	DetermineFunctionList();
	DetermineNonRFL();

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
	std::ofstream fout;
	fout.open("./dat/NonReentrantFunctionList.dat", std::ofstream::out | std::ofstream::app);
	
	std::cout << "Non Reentrant Function" << std::endl;
#ifdef TASK_DEBUG
	std::cout << "Num: " << m_NonRFL.size() << std::endl;
#endif

	fout << "#Task: " << getTaskName() << std::endl;

	for(auto iter1 = m_NonRFL.begin();
			iter1 != m_NonRFL.end(); iter1++)
	{
		wFunction* currentFunc = *iter1;
		//std::cout << currentFunc->getName() << " ";
		std::cout << currentFunc->getFunction()->getName().str() << " ";
		fout << currentFunc->getFunction()->getName().str() << " ";

	}
	std::cout << std::endl;
	
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










