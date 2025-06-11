CXX =	g++
TARGET =	bin/COBRAKAI.exe

SRC :=	$(wildcard src/*.cpp)
OBJ :=	$(patsubst src/%.cpp,build/%.o,$(SRC))

CXXFLAGS =	-std=c++17 -Wall -Iinclude
LDFLAGS  =	-Llib -lsfml-graphics -lsfml-window -lsfml-system

all:	$(TARGET)

$(TARGET):	$(OBJ) | bin
	$(CXX) $^ -o $@ $(LDFLAGS)

build/%.o:	src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir build

bin:
	mkdir bin

run:	$(TARGET)
	.\$(TARGET)

clean:
	-del /Q build\* bin\*

.PHONY: all run clean




