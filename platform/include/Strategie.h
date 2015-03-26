#ifndef STRATEGIE_H
#define STRATEGIE_H
#include "Algorithme.hpp"

typedef std::chrono::high_resolution_clock hires;
typedef std::chrono::time_point<hires> point;

namespace chronometer{ 
  double mesure_compute (Algorithme *algo)
  {	
    
    point start,stop;
    start=hires::now();
    algo->compute();
    stop=hires::now();
    return std::chrono::duration_cast<std::chrono::duration<double, std::milli> >(stop - start).count();
  }
  
  double mesure_init (Algorithme *algo)
  {	
    point start,stop;
    start=hires::now();
    algo->init();
    stop=hires::now();
    return std::chrono::duration_cast<std::chrono::duration<double, std::milli> >(stop - start).count();
  }
  double mesure_generate (Generator *gen, Algorithme * algo)
  {	
    point start,stop;
    start=hires::now();
    gen->generate(algo);
    stop=hires::now();
    return std::chrono::duration_cast<std::chrono::duration<double, std::milli> >(stop - start).count();
  }
  
  
  double mesure_fonction (void(*function)())
  {
	
    point start,stop;
    start=hires::now();
    function();
    stop=hires::now();
    return std::chrono::duration_cast<std::chrono::duration<double, std::milli> >(stop - start).count();
  }
}
using namespace chronometer;
class Strategie{
	double total;
	int loop;
	double limite;
public:
	Strategie(){total=0;}
	virtual int getLoop()=0;
	virtual  double getTotal()=0;
	virtual bool add(Algorithme *algo)=0;
	virtual ~Strategie(){}
	virtual void reset()=0;
	virtual double getLimite()=0;
};

/***************** Variance **********************/
class StrategieVariance : public Strategie{
	double total;
	double variance, somme,moy,moy_carre;
	double limite;	
	int loop;
public:
	double getLimite(){return limite;}

	StrategieVariance(double l):limite(l),total(0){ loop=1; variance=0;somme=0;moy=0; moy_carre=0;}
	virtual ~StrategieVariance(){}
	bool add(Algorithme *algo)
	{
		vector<double>::iterator it;
		somme=mesure_compute(algo);
		total+=somme;
		moy_carre=(((moy_carre * (loop-1))+(somme*somme))/loop);
		moy=((moy*(loop-1))+somme)/loop;

		variance=moy_carre-(moy*moy);
		printf("variance %lf \n", variance);
		if(variance >= limite && loop>2)
			return true;
		loop++;
		return false;
	}
	void reset(){
		loop=1;
		total=0;
		variance=0;
		somme=0;
		moy=0;
		moy_carre=0;
	}
	double getTotal(){return total;}
	int getLoop(){return loop;}
};
/**************************************** Loop ************************************/
class StrategieLoop : public Strategie
{
	double total;
	double limite;
	int loop;
public:
	double getLimite() {return limite;}
	StrategieLoop(double l):limite(l){loop=1; total=0;}
	virtual ~StrategieLoop(){}
	int getLoop(){return loop;}
	double getTotal() {return total;}

	/*******add *******/
	bool add(Algorithme *algo)
	{
		total+=mesure_compute(algo);
		if(loop>limite)
			return true;
		loop++;
		return false;
	}
	void reset()
	{
		loop=1;
		total=0;
	}

};

class StrategieTimeOut:public  Strategie
{
	double total; 
	double limite;
	int loop;
public:
	double getLimite(){ return limite;}
	StrategieTimeOut(double timeout): limite(timeout), total(0), loop(1){}
	virtual ~StrategieTimeOut(){}
	int getLoop(){return loop;}
	double getTotal(){return total;}
	bool add(Algorithme *algo)
	{
		total+=mesure_compute(algo);
		if(total> limite)
			return true;
		loop ++;
		return false;
	}
	void reset(){
		loop=1;
		total=0;
	}
};

#endif
