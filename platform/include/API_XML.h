#include <iostream>
#include "../pugixml/src/pugixml.hpp"
#include "../pugixml/src/pugiconfig.hpp"
#include <stdio.h>
#include <string.h>
#include <map>
//#include <sys/types.h>
#include <regex>
#include <map>
#include <stack>
using namespace std;
using namespace pugi;


class MapGroupValue {
public :
	enum TagType
	{
		GROUP, VALUE,UNKNOWN
	};
	map<string,TagType> MapGV;


	MapGroupValue()
	{
	  MapGV["program"]=GROUP;	
	  MapGV["Author"]=GROUP;
	  MapGV["measure"]=GROUP;
		MapGV["operation"]=VALUE;
		MapGV["paramsettings"]=GROUP;
		MapGV["data-instance"]=GROUP;
		MapGV["data"]=GROUP;
		MapGV["memory"]=VALUE;
		MapGV["throughput"]=VALUE;
		MapGV["time"]=VALUE;
		MapGV["data"]=GROUP;
		MapGV["output"]=GROUP;
		MapGV["compile"]=VALUE;
		MapGV["runenv"]=VALUE;
		MapGV["opparams"]=VALUE;
		MapGV["float"]=VALUE;
		MapGV["group"]=UNKNOWN;
		MapGV["value"]=UNKNOWN;
	}

	TagType findInMap(const char* name)
	{
		auto it =MapGV.find(name);
		if(it==MapGV.end())
			return UNKNOWN;
		else
			return it->second;
	}

};


class DocumentXML
{
	MapGroupValue mapgv;
	xml_document doc;
	xml_node benchmark; // root
	xml_node first_value;
	xml_node *lastgroup;
	xml_node *lastvalue;
	

public :
	stack<xml_node> stack_group;
	xml_document* getDoc(){return &doc;}
	xml_node* getBenchNode(){return &benchmark;}
	xml_node* getLastGroup(){return lastgroup;}
	xml_node* getLastValue(){return lastvalue;}
	void setLastGroup(xml_node addG){ *lastgroup=addG;}
	void setLastValue(xml_node addV){ *lastvalue=addV;}
	DocumentXML(){
		lastgroup=&benchmark;
		lastvalue=&first_value;
	}
	virtual ~DocumentXML(){}

	// begin adds DOCTYPE and benchmark  nodes in the xml file.
	void beginDocument()
	{
		xml_node node_d;
		if((node_d=doc.append_child(node_doctype))==NULL)
		{
			printf("append DocTYPE error %d\n",__LINE__);
			exit(-1);
		}
		node_d.set_value( "benchmark SYSTEM \"Format.dtd\"");
		benchmark = doc.append_child("benchmark");

		stack_group.push(benchmark);
	}
	void detailGroup(char* attribute_name,  string value)
	{
		if(lastgroup==NULL)
			printf("error vlaue null %d \n",__LINE__);
		if((getLastGroup()->append_attribute(attribute_name)=value.c_str())==NULL){}
		  //printf("append attribute detail group \n");
	}
	void addGroup(char* node_name,char * name)
	{

		xml_node *add1=getLastGroup();//->append_child(node_name));
		xml_node  add=add1->append_child(node_name);
		if(add ==NULL)
		{
			printf(" add NULL %d %s \n",__LINE__,__FILE__);
			exit(-1);
		}
		setLastGroup(add);
		stack_group.push(add);
		getLastGroup()->append_attribute("name")=name;
		//	printf("add group %s \n", getLastGroup()->name());
	}
	void addGroup(char* node_name)
	{

		xml_node *add1=getLastGroup();//->append_child(node_name));
		xml_node  add=add1->append_child(node_name);
		if(add ==NULL)
		{
			printf(" add NULL %d %s \n",__LINE__,__FILE__);
			exit(-1);
		}
		setLastGroup(add);
		stack_group.push(add);
		//		printf("add group %s \n", getLastGroup()->name());
	}
	void addGroup(char* node_name,const char * name)
	{

		xml_node *add1=getLastGroup();//->append_child(node_name));
		xml_node  add=add1->append_child(node_name);
		if(add ==NULL)
		{
			printf(" add NULL %d \n",__LINE__);
			exit(-1);
		}
		setLastGroup(add);
		stack_group.push(add);
		getLastGroup()->append_attribute("name")=name;
		//printf("add group %s \n", getLastGroup()->name());
	}


