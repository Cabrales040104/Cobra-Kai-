CXX =	g++
TARGET =	main.exe

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

run:	all
	.\main.exe

clean:
	-del /Q build\* bin\* main.exe

.PHONY:	all run clean



