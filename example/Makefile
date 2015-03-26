ifeq ($(OS),Windows_NT)
PLATFORM_PATH=../platform
LIB_PATH=$(PLATFORM_PATH)/lib
EXECUTABLE=Bench.exe
DIAG_FILE=diagnostic.txt
CXX=g++
MyCXXFLAGS=-O3

CXXFLAGS=-std=c++0x -I$(PLATFORM_PATH)/include
LDFLAGS= -w -L$(LIB_PATH) -lBenchmark  -Wl,-rpath=$(LIB_PATH) -lpsapi 

SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)
all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) -o $@  $^ $(LDFLAGS)

$(OBJECTS): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(MyCXXFLAGS) -c $< -o $(OBJECTS)  >$(DIAG_FILE)

clean: 
	del $(OBJECTS) $(EXECUTABLE) diagnostic.txt

else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)

PLATFORM_PATH=../platform
LIB_PATH=$(PLATFORM_PATH)/lib
EXECUTABLE=Bench
DIAG_FILE=diagnostic.txt
CXX=g++
MyCXXFLAGS=-O3

CXXFLAGS=-std=c++11 -I$(PLATFORM_PATH)/include
LDFLAGS=-L$(LIB_PATH) -lBenchmark  -Wl,-rpath=$(LIB_PATH)

SOURCES=$(wildcard *.cpp)
OBJECTS=$(SOURCES:.cpp=.o)

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS) 
	$(CXX) -o $@  $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(MyCXXFLAGS) -c $< -o $(OBJECTS)  >$(DIAG_FILE) 2>$(DIAG_FILE) 

clean: 
	rm -f $(OBJECTS) $(EXECUTABLE) $(DIAG_FILE)

endif 
endif