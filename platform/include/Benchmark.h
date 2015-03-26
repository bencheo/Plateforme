
#include <string>
#include "Strategie.h"
#include "Link_xml_info.h"
#define LOOP_TEST 1000
#define SIZE_MATRIX 100
#define myIt map<string, pair<Range,valueMesure*>>::iterator

//MatMultiplier *MM;
//string NextFile( string file_name);
char* empty="";
using namespace chronometer;
struct Benchmark {                                                      
	Algorithme *algorithme;
	Generator  *generator;
	Strategie *strategy;
	DocumentXML *doc;
	string compiler, version_compiler, option, diagnostic;

	bool oneRun;
	void (*function)();
	bool isFunc;
	Benchmark():function(0),algorithme(0), generator(0), strategy(0){}
	Benchmark(void (func)(), Strategie *strat) :algorithme(0), generator(0),strategy(strat),function (func){}
	Benchmark(Algorithme *algo): algorithme(algo),function(0), generator(0), strategy(0){}
	Benchmark(Algorithme *algo, Generator *gen): function(0),algorithme(algo), generator(gen), strategy(0){}
	Benchmark(Algorithme *algo, Strategie *strat):function(0),algorithme(algo), generator(0),strategy(strat){}
	Benchmark(Algorithme *algo, Generator *gen,Strategie *strat):function(0),algorithme(algo), generator(gen),strategy(strat){}
	~Benchmark(){
		if(algorithme)delete algorithme;
		if(generator) delete generator;
		if(strategy)  delete strategy;
	}
	void setMakeInfo(string compile, string version_compile, string options , string diag)
	{

		compiler= compile;
		version_compiler=version_compile;
		option=options;
		diagnostic=diag;

	}
	void Start_Benchmarking()
	{
		cout << " Start Benchmarking" << endl;
		doc=beginDocument();
		Soft *soft= new Soft;
		CPU_Intel * cpu = new CPU_Intel;
		WriteInfo(doc, soft, cpu, compiler, version_compiler, option, diagnostic);

		/*string cast;
		beginGroup(doc, "mesures","");
		string name("name operation");
		beginGroup(doc, "operation",name);
		name = "list of parameters";
		beginGroup(doc, "opparams",name);
		endGroup(doc);
		*/
	}
	void End_Measure()
	{
		while(endGroup(doc)>2);
	}

	void End_Benchmarking(string file_name)
	{
		const char * fn= file_name.c_str();
	
		if(doc->stack_group.size()>1)
			while(endGroup(doc)>1);
		endDocument(doc, fn);
			cout << "End benchmarkin" << endl;
	}

	void New_Measure()
	{
	  beginGroup(doc, "measure", empty);
	}

	void ParamSetting(string settings)
	{

		beginGroup(doc,"paramsettings", settings);
		for(auto it = algorithme->parameters.begin(); it!= algorithme->parameters.end(); it++)
		{
			addValue(doc, it->first);
			detailValue(doc, it->first, it->second.val->toString());
		}
		/********* values ??? ********/
		endGroup(doc);
	       
		beginGroup(doc, "data",empty);
	}

	void DataInstance()
	{

		string Datas="";
		/*	for( auto it= algorithme->parameters.begin(); it != algorithme->parameters.end(); it++)
		datas+=it->second.val->getDouble();*/
		//	Datas=datas;
		beginGroup(doc, "data-instance",empty);
		detailGroup(doc,"value", Datas);
		//	beginGroup(doc, "values", "");
	}
	void Operation(string operation)
	{
		beginGroup(doc,"operation", operation);

	}
	void Values(double time, double memory=0, double throughout=0)
	{
		beginGroup(doc, "values", empty);
		addValue(doc, "time");
		setValueAttribute(doc, time);
		detailValue(doc, "unit", "ms");
		addValue(doc, "memory");
		setValueAttribute(doc, memory);
		detailValue(doc, "unit", "Ko");
		addValue(doc, "throughput");
		setValueAttribute(doc, throughout);
		detailValue(doc, "unit", "Ko/s");
		endGroup(doc);
		endGroup(doc);
	}



	void reset(){ 	
		for(auto it = algorithme->parameters.begin(); it != algorithme->parameters.end(); it++)
			it->second.reset();
	}

