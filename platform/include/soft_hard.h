


#include "Type_Link.h"
#include <list>
#include <map>
#ifdef _WIN32 
#include <Windows.h>
#include <winbase.h>
#else
#define ULONG ulong
#endif

struct Base
{
  map<string,value*> varenv;
  list<value*> dll;
};


struct Soft:Base
{

  double loadavg;
 float usedMem,freeMem,totalMem;
string author;

  string os;
  Soft (){}
  ~Soft(){}
  string getOS();
  void getVarsEnv(char * var);
  int getDll(ULONG idP);
  double getCpuUsage();
  void getVarsEnvAll();
  void getMemUsage();
  string getAuthor();
};



void get(Soft *s);
struct Base_hardware {
  map<string, value*> values;
};

struct CPU: Base_hardware {
  string name;
  float freq;
  int cores;
  int processor;
  int cache_line_size;
  int cache_size;
  string extensions;
  

  

  string getExtensions(){return extensions;}
  CPU()
  {
    //values.insert(pair<char*,value>("jk",value(12)));
  }
  virtual int GetNumProc()=0;
  virtual int GetNumCores()=0;
  virtual int GetCPUFrequency()=0;
 virtual float getTotalSysMemory()=0;
 virtual float getUsedSysMemory()=0;
  virtual float getFreeSysMemory()=0;
  virtual void get() = 0;
};
struct CPU_ARM: CPU
{
  CPU_ARM() //:CPU(name,freq,cores,processor)
  {
   // values.insert(pair<char*,value*>("arm",new value(true)));
  }
  //void get(){}
};

struct CPU_Intel: CPU {

   float usedMem,freeMem,totalMem;

  float getTotalSysMemory();
  float getUsedSysMemory();
  float getFreeSysMemory();


  CPU_Intel():CPU()
  {
    values["intel"]=new value(true);
  }
  void get();
  int GetNumProc();
  int GetNumCores();
  int  GetCPUFrequency();

};


void getCPU_info(CPU* cpu);



