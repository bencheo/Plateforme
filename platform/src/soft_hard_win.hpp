#define WINVER 0x5000
#pragma comment(lib, "psapi")	

// Software.cpp : Defines the entry point for the console application.
//
#ifdef _WIN32
#ifndef WIN32
#define WIN32 
#endif
#endif

#include <string.h>
#include <iostream>
#include <iomanip>
#include <map>
#include <list>
#include <sstream>
#include "../include/soft_hard.h"
#include <stdio.h>
#include <Lmcons.h>
using namespace std;

#include <tchar.h>

#include <psapi.h>

#define TypeTag value::TypeTag




TypeTag value::typeTag(int) { return intValue; }

TypeTag value::typeTag(float) { return floatValue; }
TypeTag value::typeTag(double) { return doubleValue; }
TypeTag value::typeTag(string) { return stringValue; }
TypeTag value::typeTag(int*) { return intValue; }
TypeTag value::typeTag(float*) { return floatValue; }
TypeTag value::typeTag(double*) { return doubleValue; }
TypeTag value::typeTag(string*) { return stringValue; }
TypeTag value::getType(){ return type;}
void* value::getData ( ) { return data ; }



#define DIV (1024*1048576)  // Gbytes

#define cpuid(func,ax,bx,cx,dx)                                         \
	__asm__ __volatile__ ("cpuid":                                        \
	"=a" (ax), "=b" (bx), "=c" (cx), "=d" (dx) : "a" (func));



int CPU_Intel::GetCPUFrequency()
{
	LARGE_INTEGER nCntFrequency;
	if(!::QueryPerformanceFrequency(&nCntFrequency))
	{
		return 0; // high-resolution performance counter
		// not supported
	}
	LARGE_INTEGER nCnt0, nCnt1;
	ULONG  nTs0, nTs1;

	HANDLE hThread  = ::GetCurrentThread();
	int nPriority   = ::GetThreadPriority(hThread);
	::SetThreadPriority(hThread, THREAD_PRIORITY_TIME_CRITICAL);
	::QueryPerformanceCounter(&nCnt1);

	asm(".intel_syntax noprefix\n"
		".byte 0x0f \n .byte 0x31\n"
			" mov %0 ,EAX\n" : "=r"(nTs0)
	);
	nCnt0.LowPart  = nCnt1.LowPart;

	while(((ULONG)nCnt1.LowPart - (ULONG)nCnt0.LowPart) < 10000)
	{
		::QueryPerformanceCounter(&nCnt1);
		asm(".intel_syntax noprefix\n"
			".byte 0x0f \n .byte 0x31 \n"
			"	MOV  %0, EAX\n": "=r"(nTs1)
		);
	}
	::SetThreadPriority(hThread, nPriority);

	ULONG nCycles = nTs1 - nTs0;
	ULONG nTicks  = (ULONG)nCnt1.LowPart - nCnt0.LowPart;
	nTicks *= 10000;
	nTicks /= (nCntFrequency.LowPart / 100);

	UINT nFrequency = nCycles / nTicks;
	return nFrequency; // MHz
}
#define INFO_BUFFER_SIZE 32767

string Soft::getAuthor()
{

	TCHAR  infoBuf[INFO_BUFFER_SIZE];
	DWORD  bufCharCount = INFO_BUFFER_SIZE;
   // Get and display the user name. 
    if( !GetUserName( infoBuf, &bufCharCount ) )
		printf("GetUserName+++++++++++++++++++++++++++++++++++++++++++++\n" ); 
	author=infoBuf;
  return author;
}

int CPU_Intel::GetNumProc(){
	SYSTEM_INFO sysinfo;
	GetSystemInfo( &sysinfo );
	return sysinfo.dwNumberOfProcessors;
}
int CPU_Intel::GetNumCores()
{
	int regs[4];
	cpuid( 4,regs[0],regs[1],regs[2],regs[3]);
	cores = ((regs[0] >> 26) & 0x3f) + 1;
	//cout << " cores number "<< cores << endl;
	return cores;
}