	double* run()
	{
		double timer[2];
		if(!generator)
			if(algorithme->defaultGenerator)
				generator= algorithme->defaultGenerator;
			else{
				cout <<"generator nil \n"<< endl;
				exit(1);
			}

			timer[0]=mesure_generate(generator,algorithme);
			timer[1]=mesure_compute(algorithme);

			
			return timer;
	}

	/**** default_run used with validate ***********/
	void default_run(){
		oneRun=true;
		double time[3];
		reset();
		time[0]=mesure_init(algorithme);

		if(algorithme){
			double *timer=run();
			time[1]=timer[0];
			time[2]=timer[1];
		}

		else if(function){
			double	time_compute=mesure_fonction(function);
		}
		printf("temps de generatin des val aléatoires %lfms\n", time[1]);
		printf("temps d'initialisation                %lfms\n", time[0]);
		printf("temps d'execution                     %lfms\n", time[2]);
	}

	/***** default_runs used to compute an average whith/without a strategy ***/
	void default_runs(){
		New_Measure();
		ParamSetting("");
		DataInstance();
		oneRun= false;
		float time_compute=0, time_init=0,time_generate=0;
		double *timer;
		int loop;
		string datas;
		reset();
		time_init=mesure_init(algorithme);
		//		size_t size=*((size_t*) algorithme->parameters.find("size")->second.val->getData());
		if(algorithme){
			if(!strategy){
				for(int i =1 ;i< LOOP_TEST; i++) {
					timer=run();
					time_generate+=timer[0];
					time_compute+=timer[1];

					
				}
				time_compute /= LOOP_TEST-1;
				time_generate/=LOOP_TEST-1;
				loop=LOOP_TEST;
			}
			else{
				int nb_loop;
				do{
					timer=run();
					time_generate+= timer[0];
					time_compute+=timer[1];
					nb_loop= strategy->getLoop();


				}while(strategy->add(algorithme)==false);
				loop=strategy->getLoop();
				time_compute=strategy->getTotal()/strategy->getLoop();
				time_generate/=strategy->getLoop();

				strategy->reset();
			}
		}
		Operation("init");
		Values(time_init);
		Operation("generate");
		Values(time_generate);
		Operation("compute");
		Values(time_compute);
		End_Measure();
		printf("temps d'execution   %lfms      loop %d \n", time_compute, loop);
		printf("temps d'init        %lfms      loop %d \n", time_init, loop);
		printf("temps de generation %lfms      loop %d \n", time_generate, loop);
	}

	/***************** Parameters and getNextToIncrement ***********************/
	void startAtRange(myIt begin, myIt end){
		for(auto it= begin; it!= end; it++)
		{
			algorithme->parameters.find(it->first)->second.val->setDouble(it->second.first.begin);
		}

	}
	bool getNextToIncrement( myIt begin, myIt end){
		myIt last= end; 
		last--;

		int cpt=0;
		for(auto it= ++begin; it!= end; it++)
		{
			if(it->second.second->getDouble() >= it->second.first.end)
				cpt++;

		}

		if(cpt== (algorithme->parameters.size()-1))
			return false;

		for(auto it= begin; it!= end; it++)
		{
			if(it->second.second->getDouble() < it->second.first.end)
			{
				it->second.second->setDouble(it->second.second->getDouble()+1);
				return true;
			}
			if(it!=last){
				algorithme->parameters.find(it->first)->second.val->setDouble(it->second.first.begin);
			}

		}
		return false;
	}

