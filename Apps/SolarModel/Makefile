# Makefile for SolarModel

Target = SolarModel
Sources = GameAnimatedMeshObject.cpp GameBillboardObject.cpp GameConfig.cpp GameCore.cpp GameEventReceiver.cpp GameGlobals.cpp GameLightObject.cpp GameMain.cpp GameMap.cpp GameMapDescription.cpp GameObject.cpp GameSkyBoxObject.cpp GamePointcloudObject.cpp GameMeshObject.cpp GameMeasurer.cpp

# Paths
IRRLICHT_HOME = /home/Dev/libs/Irrlicht

# general compiler settings
CPPFLAGS = -I$(IRRLICHT_HOME)/include -I/usr/X11R6/include
CXXFLAGS = -O3 -ffast-math
#CXXFLAGS = -g -Wall

#default target is Linux
all: all_linux

ifeq ($(HOSTTYPE), x86_64)
LIBSELECT=64
endif

# target specific settings
all_linux: LDFLAGS = -L/usr/X11R6/lib$(LIBSELECT) -L$(IRRLICHT_HOME)/lib/Linux -lIrrlicht -lGL -lGLU -lXxf86vm -lXext -lX11
all_linux clean_linux: SYSTEM=Linux
all_win32: LDFLAGS = -L$(IRRLICHT_HOME)/lib/Win32-gcc -lIrrlicht -lopengl32 -lglu32 -lm
all_win32 clean_win32: SYSTEM=Win32-gcc
all_win32 clean_win32: SUF=.exe
# name of the binary
DESTPATH = $(Target)$(SUF)

OBJECTS = $(Sources:%.cpp=%.o)

all_linux all_win32: $(OBJECTS)
	$(warning Building...)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $(DESTPATH) $(LDFLAGS)

clean: clean_linux clean_win32
	$(warning Cleaning...)

clean_linux clean_win32:
	@$(RM) $(DESTPATH) $(OBJECTS)

.PHONY: all all_win32 clean clean_linux clean_win32

