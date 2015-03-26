#pragma comment(lib, "psapi")

#include <iostream>
#include <string.h>

using namespace std;

struct abstractDeleter{
  virtual void del(void*)=0;
  virtual ~abstractDeleter(){};
};

template<class T> struct deleter:abstractDeleter{
  void del(void*p){
    delete (T*)p;
  }
};

struct value{
  enum TypeTag { intValue, floatValue, stringValue ,doubleValue,boolValue, emptyValue } type;
  TypeTag typeTag(int);
  TypeTag typeTag(float);
  TypeTag typeTag(double);
  TypeTag typeTag(string);
  TypeTag typeTag(int*);
  TypeTag typeTag(float*);
  TypeTag typeTag(double*);
  TypeTag typeTag(string*);
  void *data;
  TypeTag getType();
  void* getData();
  abstractDeleter* deleter1;
  value(): type(emptyValue), data(0), deleter1(0) {}
  template<class T> value(T v): type(typeTag(v)), data(new T(v)), deleter1(new deleter<T>) {  }
  template<class T> value(T* v, bool cpy = false): type(typeTag(*v)), data(cpy ? new T(*v) : v), deleter1(cpy ? new deleter<T> : 0) {}


  ~value() {
    if(deleter1) {
      deleter1->del(data);
      delete deleter1;
    }

  }
};

