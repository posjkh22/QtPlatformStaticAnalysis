
#include <list>
#include <string>

namespace IPA
{

class Visualize;
class VisualizationPolicy;

class Visualize final
{

public:
	Visualize()
	{
		p_policy = nullptr;
	}

private:
	VisualizationPolicy* p_policy;

public:
	bool setVisualizePolicy(VisualizationPolicy* );
	bool run();
	bool Setup();
};


class VisualizationPolicy
{
protected:
	std::list<std::string* > input_value_list;

public:
	virtual bool apply_input_value(const char*){ return true; };
	virtual bool activate(const char*){ return true; };

	virtual ~VisualizationPolicy(){};
};


class Graphviz final: public VisualizationPolicy
{
	
public:
	bool apply_input_value(const char* ) override;
	bool debug_show_input_value();
	bool activate(const char* ) override;

};

};
