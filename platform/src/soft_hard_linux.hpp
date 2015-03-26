
// Software.cpp : Defines the entry point for the console application.
//
#include <string.h>
#include <sys/sysinfo.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <list>
#include <sstream>
#include <sys/types.h>
#include <unistd.h>
#include "../include/soft_hard.h"

using namespace std;
#define DIV (1048576*1024)  
#define TypeTag value::TypeTag

TypeTag value::typeTag(int){return intValue;}
TypeTag value::typeTag(float){return floatValue;}
TypeTag value::typeTag(double){return doubleValue;}
TypeTag value::typeTag(string){return stringValue;}
TypeTag value::typeTag(int*){return intValue;}
TypeTag value::typeTag(float*){return floatValue;}
TypeTag value::typeTag(double*){return doubleValue;}
TypeTag value::typeTag(string*){return stringValue;}
TypeTag value::getType(){return type;}
void*   value::getData(){return data;}

int Soft::getDll(ulong pid)
{
  int my_pid=getpid();
  char* dll_pmap=(char*) malloc(sizeof(char)*4096);
  char * line =(char*) malloc (sizeof(char)*128);
  FILE *f1;
  int i=0;
  ostringstream oss;
  string toMap="pmap ";
  oss << toMap << my_pid << " | cut -c 33- | grep \"/lib\"";
  toMap=oss.str();
  f1=  popen(toMap.c_str(),"r");
  //  printf("line %d \n",__LINE__);
  do{
    line=fgets(line,sizeof(char)*4096,f1);
    if(line !=NULL)
      {
        string l=line;
        dll.push_back(new value(l));
      }

  }  while(line!=NULL);
  
  free(dll_pmap);
  free(line);
  pclose(f1);  return 1;
}
string Soft::getOS()
{
  FILE* f1;
  char *osname=(char*)malloc(sizeof(char)*16);
  f1=popen("uname -o ","r");
  fgets(osname,sizeof(char)*16,f1);
  os=osname;
  return os;
}
void Soft::getVarsEnv(char* var)
{
  char *val;
  val=(char*)malloc (sizeof(char)*1024);
  val=getenv(var);
  string undef ="UNDEF";


  if (val != NULL) {
       string retval (val);
    varenv[var]=new value(retval);
  }
  else {
       varenv[var]=new value(undef);
  }
}
string Soft::getAuthor()
{
 FILE* f1;
  char *osname=(char*)malloc(sizeof(char)*16);
  f1=popen("echo $USER ","r");
  fgets(osname,sizeof(char)*16,f1);
  author=osname;
  string none="";
  string toerase="&#10";
  author.erase(author.end()-1,author.end());
  return author;

}
void Soft::getVarsEnvAll()
{
  this->getVarsEnv("PATH");
  this->getVarsEnv("SHELL");
  this->getVarsEnv("LIB");
 
}


double Soft::getCpuUsage()
{
  /*
  sigar_t *sigar;
  sigar_open(&sigar);
  sigar_loadavg_t ldavg;
  sigar_loadavg_get(sigar,&ldavg);
  sigar_close(sigar);
  loadavg=ldavg.loadavg[0];
  return ldavg.loadavg[0];*/
  double tabloadavg[3];
  int nelem=3;
  if(getloadavg(tabloadavg,nelem)==-1)
    loadavg=-1;
  else
    loadavg=tabloadavg[0];
  return loadavg;

}

void  Soft::getMemUsage()
{
  struct sysinfo memInfo;
  sysinfo (&memInfo);
  totalMem = memInfo.totalram;
  usedMem = memInfo.totalram - memInfo.freeram;
  freeMem=memInfo.freeram;
 

 

  /*
  sigar_mem_t mem;
  sigar_t* sigar;
  sigar_open(&sigar);
  sigar_mem_get(sigar,&mem);
  sigar_close(sigar);
  totalMem = mem.total/DIV;
  usedMem  = mem.used/DIV;
  freeMem  = mem.free/DIV;*/
   
}

void get(Soft* s)
{
  //  printf("%s %d get(soft)\n", __FILE__,__LINE__);
  s->getOS();
  s->getVarsEnvAll();
  s->getDll(getpid());
  s->getCpuUsage();
  // s->getMemUsage();
  s->getAuthor();
}

