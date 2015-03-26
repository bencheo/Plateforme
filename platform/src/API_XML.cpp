#include "../include/API_XML.h"
#include <iostream>
#include <stdio.h>
#include <string.h>

#include <regex>
#include <map>
#include <stdarg.h>

using namespace std;
using namespace pugi;



DocumentXML* beginDocument()
{
	DocumentXML *doc = new DocumentXML;
	doc->beginDocument();
	return doc;
}

void endDocument(DocumentXML *doc,const char * file_name)
{
	doc->end(file_name);
	delete doc;
}

void beginGroup(DocumentXML *doc, char * tag,string attribute_name)
{
	if(attribute_name.compare("")!=0)
		doc->beginGroup(tag,attribute_name.c_str());
	else
		doc->beginGroup(tag);
}
void detailGroup(DocumentXML *doc, char* attribute_name,  string value)
{
	doc->detailGroup(attribute_name, value);
}

/*void beginGroup(DocumentXML *doc, char * tag, char * attribute_name)
{
doc->beginGroup(tag,attribute_name);
}*/

void addValue(DocumentXML *doc, char *tag)
{
	doc->addValue(tag);
}
void addValue(DocumentXML *doc, string tag)
{
	doc->addValue(tag);
}


void addValue(DocumentXML *doc, const char *tag)
{
	doc->addValue(tag);
}


// add attributes to a Value nodes :encoding other ...
void detailValue(DocumentXML *doc, char* attribute_name,  string value)
{
	doc->detailValue(attribute_name, value);
}
void detailValue(DocumentXML *doc, string attribute_name, string value)
{
	doc->detailValue(attribute_name, value);
}

void detailValue(DocumentXML *doc, char* attribute_name,const char *value)
{
	doc->detailValue(attribute_name, value);
}


// add attributes value type binary (float) to Value nodes
void setValueAttribute(DocumentXML *doc, float value)
{
	char f[128];
	sprintf(f,"%f",value);
	detailValue(doc,"value",f);
	detailValue(doc,"type","float");
	detailValue(doc,"binary","");
}
void setValueAttribute(DocumentXML *doc,long value)
{
	char f[128];
	sprintf(f,"%lu",value);
	detailValue(doc,"value",f);
	detailValue(doc,"type","long");

}

void setValueAttribute(DocumentXML *doc, int value)
{

	char f[128];
	sprintf(f,"%d",value);
	detailValue(doc,"value",f);
	detailValue(doc,"type","int");
}

void setValueAttribute(DocumentXML *doc, double value)
{

	char f[128];
	sprintf(f,"%f",value);
	detailValue(doc,"value",f);
	detailValue(doc,"type","double");
}

void setValueAttribute(DocumentXML *doc, char *value)
{
	detailValue(doc,"value",value);
	detailValue(doc,"type","text");
}


void setValueAttribute(DocumentXML *doc, const char *value)
{
	detailValue(doc,"value",value);
	detailValue(doc,"type","text");
}
/*
void setValueAttribute(DocumentXML *doc, string value)
{
detailValue(doc,"value",value);
detailValue(doc,"type","text");
}
*/
int endGroup(DocumentXML *doc )
{
	return doc->endGroup();

}

