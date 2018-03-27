
#include <iostream>
#include <fstream>
#include "Visualize.hpp"

using namespace IPA;


bool Visualize::setVisualizePolicy(VisualizationPolicy* new_policy)
{
	if(p_policy == nullptr)
	{
		p_policy = new_policy;
	}
	else
	{
		delete p_policy;
		p_policy = new_policy;
	}

	return true;
}

bool Visualize::run()
{
	p_policy->activate("./dat/SharedResourcesList.dat");

	return true;
}

bool Visualize::Setup()
{
	
	p_policy->apply_input_value("./dat/resource_graph.dot");

	return true;
}

bool Graphviz::apply_input_value(const char* dat)
{
	
	std::ifstream fin;
	fin.open(dat, std::ifstream::in);

	#define STRING_SIZE 128
	char inputString[STRING_SIZE];
	std::string* p_inputString;

	while(!fin.eof())
	{	
		fin.getline(inputString, STRING_SIZE);
		p_inputString = new std::string(inputString);
		input_value_list.push_back(p_inputString);
	}

	#undef STRING_SIZE
	
	fin.close();
	return true;
}

bool Graphviz::debug_show_input_value()
{
	for(auto iter = input_value_list.begin();
		iter != input_value_list.end(); iter++)
	{
		std::string currentstring(**iter);
		std::cout << currentstring << std::endl;
	}

	return true;
}



bool Graphviz::activate(const char* dat)
{
	
	std::ofstream fout;
	fout.open(dat, std::ofstream::out);

	fout << "digraph Resource {" << std::endl;	

	fout << "node [shape=box]; ";
	for(auto iter = input_value_list.begin();
		iter != input_value_list.end(); iter++)
	{	
		std::string currentString(**iter);
		if(currentString.at(0) == '#')
		{
			fout << " " << currentString << ";";
		
		}

	}

	fout << "node [shape=circle, fixedsize=true, width=0.9]; ";
	for(auto iter = input_value_list.begin();
		iter != input_value_list.end(); iter++)
	{	

		std::string currentString(**iter);
		if(currentString.at(0) != '#')
		{	
			fout << " " << currentString << ";";
		}
	
	}

	for(auto iter = input_value_list.begin();
		iter != input_value_list.end(); iter++)
	{	

		std::string currentString(**iter);
		std::string processString;
		if(currentString.at(0) != '#')
		{	
			processString = currentString;
		
		}
		else
		{	
			fout << processString << "->";
			fout << currentString << ";" << std::endl;
		}


	}
	fout.close();
	return true;
}
