#include <iostream>
#include <map>
#include <list>
#include "../include/Link_xml_info.h"
//#include "../../API_XML_C/include/API_XML.h"

using namespace std;
void WriteInfo(DocumentXML * doc , Soft * soft,CPU *cpu, string compiler, string version_compiler, string option, string diagnostic)

{
  get(soft);
  getCPU_info(cpu);
  beginGroup(doc,"Author",soft->author);
  endGroup(doc);
  beginGroup(doc,"program","nom de fonction a tester");
  beginGroup(doc,"runenv","");// a voir beginGroup avec 2 parametres
  
  /********************************************* HARDWARE***************************************/
  beginGroup(doc,"hardware","");
  addValue(doc,"name");
  detailValue(doc,"name","CPU");
 string s2(" ");	
 int found= cpu->name.find(s2); 
 for (int i=0;i<7;i++)
	 cpu->name.erase(found,1);

 
 setValueAttribute(doc,cpu->name.c_str());
 addValue(doc,"frequency");
  detailValue(doc,"name","frequency");
  setValueAttribute(doc,cpu->freq);
  addValue(doc,"cores");
  detailValue(doc,"name","cores");
  setValueAttribute(doc,cpu->cores);
  addValue(doc,"processor");
  detailValue(doc,"name","processors");
  setValueAttribute(doc,cpu->processor);
  addValue(doc,"cache_line_size");
  detailValue(doc,"name","cache_line_size");
  setValueAttribute(doc,cpu->cache_line_size);
  addValue(doc,"cache_size");
  detailValue(doc,"name","cache_size");
  setValueAttribute(doc,cpu->cache_size);
  beginGroup(doc,"extensions","");

  int start_pos=0;
  int position=-1;
  position = cpu->extensions.find('\n',start_pos);
  string sub = cpu->extensions.substr(start_pos,position);
  addValue(doc,sub.c_str());
  setValueAttribute(doc,sub.c_str());

  while(position <cpu->extensions.size()-1){

    //    cout << sub;
    start_pos=position+1;
    position = cpu->extensions.find('\n',start_pos);
    sub = cpu->extensions.substr(start_pos,position-(start_pos));
    addValue(doc,sub.c_str());
    setValueAttribute(doc,sub.c_str());
    if(start_pos==position)
      break;

  }
  endGroup(doc);
  endGroup(doc);

  /***************************************************SOFTWARE******************/
  beginGroup(doc,"software"," Software Information");
  addValue(doc,"OS");
  detailValue(doc,"name","Operating System");
  string st = soft->os.substr(0,soft->os.size()-1);
  setValueAttribute(doc,st.c_str());
  addValue(doc,"load avg");
  detailValue(doc,"name","Load average CPU before the run");
  setValueAttribute(doc,soft->loadavg);


  /****************Memory**********************/
#ifdef __linux__
   addValue(doc,"Memory Octets ");
  detailValue(doc,"name","Total Memory Octets");
  
  long smem=cpu->getTotalSysMemory();

  setValueAttribute(doc,smem);

  addValue(doc,"Memory ");

  detailValue(doc,"name","Used Memory Octets");
  smem=cpu->getUsedSysMemory();
  setValueAttribute(doc,smem);

  addValue(doc,"Memory ");
  detailValue(doc,"name","Free Memory Octets");
  smem=cpu->getFreeSysMemory();
  setValueAttribute(doc,smem);

#elif WINDOWS
  addValue(doc,"Memory Go ");
  detailValue(doc,"name","Total Memory Octets");
  long smem=cpu->totalMem;
  //printf("SMEM %f \n",smem);
  setValueAttribute(doc,smem);
  
  addValue(doc,"Memory ");
  detailValue(doc,"name","Used Memory Octets");
  smem=cpu->usedMem;
  setValueAttribute(doc,smem);
  
  addValue(doc,"Memory ");
  detailValue(doc,"name","Free Memory Octets");
  smem=cpu->freeMem;
  setValueAttribute(doc,smem);
  
#endif


  /********************** DLL ********************/
  beginGroup(doc,"DLL","");
  for(auto it = soft->dll.begin(); it != soft->dll.end();it++)
    {
      addValue(doc,"");
      string *s=(string*)(*it)->getData();
      
      string st = s->substr(0,s->size());
      st.erase(st.end()-1, st.end());
      setValueAttribute(doc,st.c_str());
    }
  endGroup(doc);
 
  
  /************ variables d'env **************/
  beginGroup(doc,"VarEnv","");
  for(auto it = soft->varenv.begin();it!=soft->varenv.end();it++)
    {
      addValue(doc,"");
      string *s=(string*)it->second->getData();
      setValueAttribute(doc,s->c_str());
      detailValue(doc,"name",it->first);
      }
endGroup(doc);// end varenv
	endGroup(doc);// end software
	endGroup(doc);// end runenv
	beginGroup(doc, "compile", "");
	detailGroup(doc, "compiler", compiler);
	detailGroup(doc, "version_compiler", version_compiler);
	detailGroup(doc, "option", option);
	addValue(doc, "diagnostic"); 
	detailValue(doc,"diagnostic", diagnostic);


	endGroup(doc); // end compile
	endGroup(doc);//end Program
 
}
/*
int main ()
{
  DocumentXML * doc=beginDocument();
  Soft *soft= new Soft;
  CPU_Intel *cpu=new CPU_Intel;
  WriteInfo(doc,soft,cpu);
  printf("load avg %f\n", soft->loadavg);

  endDocument(doc,"test");
 // soft->getMemUsage();
  return 0;
}
*/
