CC=g++
GCC=gcc
C_FLAGS= -ffast-math -Wall -g
COMPILE = $(CC) $(C_FLAGS) -c
CCOMPILE = $(GCC) $(C_FLAGS) -c

L_FLAGS=-lGL -lGLU -lGLEW -lSDL -lpng -lstdc++ -lopenal -logg -lvorbis -lvorbisfile -lfreetype -ljpeg -lz -lcal3d -lbulletdynamics -lbulletcollision -lbulletmath
L_INCLUDES=-I/usr/include/freetype2 -I/usr/include/bullet -I/usr/local/include/bullet

W_FLAGS=-lmingw32 -lopengl32 -lglu32 -lglew32 -lSDLmain -lSDL -lpng -lstdc++ -lopenal32 -lvorbisfile -lvorbis  -logg -lwsock32 -lzlib -lfreetype -lcal3d -lwinmm -lpthreadGC2 -lbulletdynamics -lbulletcollision -lbulletmath

M_FLAGS=-framework Cocoa -framework OpenGL -framework OpenAL -lGLEW -lSDLmain -lSDL -lpng -lvorbisfile -lvorbis -logg -lz -lfreetype -lstdc++ -lcal3d -lbulletdynamics -lbulletcollision -lbulletmath

OBJS := $(patsubst %.cpp, %.o, $(wildcard src/*.cpp))
OBJS2 := $(patsubst %.cpp, %.o, $(wildcard src/*/*.cpp))
OBJS3 := $(patsubst %.cpp, %.o, $(wildcard src/*/*/*.cpp))
OBJS4 := $(patsubst %.cpp, %.o, $(wildcard src/*/*/*/*.cpp))
OBJS5 := $(patsubst %.cpp, %.o, $(wildcard src/*/*/*/*/*.cpp))

MOBJSC := $(patsubst %.m, %.o, $(wildcard src/*.m))
MOBJSCPP := $(patsubst %.mm, %.o, $(wildcard src/*.mm))
MOBJS := $(patsubst %.cpp, %.o, $(wildcard src/*.cpp))
MOBJS2 := $(patsubst %.cpp, %.o, $(wildcard src/*/*.cpp))
MOBJS3 := $(patsubst %.cpp, %.o, $(wildcard src/*/*/*.cpp))
MOBJS4 := $(patsubst %.cpp, %.o, $(wildcard src/*/*/*/*.cpp))
MOBJS5 := $(patsubst %.cpp, %.o, $(wildcard src/*/*/*/*/*.cpp))

OBJECTS = $(OBJS) $(OBJS2) $(OBJS3) $(OBJS4) $(OBJS5)

MOBJECTS = $(MOBJSC) $(MOBJSCPP) $(MOBJS) $(MOBJS2) $(MOBJS3) $(MOBJS4) $(MOBJS5)

all: error

precompile: $(HEADS)
	@echo
	@echo "        Precompiled the headers"
	@echo "        Note that this is done only once unless you dist-clean"
	@echo

error:
	@echo
	@echo
	@echo "        Please run make win, make lin or make mac to build depending on your platform"
	@echo "        Other options:"
	@echo "        make clean	 	: removes Azadi executable as well as object files"
	@echo "        make dist-clean		: removes all compiled objects, executables, and headers"
	@echo
	@echo

win: Azadi-Win

lin: Azadi-Lin

mac: Azadi-Mac

Azadi-Win: $(OBJECTS)
	$(CC) -g -o Century $(OBJECTS) $(W_FLAGS)
	doxygen azadi-docs

Azadi-Lin: $(OBJECTS)
	$(CC) -g -o Century $(OBJECTS) $(L_FLAGS)
	doxygen azadi-docs

Azadi-Mac: $(MOBJECTS)
	$(CC) -g -o Azadi $(OBJECTS) $(M_FLAGS)
	doxygen azadi-docs

%.obj: %.c
	$(CCOMPILE) $(L_INCLUDES) $< -o $@

%.o: %.cpp
	$(COMPILE) $(L_INCLUDES) $< -o $@

%.o: %.m
	$(COMPILE) $(L_INCLUDES) $< -o $@

%.o: %.mm
	$(COMPILE) $(L_INCLUDES) $< -o $@
	
%.h.gch: %.h
	$(COMPILE) $< -o $@

dist-clean:
	rm $(OBJECTS)
	rm Century
	rm -rf SourceDocs

clean:
	rm $(OBJECTS)
	rm Century