#define cpuid(func,ax,bx,cx,dx)                                         \
  __asm__ __volatile__ ("cpuid":                                        \
			"=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func));



float CPU_Intel::getUsedSysMemory()
{
  struct sysinfo info;
  sysinfo( &info );
  float total=(float)(info.totalram * (size_t)info.mem_unit - info.freeram *(size_t)info.mem_unit);
  
  return total;
}

float CPU_Intel::getTotalSysMemory()
{
  struct sysinfo info;
  sysinfo( &info );
  float totalMem=(float)info.totalram * (size_t)info.mem_unit;

  return totalMem;
}

float CPU_Intel::getFreeSysMemory()
{

  struct sysinfo info;
  sysinfo( &info );
  float total=(float)info.freeram * (size_t)info.mem_unit;
  float freeMem=total;
  return freeMem;
}

int CPU_Intel::GetNumProc()
{
  int nb=0;  
   char * buff=(char*) malloc (sizeof(char)*200);
   fgets(buff,sizeof(char)*200,popen("nproc","r")); 
   nb=atoi(buff);
   free(buff);
   return nb;
}

int CPU_Intel::GetNumCores()
{
  int nb=0;
  char * buff=(char * ) malloc (sizeof(char)*200);
  fgets(buff,sizeof(char)*200,popen("cat /proc/cpuinfo |grep \"cpu cores\"  | uniq","r"));
  int len=strlen(buff);
  nb=atoi((buff)+len-2);
  free(buff);
  return nb;
}

int CPU_Intel::GetCPUFrequency()
{
  int nb;
  char * buff =(char*)malloc(sizeof(char)*200);
  fgets(buff,sizeof(char)*200,popen(" cat /sys/devices/system/cpu/cpu0/cpufreq/scaling_max_freq","r"));
  nb=atof(buff)/1000000.0;
  return nb;
}

void CPU_Intel::get()

{  

  const char* szFeatures[] =
    {
      "x87 FPU On Chip",
      "Virtual-8086 Mode Enhancement",
      "Debugging Extensions",
      "Page Size Extensions",
      "Time Stamp Counter",
      "RDMSR and WRMSR Support",
      "Physical Address Extensions",
      "Machine Check Exception",
      "CMPXCHG8B Instruction",
      "APIC On Chip",
      "Unknown1",
      "SYSENTER and SYSEXIT",
      "Memory Type Range Registers",
      "PTE Global Bit",
      "Machine Check Architecture",
      "Conditional Move/Compare Instruction",
      "Page Attribute Table",
      "Page Size Extension",
      "Processor Serial Number",
      "CFLUSH Extension",
      "Unknown2",
      "Debug Store",
      "Thermal Monitor and Clock Ctrl",
      "MMX Technology",
      "FXSAVE/FXRSTOR",
      "SSE Extensions",
      "SSE2 Extensions",
      "Self Snoop",
      "Hyper-threading Technology",
      "Thermal Monitor",
      "Unknown4",
    "Pend. Brk. EN."
    };

  bool sse_ext,sse2_ext,sse3_ext,hyperthreading;

  char CPUString[0x20];
  char CPUBrandString[0x40];
  int CPUInfo[4] = {-1};
  int nSteppingID = 0;
  int nModel = 0;
  int nFamily = 0;
  int nProcessorType = 0;
  int nExtendedmodel = 0;
  int nExtendedfamily = 0;
  int nBrandIndex = 0;
  int nCLFLUSHcachelinesize = 0;
  int nAPICPhysicalID = 0;
  int nFeatureInfo = 0;
  int nCacheLineSize = 0;
  int nL2Associativity = 0;
  int nCacheSizeK = 0;
  int nRet = 0;
  unsigned    nIds, nExIds, i;
  bool    bSSE3NewInstructions = false;
  bool    bMONITOR_MWAIT = false;
  bool    bCPLQualifiedDebugStore = false;
  bool    bThermalMonitor2 = false;
#ifdef WIN32
  __cpuid(CPUInfo, 0);
#else
  cpuid(0,CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
#endif
  nIds = CPUInfo[0];
  memset(CPUString, 0, sizeof(CPUString));
  *((int*)CPUString) = CPUInfo[1];
  *((int*)(CPUString+4)) = CPUInfo[3];
  *((int*)(CPUString+8)) = CPUInfo[2];

  // Get the information associated with each valid Id
  for (i=0; i<=nIds; ++i)
    {
#ifdef WIN32
      __cpuid(CPUInfo, i);
#else
      cpuid(i,CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
#endif
      // Interpret CPU feature information.
      if  (i == 1)
        {
          nSteppingID = CPUInfo[0] & 0xf;
          nModel = (CPUInfo[0] >> 4) & 0xf;
          nFamily = (CPUInfo[0] >> 8) & 0xf;
          nProcessorType = (CPUInfo[0] >> 12) & 0x3;
          nExtendedmodel = (CPUInfo[0] >> 16) & 0xf;
          nExtendedfamily = (CPUInfo[0] >> 20) & 0xff;
          nBrandIndex = CPUInfo[1] & 0xff;
          nCLFLUSHcachelinesize = ((CPUInfo[1] >> 8) & 0xff) * 8;
          nAPICPhysicalID = (CPUInfo[1] >> 24) & 0xff;
          bSSE3NewInstructions = (CPUInfo[2] & 0x1) || false;
          bMONITOR_MWAIT = (CPUInfo[2] & 0x8) || false;
          bCPLQualifiedDebugStore = (CPUInfo[2] & 0x10) || false;
          bThermalMonitor2 = (CPUInfo[2] & 0x100) || false;
          nFeatureInfo = CPUInfo[3];
        }
    }
  // Calling __cpuid with 0x80000000 as the InfoType argument
  // gets the number of valid extended IDs.
#ifdef WIN32
  __cpuid(CPUInfo, 0x80000000);
#else
  cpuid(0x80000000,CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
#endif

  nExIds = CPUInfo[0];
  memset(CPUBrandString, 0, sizeof(CPUBrandString));

  // Get the information associated with each extended ID.
  for (i=0x80000000; i<=nExIds; ++i)
    {
#ifdef WIN32
      __cpuid(CPUInfo, i);
#else
      cpuid(i,CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
#endif
      // Interpret CPU brand string and cache information.
      if  (i == 0x80000002)
        memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
      else if  (i == 0x80000003)
        memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
      else if  (i == 0x80000004)
        memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
      else if  (i == 0x80000006)
        {
          nCacheLineSize = CPUInfo[2] & 0xff;
          nL2Associativity = (CPUInfo[2] >> 12) & 0xf;
          nCacheSizeK = (CPUInfo[2] >> 16) & 0xffff;
        }
    }
  // Display all the information in user-friendly format.

  if  (nIds >= 1)
    {
      if  (nFeatureInfo || bSSE3NewInstructions ||
             bMONITOR_MWAIT || bCPLQualifiedDebugStore ||
           bThermalMonitor2)
        {

          if  (bSSE3NewInstructions){
            extensions.append("SSE3 New Instructions\n");
	    sse3_ext=true;
	  }

          i = 0;
          nIds = 1;
          while (i < (sizeof(szFeatures)/sizeof(const char*)))
            {
              if  (nFeatureInfo & nIds)
                {

                  //strcat_s(extensions,strlen(szFeatures[i]),szFeatures[i]);
                
		  extensions.append(szFeatures[i]);
                  extensions.append("\n");

                }

              nIds <<= 1;
              ++i;
            }
        }
    }

  if  (nExIds >= 0x80000004)
    {
      name.append(CPUBrandString);
    }
  if  (nExIds >= 0x80000006)
    {
      cache_line_size=nCacheLineSize;
      cache_size=nCacheSizeK;
    }

   freq =(float)GetCPUFrequency();
   cores=GetNumCores();
   processor=GetNumProc();
  values["frequency"]= new value(freq)  ;
  values["processor"]=new value(processor);
  values["name"]=new value(name);
  values["cores"]=new value(cores);
  values["cache_line_size"]=new value(cache_line_size);
  values["cache_size"]=new value(cache_size);
  values["extensions_available"]= new value(extensions);
  values["used memory"]=new value(getUsedSysMemory());
  values["free memory"]=new value(getFreeSysMemory());
  values["total memory"]= new value(getTotalSysMemory());
}


void getCPU_info(CPU* cpu)
{
  cpu->get();
}


