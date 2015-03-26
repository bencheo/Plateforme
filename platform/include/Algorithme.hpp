
#include <iostream>
#include <map>
#include <list>
#include <functional>
#include "Type.h"
#include <valarray>
#include <vector>
#include <chrono>
#include <string>

using namespace std;
struct Algorithme;
struct SubStep
{
	std::function<void()> func;
	std::string name;
}; 
struct Parameter
{
	std::string name;
	valueMesure*val, *init;
	Parameter(){}
	template<class T >Parameter(string name, T& field): name(name)
	{
		val=new valueMesure(&field,true);
		init=new valueMesure(&field,true);
	}
	template<class T> void set(T& v)
	{
		val->setData(v);
	}
	void reset()
	{
		val->setData(init->getData()); 
	}
};

struct Range{
	Range():begin(0),end(0){}
	Range( double b, double e,double s): begin(b), end(e),step(s){}
	double begin,end,step;
	bool next(valueMesure* val) {
		double newVal= val->getDouble();
		if(newVal > end-step) {
			val->setDouble(begin);
			return true;
		} else {
			newVal += step;
			val->setDouble(newVal);
			return false;
		}
	}
/*	void reset(valueMesure* val) {
		val->setDouble(begin);
	}*/
};

class Generator;
/***************************************** Algorithme   and Generator*********************/
struct Algorithme
{
	list<SubStep> initStep;
	list<SubStep> computeStep;
	map<string, Parameter> parameters;
	Generator* defaultGenerator;
	virtual void compute()=0;
	virtual void addParam(Parameter *param)=0;
	Algorithme(): defaultGenerator(0){}
	virtual ~Algorithme(){	}
	virtual void init()=0;
	virtual bool validate()=0;
	virtual  Range defaultRange()=0;
	// GPU ?
	// virtual void upload(){}
	// virtual void download (){}
};

class Generator
{
public:
	Generator(){}
	virtual ~Generator(){}
	virtual void generate(Algorithme* a)=0;
};

