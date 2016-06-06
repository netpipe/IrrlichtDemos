Target = Demo

#all source files
Sources = main.cpp core/Application.cpp core/Configuration.cpp core/GameObject.cpp core/Game.cpp core/IHoverCraft.cpp \
physics/Physics.cpp physics/OdeObject.cpp \
scene/IrrlichtObject.cpp scene/WeatherSystem.cpp scene/Ship.cpp scene/Laser.cpp scene/Terrain.cpp scene/grass/CGrassGeneratorNode.cpp \
scene/grass/CGrassPatchSceneNode.cpp

#common compiler flags
CPPFLAGS = -I../lib/irrlicht/include -I../lib/ode/include
CXXFLAGS = -Wall -O3 -finline-functions

#CXXFLAGS = -Wall -O0 -g

ifndef NOSOUND
CPPFLAGS += -I../lib/irrklang/include
CXXFLAGS += -D_SOUND
endif

#common linker flags
LDFLAGS = -L../lib/irrlicht/bin/$(SYSTEM) -lIrrlicht -L../lib/ode/bin/$(SYSTEM) -lode

#default target is linux
all: all_linux

# target specific settings
all_linux: SYSTEM=linux
all_linux: LDFLAGS += -lGL -lXxf86vm -lXext -lX11

all_win32 clean_win32: SYSTEM=win_gcc
all_win32 clean_win32: SUF=.exe
all_win32: LDFLAGS += -lopengl32 -lm

ifndef NOSOUND
all_linux: LDFLAGS += ../lib/irrklang/bin/$(SYSTEM)/libIrrKlang.so -lpthread
all_win32: LDFLAGS += -L../lib/irrklang/bin/$(SYSTEM) -lIrrklang
endif

# name and location of the binary
DESTPATH = ../demo/$(Target)$(SUF)

OBJ = $(Sources:.cpp=.o)

all_linux all_win32: $(OBJ)
	$(warning Building...)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $^ -o $(DESTPATH) $(LDFLAGS)

clean:
	$(warning Cleaning...)
	$(RM) $(OBJ)

.PHONY: all all_win32 clean clean_linux clean_win32
