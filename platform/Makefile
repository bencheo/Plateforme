ifeq ($(OS),Windows_NT)

CC=g++
BIN=bin
INC=include
OBJ=obj
SRC=src
CFLAGS=-masm=intel -std=c++0x -U__STRICT_ANSI__ -w
LDFLAGS=
CPPFLAGS=
MyLIB=lib


LIB= $(OBJ)/API_XML.o $(OBJ)/Link_xml_info.o pugixml/src/pugixml.o $(OBJ)/soft_hard.o 

all:  $(OBJ)/soft_hard.o  $(OBJ)/API_XML.o    $(OBJ)/Link_xml_info.o  pugixml  ${MyLIB}/Benchmark.lib

pugixml:pugixml\src\pugixml.cpp
	$(CC) -c -o pugixml\src\pugixml.o pugixml\src\pugixml.cpp $(CFLAGS) -fPIC
	
obj:
	mkdir -p $@
lib:
	mkdir -p $@
bin:
	mkdir -p $@

$(OBJ)/API_XML.o: $(SRC)/API_XML.cpp 
	$(CC) -c -o $(OBJ)/API_XML.o  $(SRC)/API_XML.cpp  -I$(INC) $(CFLAGS) -fPIC

$(OBJ)/soft_hard.o: $(SRC)/soft_hard.cpp  
	$(CC) -c  -o  $(OBJ)/soft_hard.o  $(SRC)/soft_hard.cpp $(CFLAGS) -I$(INC)    -fPIC

$(OBJ)/Link_xml_info.o: $(SRC)/Link_xml_info.cpp   
	$(CC)  -c  -o $(OBJ)/Link_xml_info.o $(SRC)/Link_xml_info.cpp $(CFLAGS) -I$(INC) -fPIC

$(MyLIB)/Benchmark.lib: 
	ar rcs $(MyLIB)/libBenchmark.a   $(LIB) 

clean:
	del $(BIN)\* $(OBJ)\* $(MyLIB)\*

else
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Linux)


TARGET=lib/libBenchmark.so
CXX=g++
CXXFLAGS=-std=c++11 -Iinclude -fPIC -w
LDFLAGS=-shared

SOURCES=$(wildcard src/*.cpp)
OBJECTS=$(SOURCES:src/%.cpp=obj/%.o)
ALL_OBJ=$(OBJECTS)
ALL_OBJ+=obj/pugixml.o

all: $(TARGET)

$(TARGET): $(ALL_OBJ) lib
	$(CXX) $(LDFLAGS) -o $@  $(ALL_OBJ)

obj:
	mkdir -p $@
lib:
	mkdir -p $@

$(OBJECTS): obj/%.o:src/%.cpp obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

obj/pugixml.o: pugixml/src/pugixml.cpp obj
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -f $(ALL_OBJ) $(TARGET) *~


endif
endif