float CPU_Intel::getTotalSysMemory()
{

	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
//	cout << " TOTAL SIZE PHY :" << (float)status.ullTotalPhys/DIV<< endl;
	totalMem= (float) status.ullTotalPhys;
	return totalMem;
}


float CPU_Intel::getUsedSysMemory()
{
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	//cout << " used SIZE PHY :" << (float)(status.ullTotalPhys- status.ullAvailPhys)/DIV << endl;
	usedMem= (float) (status.ullTotalPhys-status.ullAvailPhys);
	return usedMem;
}
float CPU_Intel::getFreeSysMemory()
{
	MEMORYSTATUSEX status;
	status.dwLength = sizeof(status);
	GlobalMemoryStatusEx(&status);
	//cout << " free SIZE PHY :" << (float)status.ullAvailPhys/DIV << endl;
	freeMem= (float)status.ullAvailPhys;
	return freeMem;
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


/*
#ifdef WIN32
	__cpuid(CPUInfo, 0);
#else*/
	cpuid(0,CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
//#endif
	nIds = CPUInfo[0];
	memset(CPUString, 0, sizeof(CPUString));
	*((int*)CPUString) = CPUInfo[1];
	*((int*)(CPUString+4)) = CPUInfo[3];
	*((int*)(CPUString+8)) = CPUInfo[2];

	// Get the information associated with each valid Id
	for (i=0; i<=nIds; ++i)
	{/*
#ifdef WIN32
		__cpuid(CPUInfo, i);
#else*/
		cpuid(i,CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
//#endif
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
/*#ifdef WIN32
	__cpuid(CPUInfo, 0x80000000);
#else*/
	cpuid(0x80000000,CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
//#endif
	nExIds = CPUInfo[0];
	memset(CPUBrandString, 0, sizeof(CPUBrandString));

	// Get the information associated with each extended ID.
	for (i=0x80000000; i<=nExIds; ++i)
	{
/*#ifdef WIN32
		__cpuid(CPUInfo, i);
#else*/
		cpuid(i,CPUInfo[0],CPUInfo[1],CPUInfo[2],CPUInfo[3]);
//#endif
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

	if  (nIds >= 1)
	{
		if  (nFeatureInfo || bSSE3NewInstructions ||
			bMONITOR_MWAIT || bCPLQualifiedDebugStore ||
			bThermalMonitor2)
		{

			if  (bSSE3NewInstructions){
				extensions.append("SSE3 New Instructions");
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
	values["frequency"]= new value(freq);
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


BOOL GetOSDisplayString( string *m_sStr)
{
	OSVERSIONINFO OSversion;

	OSversion.dwOSVersionInfoSize=sizeof(OSVERSIONINFO);
	::GetVersionEx(&OSversion);

	stringstream s;

	switch(OSversion.dwPlatformId)
	{
	case VER_PLATFORM_WIN32s:
		//.("Windows %d.%d",OSversion.dwMajorVersion,OSversion.dwMinorVersion);
		break;
	case VER_PLATFORM_WIN32_WINDOWS:

		if(OSversion.dwMinorVersion==0)
			s<<"Windows 95";
		else
			if(OSversion.dwMinorVersion==10)
				s<<"Windows 98";
			else
				if(OSversion.dwMinorVersion==90)
					s<<"Windows Me";
		break;
	case VER_PLATFORM_WIN32_NT:

		if(OSversion.dwMajorVersion==5 && OSversion.dwMinorVersion==0)
			s << "Windows 2000 With "<<  OSversion.szCSDVersion;
		else
			if(OSversion.dwMajorVersion==5 &&   OSversion.dwMinorVersion==1)
				s <<"Windows XP "<< OSversion.szCSDVersion;
			else
				if(OSversion.dwMajorVersion<=4)
					s<<"Windows NT "<< OSversion.dwMajorVersion <<"."<< OSversion.dwMinorVersion <<OSversion.szCSDVersion;
				else
					//for unknown windows/newest windows version
					s << "Windows " << OSversion.dwMajorVersion<<"." << OSversion.dwMinorVersion;
		*m_sStr = s.str();
		//cout << m_sStr->c_str() << endl;;

		break;

	}
	return true;
}

double Soft::getCpuUsage(){

	FILE* f1;
	char *time=(char*)malloc(sizeof(char)*10);


#ifdef WIN32 	
	f1=popen("wmic cpu get loadpercentage|findstr /R \"[0-9]\"","r");
#else //LINUX 
	f1=popen("cat /proc/loadavg | cut -c 1-4","r");
#endif
	if(f1==NULL)
		printf("erreur  file null ine FIle %s\n", __FILE__);
	if(fgets(time,sizeof(char)*10,f1)==NULL)
	{
		printf("error fgets null\n");		
	}
#ifdef WIN32
	loadavg=atof(time)/100;
	return atof(time);
#else
	return atof(time);
#endif
}/*
 double Soft::getCpuUsage()
 {
 sigar_t *sigar;
 sigar_open(&sigar);
 sigar_loadavg_t ldavg;
 sigar_loadavg_get(sigar,&ldavg);
 sigar_close(sigar);
 loadavg=ldavg.loadavg[0];
 return ldavg.loadavg[0];
 }
 */
/*void Soft::getMemUsage()
{
sigar_mem_t mem;
sigar_t* sigar;
sigar_open(&sigar);
sigar_mem_get(sigar,&mem);
sigar_close(sigar);
printf(" mem total %lu \n", mem.total);
}*/


void Soft::getVarsEnv(char* var)
{
#if 0
	std::string key ;
	char *val;
	size_t len;
	getenv_s(&len,NULL,0,var);

	val=(char*)malloc (sizeof(char)*len);
	getenv(&len,val,len,var);


	if (val != NULL) {
		string retval (val);
		varenv[var]=new value(retval);
	}
	free(val);
	#else	
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
  #endif
}

string Soft::getOS()
{

	string *szOS=new string;
	if( GetOSDisplayString( szOS ) )
	{
	}
	os=*szOS;
	return "";

}
int Soft::getDll(ULONG processID)
{
	HMODULE hMods[1024];
	HANDLE hProcess;
	DWORD cbNeeded;
	unsigned int i;

	// Print the process identifier.

	// Get a handle to the process.

	hProcess = OpenProcess( PROCESS_QUERY_INFORMATION |
		PROCESS_VM_READ,
		FALSE, processID );
	if (NULL == hProcess)
		return 1;

	// Get a list of all the modules in this process.

	if(EnumProcessModules(hProcess, hMods, sizeof(hMods), &cbNeeded))
	{
		for ( i = 0; i < (cbNeeded / sizeof(HMODULE)); i++ )
		{
			TCHAR szModName[MAX_PATH];

			// Get the full path to the module's file.

			if ( GetModuleFileNameEx( hProcess, hMods[i], szModName,
				sizeof(szModName) / sizeof(TCHAR)))
			{
				// Print the module name and handle value.

				// _tprintf( TEXT("\t%s \n"), szModName );
				char szModName2[MAX_PATH];
				for(int i =0;i<MAX_PATH;i++)
				{
					szModName2[i]=szModName[i];
				}
				string dll_line=szModName2;
				dll.push_back(new value(dll_line));
			}
		}
	}
	// Release the handle to the process.

	CloseHandle( hProcess );

	return 0;
}
void get(Soft* s)
{
cout << "get soft info" <<endl;
	s->getOS();
	s->getVarsEnvAll();
	s->getDll(GetCurrentProcessId());
	s->getAuthor();
	s->getCpuUsage();
}


void Soft::getVarsEnvAll(){
	this->getVarsEnv("PATH");
#ifdef __linux__

	this->getVarsEnv("SHELL");
#endif
}
