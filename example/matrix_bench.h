#include "matrix.h"
#include "Benchmark.h"

struct MatrixPairGen;
struct MatMultiplier: Algorithme {
	double sizeA, sizeB;
	double testC;
	Matrix<int> a, b, c;
	MatMultiplier(double sA, double sB,double tC):sizeA(sA), sizeB(sB), testC(tC)
	{
		defaultGenerator=NULL;	
		string pd("testD");
		addParam(new Parameter(pd,(double  &)sizeA));
		string pA("sizeA");
		addParam(new Parameter(pA,(double  &)sizeA));
		string pB("sizeB");
		addParam(new Parameter(pB,(double  &)sizeB));
		string pC("testC");
		addParam(new Parameter(pC,(double  &)testC));
	}

	~MatMultiplier() {}
	
	void addParam(Parameter *param){
		parameters[param->name]=*param;
	}
	void init() 
	{
		string sA("sizeA");
		string sB("sizeB");
		string sC("testC");

		double  ssA=parameters[sA].val->getDouble();
		double  ssB=parameters[sB].val->getDouble();
		double  ssC=parameters[sC].val->getDouble();

		a.resize(ssA,ssB); // b, c
		b.resize(ssB,ssA);
		c.resize(ssB,ssB);

	}

	Range defaultRange()
	{
		Range r;
		r.begin =4;
		r.end=100;
		return r;
	}
	void compute()
	{ 
		c = fastMult(&a, &b);
	}
	bool validate() { return true; }
};

struct MatrixPairGen: Generator {
	MatrixPairGen(){}
	void genMatrix(Matrix<int> &m)
	{
		for (auto it=m.data.begin();it!=m.data.end();it++)
		{
			*(it)=rand()%4+1 ;
		};
	}
	void generate(Algorithme* algo) {
		MatMultiplier* mm = dynamic_cast<MatMultiplier*>(algo);
		if(!mm) throw "bad algorithm";
		else
		{
			mm->defaultGenerator= new MatrixPairGen;
			genMatrix(mm->a);
			genMatrix(mm->b);
		}
	}
};

struct LowRankGenerator: MatrixPairGen {
	void genMatrix(Matrix<int>m)
	{
		for(auto it=m.data.begin();it!=m.data.end();it++)
		{
			*(it)=rand()%10;
		}
	}
};
