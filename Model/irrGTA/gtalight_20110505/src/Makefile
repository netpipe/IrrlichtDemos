CC=g++
CFLAGS=-Wall -I../include
LINKFLAGS=-L../lib -lIrrlicht
OBJ =   CGTASceneNode.o \
        CDFFMeshFileLoader.o \
        CIMGArchiveLoader.o \
        CTXDArchiveLoader.o \
        CGTAFileList.o \
        CImageLoaderGTATexture.o \
        CEventReceiver.o \
        CRenderManager.o \
        s3tc.o \
        main.o

%.o: %.cpp
	$(CC) -g -c -o $@ $< $(CFLAGS)

main: $(OBJ)
	$(CC) -g -o $@ $^ $(CFLAGS) $(LINKFLAGS)

default: main
