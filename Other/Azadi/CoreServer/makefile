CC=g++
GCC=gcc
C_FLAGS=-ffast-math -Wall -g
COMPILE = $(CC) $(C_FLAGS) -c
CCOMPILE = $(GCC) $(C_FLAGS) -c

L_FLAGS=-lmysqlpp
L_INCLUDES=-I/usr/include/mysql++

OBJS := $(patsubst %.cpp, %.o, $(wildcard src/*.cpp))
OBJS2 := $(patsubst %.cpp, %.o, $(wildcard src/*/*.cpp))
OBJS3 := $(patsubst %.cpp, %.o, $(wildcard src/*/*/*.cpp))
OBJS4 := $(patsubst %.cpp, %.o, $(wildcard src/*/*/*/*.cpp))
OBJS5 := $(patsubst %.cpp, %.o, $(wildcard src/*/*/*/*/*.cpp))

OBJECTS = $(OBJS) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5)

all: $(OBJECTS)
	$(CC) -g -o LunaServer $(OBJECTS) $(L_FLAGS)

%.obj: %.c
	$(CCOMPILE) $(L_INCLUDES) $< -o $@

%.o: %.cpp
	$(COMPILE) $(L_INCLUDES) $< -o $@

clean:
	rm $(OBJECTS)
	rm LunaServer
