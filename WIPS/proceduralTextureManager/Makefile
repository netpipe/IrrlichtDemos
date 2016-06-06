#
# Irrlicht Procedural Texture Manager Showcase by CZestmyr
#
# INCLUDEDIR should be set to the include directory of Irrlicht
# LIBDIR should be set to the lib directory of Irrlicht
#

INCLUDEDIR = -I/home/Dev/libs/Irrlicht/include
LIBDIR = -L/home/Dev/libs/Irrlicht/lib/Linux
LIBS = -lIrrlicht -lstdc++ -lGL -lGLU -lXxf86vm

showcase: CCrossPlaneSceneNode.o CProceduralTextureManager.o showcase.cpp
	gcc $(CFLAGS) $(INCLUDEDIR) CProceduralTextureManager.o CCrossPlaneSceneNode.o showcase.cpp $(LIBDIR) $(LIBS) -o showcase

CCrossPlaneSceneNode.o: CCrossPlaneSceneNode.cpp CCrossPlaneSceneNode.h
	gcc $(CFLAGS) $(INCLUDEDIR) -c CCrossPlaneSceneNode.cpp

CProceduralTextureManager.o: CProceduralTextureManager.cpp CProceduralTextureManager.h
	gcc $(CFLAGS) $(INCLUDEDIR) -c CProceduralTextureManager.cpp

clean:
	rm ./*.o
