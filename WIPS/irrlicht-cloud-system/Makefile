TARGET = test
OBJECTS = test.o clouds.o common.o

INCLUDE_PATH = -I/usr/include/irrlicht -I/usr/X11R6/include
LIBRARY_PATH = -L/usr/lib
LIBRARIES = -lIrrlicht 
# to add to libraries if necessary
#-lGL -lXxf86vm -lXext -lX11 -lXcursor
CC_FLAGS = -ggdb -O3 -ffast-math -std=c++11 -fpermissive
LD_FLAGS = $(CC_FLAGS)

#all: $(TARGET)
$(TARGET): $(OBJECTS)
	g++ $(OBJECTS) $(LD_FLAGS) $(LIBRARY_PATH) $(LIBRARIES) -o $(TARGET)

.cpp.o:
	g++ -c $(CC_FLAGS) $(INCLUDE_PATH) $<

clean:
	rm $(OBJECTS)
