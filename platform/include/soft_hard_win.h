#pragma comment(lib, "psapi")

#include "Type_Link.h"
#include <list>
#include <map>

#include <Windows.h>
#include <winbase.h>
struct Base
{
  map<string,value*> varenv;
  list<value*> dll;
};


struct Soft:Base
{

  double loadavg;

  string os;
  Soft (){}
  ~Soft(){}
  string getOS();
  void getVarsEnv(char * var);
  int getDll(ULONG idP);
  double getCpuUsage();
  void getVarsEnvAll();
  void getMemUsage();
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
  	  float usedMem,freeMem,totalMem;

  string getExtensions(){return extensions;}
  CPU()
  {
    //values.insert(pair<char*,value>("jk",value(12)));
  }
  virtual int GetNumProc()=0;
  virtual int GetNumCores()=0;
  virtual int GetCPUFrequency()=0;
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

  size_t totalmem;
  size_t freemem;
  size_t usedmem;

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



