CPP = g++
SRC = main.cpp
OBJ = $(SRC:.cpp=.o)

CPPFLAGS = -I. -I"/home/Dev/libs/Irrlicht/include" -I"/usr/X11R6/include"

OPTS_10 =   -L"/usr/X11R6/lib" -L"/home/Dev/libs/Irrlicht/lib/gcc" -lIrrlicht -lGL -lGLU -lXxf86vm -lXext -lX11 -lz -ljpeg

all:  Main

Main: $(OBJ)
	$(CPP) $(CXXINCS) $(OBJ) $(OPTS_10) -o Main

clean:
	rm -f *~ *.o Main
