# Makefile for Irrlicht Interactive Demo theIsland created by Strong99
# It's usually sufficient to change just the target name and source file list
# and be sure that CXX is set to a valid compiler
Target = theIsland
Sources = main.cpp CCore.h CCore.cpp CController.h CController.cpp Randomizer.h Randomizer.cpp shader.h ThirdPersonCamera.h ThirdPersonCamera.cpp

# general compiler settings
CPPFLAGS = -I../../include -I/usr/X11R6/include
CXXFLAGS = -O3 -ffast-math
#CXXFLAGS = -g -Wall

#default target is Linux
all: all_linux

ifeq ($(HOSTTYPE), x86_64)
LIBSELECT=64
endif

# target specific settings
all_linux: LDFLAGS = -L/usr/X11R6/lib$(LIBSELECT) -Llib/Linux -lIrrlicht -lGL -lXxf86vm -lXext -lX11
all_linux clean_linux: SYSTEM=Linux
all_win32: LDFLAGS = -Llib/Win32-gcc -lIrrlicht -lopengl32 -lm
all_win32 clean_win32: SYSTEM=Win32-gcc
all_win32 clean_win32: SUF=.exe
# name of the binary - only valid for targets which set SYSTEM
DESTPATH = $(Target)$(SUF)

all_linux all_win32:
	$(warning Building...)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) $(Sources) -o $(DESTPATH) $(LDFLAGS)

clean: clean_linux clean_win32
	$(warning Cleaning...)

clean_linux clean_win32:
	@$(RM) $(DESTPATH)

.PHONY: all all_win32 clean clean_linux clean_win32
