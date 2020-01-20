#c++ compiler
CC = gcc

#binary name
BINNAME = libLeeuwenGfx.a

#source directory
SOURCE = .

#binary directory
BINARY = ../lib/linux

#header-include directory
INCLUDE	= -I ../include

#linked libraries
LIBS = -lstdc++

#additional optimizations
OPTIMIZATIONS = -O3 -Os -fexpensive-optimizations -Wall

#definitions
DEFINES = -DLINUX -DGLEW_STATIC

#objects to compile
OBJECTS	:= $(patsubst %.cpp,%.o,$(wildcard $(SOURCE)/*.cpp)) 

#sources to compile
SOURCES := $(patsubst %.h,%.o,$(wildcard $(SOURCE)/*.cpp)) 

#paths to libraries/header files
PATHS = -L /usr/lib -I /usr/include $(INCLUDE)

#compile the project
all:
	make project

project: $(OBJECTS)
	$(AR) rs $(BINNAME) $^
	cp $(BINNAME) $(BINARY)
	
objects: $(OBJECTS)
	
#compile the single source-files
%.o: %.cpp
	$(CC) -c -o $@ $< $(PATHS) $(OPTIMIZATIONS) $(DEFINES)


#clean the project (remove objects, binary)
clean:
	rm -r $(OBJECTS) $(BINNAME) -fiRr -f
