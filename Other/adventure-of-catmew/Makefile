#c++ compiler
CC = gcc

#binary name
BINNAME = Game

#binary directory
BINARY = bin

#linked libraries
LIBS = -lstdc++ -lLeeuwenGfx -lGL -lSDL -lSDL_image -lSDL_ttf -lGLU -lpthread -lm

#additional optimizations
OPTIMIZATIONS =  -Wall -O3 -Os -fexpensive-optimizations

#definitions
DEFINES = -DLINUX

#objects to compile
OBJECTS	:= $(patsubst %.cpp,%.o,$(wildcard *.cpp)) 

#sources to compile
SOURCES := $(patsubst %.h,%.o,$(wildcard *.cpp)) 

#paths to libraries/header files
PATHS = -L /usr/lib -I /usr/include -I gfxengine/include -L gfxengine/lib/linux

#compile the project
all:
	make project

project: $(BINNAME)

#link the object-files together
$(BINNAME):$(OBJECTS)
			$(CC) -o $(BINARY)/$(BINNAME) $(OBJECTS) $(PATHS) $(LIBS) $(OPTIMIZATIONS) $(DEFINES)

#run the compiled executable, but first set the rights to +x
run:
	chmod +x $(BINARY)/$(BINNAME)
	cd $(BINARY) && DISPLAY=:0 ./$(BINNAME)

#compile the single source-files
%.o: %.cpp
	$(CC) -c -o $@ $< $(PATHS) $(OPTIMIZATIONS) $(DEFINES)


#clean the project (remove objects, binary)
clean:
	rm -r $(OBJECTS) $(BINARY)/$(BINNAME) -fiRr -f
