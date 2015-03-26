#include <iostream>
#include <string.h>
#include <sstream>
using namespace std;

struct abstractType{
	virtual void del(void*)=0;
	virtual ~abstractType(){};
	virtual size_t size()const=0;
	virtual std::string toString(void* data) const = 0;
};

template<class T> struct Type:abstractType{
	void del(void*p){
		delete (T*)p;
	}
	size_t size()const{
		return sizeof(T);
	}
	string toString(void* data) const 
	{
		stringstream ss;
		ss << *reinterpret_cast<T*>(data);
		return ss.str();
	}
	size_t size(){
		return sizeof(T);
	}
};

struct valueMesure{
	enum TypeTagMesure { intvalueMesure, floatvalueMesure, stringvalueMesure ,doublevalueMesure,boolvalueMesure, emptyvalueMesure } typeMesure;

  TypeTagMesure typeTag(int){return intvalueMesure;}
  TypeTagMesure typeTag(float){return floatvalueMesure;}
  TypeTagMesure typeTag(double){return doublevalueMesure;}
  TypeTagMesure typeTag(string){return stringvalueMesure;}
  TypeTagMesure getType(){return typeMesure;}
  void* getData(){return data;}
  void *data;
  double getDouble(){return *((double*) data);}
	void setDouble(double val){memcpy(data,&val, sizeof(double));}

  void testvalueMesure();
	abstractType* deleter;
	valueMesure(): typeMesure(emptyvalueMesure), data(0), deleter(0) {}


	size_t size()
	{
		switch (getType()){
		case intvalueMesure:
			return sizeof(int);
		case doublevalueMesure: 
			return sizeof(double);
		case stringvalueMesure : 
			return sizeof(string);
		case floatvalueMesure :
			return sizeof(float);
		default:
			return  -1;
		}
		return -1;
	}
  template<class T> valueMesure(T v): typeMesure(typeTag(v)), data(new T(v))
  {
    deleter=new Type<T> ; 
  }
  
  
  
  void setData(void* d)
  {
    if(deleter){
      size_t s=deleter->size();
      memcpy(data,getData(),s);
    }
    else {
      data =d;
    }
    
  }
  
  template<class T> valueMesure(T* v, bool cpy = false): typeMesure(typeTag(*v)), data(cpy ? new T(*v) : v) 
  {
    if(cpy)
      deleter=new Type<T>;	
    else
      deleter=NULL;
  }
  std::string toString() const { return deleter->toString(data); }
  
  ~valueMesure() {
    if(deleter) 
      {
	deleter->del(data);
	delete deleter;
      }
  }

};