	void addValue(char * node_name)
	{
		xml_node add;
		int res =mapgv.findInMap(node_name);
		if(res==mapgv.UNKNOWN)
		{
			add=getLastGroup()->append_child("value");
		}
		else
		{
			add=getLastGroup()->append_child(node_name);
		}
		if(add==NULL){
			printf("value unknow %s \n",node_name);
			exit(-1);
		}
		setLastValue(add);

	}
	void addValue(string node_name)
	{
		xml_node add;
		int res =mapgv.findInMap(node_name.c_str());
		if(res==mapgv.UNKNOWN)
		{
			add=getLastGroup()->append_child("value");
		}
		else
		{
			add=getLastGroup()->append_child(node_name.c_str());
		}
		if(add==NULL){
			printf("value unknow %s \n",node_name.c_str());
			exit(-1);
		}
		setLastValue(add);
	}
	void addValue(const char * node_name)
	{

		xml_node add;
		int res =mapgv.findInMap(node_name);
		if(res==mapgv.UNKNOWN)
		{
			add=getLastGroup()->append_child("value");
		}
		else
		{
			add=getLastGroup()->append_child(node_name);
		}
		if(add==NULL){
			printf("value unknow %s \n",node_name);
			exit(-1);
		}

		setLastValue(add);
	}



	void  beginGroup(char * node_name,char *name)
	{
		int res =mapgv.findInMap(node_name);
		if(res==mapgv.UNKNOWN)
		{
			addGroup("group",node_name);
		}
		else
			addGroup(node_name,name);
	}


	void  beginGroup(char * node_name)
	{
		int res =mapgv.findInMap(node_name);
		if(res==mapgv.UNKNOWN)
		{
			addGroup("group",node_name);
		}
		else
			addGroup(node_name);
	}

	void  beginGroup(char * node_name,const char *name)
	{
		int res =mapgv.findInMap(node_name);
		if(res==mapgv.UNKNOWN)
		{
			addGroup("group",node_name);
		}
		else
			addGroup(node_name,name);
	}


	void detailValue(char* attribute,char* value)
	{
		if(lastvalue==NULL)
			printf("error vlaue null %d \n",__LINE__);
		if((getLastValue()->append_attribute(attribute)=value)==NULL){}
		  //			printf("append attribute detail value \n");
	}

	void detailValue(char* attribute,const string value)
	{
		if(lastvalue==NULL)
			printf("error vlaue null %d \n",__LINE__);
		if((getLastValue()->append_attribute(attribute)=value.c_str())==NULL){}
		  //printf("append attribute detail value \n");
	}

	void detailValue(string attribute,string value)
	{
		if(lastvalue==NULL)
			printf("error vlaue null %d \n",__LINE__);
		if((getLastValue()->append_attribute(attribute.c_str())=value.c_str())==NULL){}
		  //printf("append attribute detail value \n");
	}
	int endGroup()
	{
		stack_group.pop();
		xml_node &top= stack_group.top();
		setLastGroup(top);
		return stack_group.size();
	}

	void end(const  char* name_file_xml)
	{
		char file_name[128];
		strcpy(file_name,name_file_xml);
		if(!regex_match(name_file_xml,regex(".*\\.xml")))
		{
			strcat(file_name,".xml");
		}
		doc.save_file(file_name,"\t",pugi::format_default & ~pugi::format_no_escapes);

	}

};




DocumentXML* beginDocument();
void endDocument(DocumentXML *doc,const char* file_name);
void beginGroup(DocumentXML *doc, char * tag,string attribute_name="");
//void beginGroup(DocumentXML *doc, char * tag,char * attribute_name);
void addValue(DocumentXML *doc, char *tag);
void addValue(DocumentXML *doc, const char *tag);
void addValue(DocumentXML *doc, string tag);
void detailValue(DocumentXML *doc, char* attribute_name, string value);
int endGroup(DocumentXML *doc);
void setValueAttribute(DocumentXML *doc, float value);
void setValueAttribute(DocumentXML *doc, int value);
void setValueAttribute(DocumentXML *doc, char* value);
void setValueAttribute(DocumentXML *doc, double value);
void setValueAttribute(DocumentXML *doc, long value);
void setValueAttribute(DocumentXML *doc, const char* value);
void setValueAttribute(DocumentXML *doc, string value);
void detailGroup(DocumentXML *doc, char* attribute_name,  string value);
void detailValue(DocumentXML *doc, string attribute_name, string value);
