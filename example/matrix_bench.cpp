
#include "matrix_bench.h"

int main (int argc, char ** argv){
  string  compileTag[4]={"","","",""};
  if(argc>1)
    for(int i=1; i < argc; i++)
      compileTag[i-1]=argv[i];

  double sizeA=100;
  double sizeB=100;
  double testC=3;

  MatMultiplier *MM= new MatMultiplier(sizeA, sizeB,testC);
  MM->defaultGenerator= new MatrixPairGen;
  StrategieLoop *sl = new StrategieLoop(1);
  auto *bench = new Benchmark(MM,MM->defaultGenerator,sl);
  string file_name=bench->NextFile("matrix_bench");

  bench->setMakeInfo(compileTag[0], compileTag[1], compileTag[2], compileTag[3] );
  bench->Start_Benchmarking();

  //bench->default_runs();
  Range range;
  range.begin=51; range.end=52;	range.step=1;
  map<string,Range> param;
  string s("sizeA");
  string ss("sizeB");
  string sss("testC");
  param[s]= range;
  param[ss]=range;
  param[sss]=range;
  bench->parameters(param);
  bench->End_Benchmarking(file_name);

  //	delete bench;
  return 0;
}

