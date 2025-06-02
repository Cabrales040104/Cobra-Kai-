CXX ?= g++
PREFIX ?= /mingw64
TARGET ?= app

SRC := $(wildcard src/*.cpp)
OBJ := $(patsubst src/%.cpp,build/%.o,$(SRC))

CXXFLAGS ?= -std=c++17 -Wall -I$(PREFIX)/include -Iinclude
LDFLAGS  ?= -L$(PREFIX)/lib -lsfml-graphics -lsfml-window -lsfml-system -lmingw32

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $^ -o $@ $(LDFLAGS)

build/%.o: src/%.cpp | build
	$(CXX) $(CXXFLAGS) -c $< -o $@

build:
	mkdir -p build

run: $(TARGET)
	./$(TARGET)

clean:
	-$(RM) -r build $(TARGET)

.PHONY: all run clean

run: 
    .\bin\COBRAKAI.exe

compile:
	g++ menu.cpp -o bin/COBRAKAI.exe -Iinclude -Llib -lsfml-graphics -lsfml-window -lsfml-system\
	.\bin\COBRAKAI.exe



