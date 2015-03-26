#include "Type.hpp"
#include <list>
#include <string>
struct Base
{
  map<string,value*> varenv;
  list<value*> dll;
};

struct Soft:Base
{
  double loadavg;
  long usedMem,freeMem,totalMem; 
  string os;
  Soft (){}
  ~Soft(){}
  string getOS();
  void getVarsEnv(char * var);
  int getDll(ulong idP);
  double getCpuUsage();
  void getVarsEnvAll();
  void getMemUsage();
};


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

  virtual ~CPU(){}
  virtual float getTotalSysMemory()=0;
  virtual float getUsedSysMemory()=0;
  virtual float getFreeSysMemory()=0;

  
  virtual int GetNumProc()=0;
  virtual int GetNumCores()=0;
  virtual int GetCPUFrequency()=0;
  virtual void get() = 0;
};


struct CPU_ARM: CPU
{
  CPU_ARM() //:CPU(name,freq,cores,processor)
  {
    //    values.insert(pair<char*,value*>("arm",new value()));
  }
  void get(){}
};

struct CPU_Intel: CPU {

  float getTotalSysMemory();
  float getUsedSysMemory();
  float getFreeSysMemory();

  CPU_Intel():CPU()
  {
    //    values["intel"]=new value(true);
  }
  void get();
  int GetNumProc();
  int GetNumCores();
  int  GetCPUFrequency();

};


void getCPU_info(CPU* cpu);
void get(Soft *s);