	void parameters( map<string,Range> param ){
		if(!strategy){
			printf("Benchmark must be instanciated with a strategy\n");
			exit(-1);
		}

		float time_compute=0, time_init=0, time_generate=0;
		double *timer;
		int nb_loop;
		std::size_t found;
		map<string, pair< Range, valueMesure* >> linkedParams;

		if(!generator && algorithme->defaultGenerator)
			generator= algorithme->defaultGenerator;
		if(!generator && !algorithme->defaultGenerator){
			cout << "generator nil  error " << endl;
			exit(1);
		}

		/**************** intersection algo->parametre et  param **********************/
		for(auto it = param.begin(); it!=param.end(); it++)
		{
			auto al= algorithme->parameters.find(it->first);
			if(al!= algorithme->parameters.end())
			{
				linkedParams[it->first].first= (it->second);
				linkedParams[it->first].second=al->second.val;
			}
		}

		/***************************    Faire varier les paramètres *****************/
		bool allIsDone= false;
		bool isFirst= true;
		bool res=false;
		auto firstParam= linkedParams.begin();
		auto PastlastParam= linkedParams.end();
		auto firstParamSecond = firstParam->second;
		startAtRange(firstParam, PastlastParam);
		int loop;

		while(!allIsDone){

			if(isFirst)
			{
				do
				{
					New_Measure();
					ParamSetting("");

					DataInstance();
					time_init=mesure_init(algorithme);
					do{
						timer=run();
						time_generate+= timer[0];
						time_compute+=timer[1];
						nb_loop= strategy->getLoop();
					}while(strategy->add(algorithme)==false);
					loop=strategy->getLoop();
					time_compute=time_compute/loop;
					time_generate/=strategy->getLoop();

					Operation("init");
					Values(time_init);
					Operation("generate");
					Values(time_generate);
					Operation("compute");
					Values(time_compute);
					endGroup(doc);
					endGroup(doc);
					End_Measure();

					strategy->reset();
					time_compute=0;
					time_generate=0;
					/*				printf("temps d'execution   %lfms      \n", time_compute);
					printf("temps d'init        %lfms      \n", time_init);
					printf("temps de generation %lfms      \n", time_generate);
					printf("\n\n\n");
					printf("**************************************Matrix A******************************** \n");
					printf_matrix(MM->a);
					printf("**************************************Matrix B******************************** \n");
					printf_matrix(MM->b);
					printf("**************************************Matrix C******************************** \n");
					printf_matrix(MM->c);			
					printf(" value of algorithme parameters %lf    %lf     testC %f  \n", MM->a.col ,MM->a.row, algorithme->parameters.find("testC")->second.val->getDouble());
					*/
					//					printf(" value of first parameter of the algorithme %lf    %lf    testC %f  \n", MM->a.col ,MM->a.row, algorithme->parameters.find("testC")->second.val->getDouble());

				}while(!firstParamSecond.first.next(firstParamSecond.second));
				isFirst= false;
				continue;
			}
			if(linkedParams.size()>1){
				algorithme->parameters.begin()->second.reset();
				res=getNextToIncrement(firstParam, PastlastParam);
				if(!res){
					allIsDone=true;
					break;
				}
				do{
					//

					New_Measure();
					ParamSetting("");

					DataInstance();
					time_init=mesure_init(algorithme);
					//					printf(" value of first parameter of the algorithme %lf    %lf    testC %f  \n", MM->a.col ,MM->a.row, algorithme->parameters.find("testC")->second.val->getDouble());
					do{
						timer=run();
						time_generate+= timer[0];
						time_compute+=timer[1];
						nb_loop= strategy->getLoop();
					}while(strategy->add(algorithme)==false);

					//printf(" time generate %lf     time compute   %lf \n", time_generate, time_compute);
					Operation("init");
					Values(time_init);
					Operation("generate");
					Values(time_generate);
					Operation("compute");
					Values(time_compute);
					endGroup(doc);
					endGroup(doc);
					End_Measure();
					strategy->reset();
					time_compute=0;
					time_generate=0;
					/*	printf("temps d'execution   %lfms      \n", time_compute);
					printf("temps d'init        %lfms      \n", time_init);
					printf("temps de generation %lfms      \n", time_generate);
					printf("\n\n\n");
					printf("**************************************Matrix A******************************** \n");
					printf_matrix(MM->a);
					printf("**************************************Matrix B******************************** \n");
					printf_matrix(MM->b);
					printf("**************************************Matrix C******************************** \n");
					printf_matrix(MM->c);

					*/

				}while(!firstParamSecond.first.next(firstParamSecond.second));

			}
		}

	}

string NextFile( string file_name)
{
  stringstream ss_old, ss_new ;
  FILE *file=NULL;
	int i=0;
	if(!regex_match(file_name.c_str(),regex(".*\\.xml")))
	  {
	    ss_old <<file_name << ".xml";
	  }
	file = fopen(ss_old.str().c_str(), "r");
	if(!file)
	  return ss_old.str();
	while(file){
	  
	  i++;
	  stringstream sstmp;
	  sstmp<< file_name << i << ".xml";
	  
		file = fopen(sstmp.str().c_str(), "r");
	}
	ss_new <<file_name << i << ".xml";
	return ss_new.str();
}


};

