# The code is distributed under the terms of the GNU GPL v3
# Part of the OSRPG project: http://osrpgcreation.sf.net
# Do you want to create the best RPG? Join us!
# Copyright (C) 2006 Michael Kapelko (aka kornerr)

# Compiler flags

CPP = g++
#FLAGS = -I"/usr/local/include/irrlicht" -I"/usr/local/include/CEGUI" -I"/usr/X11R6/include" -g -Wall $(CXXFLAGS)
FLAGS =  -I"/home/Dev/libs/ode/include/" -I"/home/Dev/libs/Irrlicht/include" -I"/usr/local/include/irrlicht" -I"/usr/X11R6/include" -g -Wall $(CXXFLAGS)
#LIBS = -L"/usr/X11R6/lib" -L"/usr/local/lib" -lode -lIrrlicht -lGL -lGLU -lXxf86vm -lXext -lX11 -lCEGUIBase -lCEGUIIrrlichtRenderer
LIBS = -L"/usr/X11R6/lib" -L"/usr/local/lib" -L"/home/Dev/libs/ode/lib" -L"/home/Dev/libs/Irrlicht/lib/Linux"  -lode -lIrrlicht -lGL -lGLU -lXxf86vm -lXext -lX11 

# root directory

test: .objs/src/phys/world.o .objs/src/help.o .objs/src/settings.o .objs/src/phys/sm_entity.o .objs/src/phys/box.o .objs/src/phys/sphere.o .objs/src/wrap/sm_entity.o .objs/src/wrap/box.o .objs/src/wrap/sphere.o .objs/src/wrap/map.o .objs/src/gui/about_menu.o .objs/src/gui/options_menu.o .objs/src/gui/main_menu.o .objs/src/core.o
	$(CPP) -o test .objs/src/phys/world.o .objs/src/help.o .objs/src/settings.o .objs/src/phys/sm_entity.o .objs/src/phys/box.o .objs/src/phys/sphere.o .objs/src/wrap/sm_entity.o .objs/src/wrap/box.o .objs/src/wrap/sphere.o .objs/src/wrap/map.o .objs/src/gui/about_menu.o .objs/src/gui/options_menu.o .objs/src/gui/main_menu.o .objs/src/core.o $(FLAGS) $(LIBS)
	@./data/success

# src/ directory

.objs/src/help.o: src/help.h src/help.cpp
	$(CPP) -o .objs/src/help.o -c src/help.cpp $(FLAGS)

.objs/src/settings.o: src/settings.h src/settings.cpp
	$(CPP) -o .objs/src/settings.o -c src/settings.cpp $(FLAGS)

.objs/src/core.o: src/core.h src/core.cpp src/gui/main_menu.h src/gui/main_menu.cpp src/gui/options_menu.h src/gui/options_menu.cpp
	$(CPP) -o .objs/src/core.o -c src/core.cpp $(FLAGS)

# src/gui/ directory

.objs/src/gui/main_menu.o: src/gui/main_menu.h src/gui/main_menu.cpp
	$(CPP) -o .objs/src/gui/main_menu.o -c src/gui/main_menu.cpp $(FLAGS)

.objs/src/gui/options_menu.o: src/gui/options_menu.h src/gui/options_menu.cpp
	$(CPP) -o .objs/src/gui/options_menu.o -c src/gui/options_menu.cpp $(FLAGS)

.objs/src/gui/about_menu.o: src/gui/about_menu.h src/gui/about_menu.cpp
	$(CPP) -o .objs/src/gui/about_menu.o -c src/gui/about_menu.cpp $(FLAGS)

# src/phys/ directory

.objs/src/phys/world.o: src/phys/world.h src/phys/world.cpp
	$(CPP) -o .objs/src/phys/world.o -c src/phys/world.cpp $(FLAGS)

.objs/src/phys/sm_entity.o: src/phys/sm_entity.h src/phys/sm_entity.cpp
	$(CPP) -o .objs/src/phys/sm_entity.o -c src/phys/sm_entity.cpp $(FLAGS)

.objs/src/phys/box.o: src/phys/box.h src/phys/box.cpp
	$(CPP) -o .objs/src/phys/box.o -c src/phys/box.cpp $(FLAGS)

.objs/src/phys/sphere.o: src/phys/sphere.h src/phys/sphere.cpp
	$(CPP) -o .objs/src/phys/sphere.o -c src/phys/sphere.cpp $(FLAGS)

# src/wrap/ directory

.objs/src/wrap/sm_entity.o: src/wrap/sm_entity.h src/wrap/sm_entity.cpp
	$(CPP) -o .objs/src/wrap/sm_entity.o -c src/wrap/sm_entity.cpp $(FLAGS)

.objs/src/wrap/box.o: src/wrap/box.h src/wrap/box.cpp
	$(CPP) -o .objs/src/wrap/box.o -c src/wrap/box.cpp $(FLAGS)

.objs/src/wrap/sphere.o: src/wrap/sphere.h src/wrap/sphere.cpp
	$(CPP) -o .objs/src/wrap/sphere.o -c src/wrap/sphere.cpp $(FLAGS)

.objs/src/wrap/map.o: src/wrap/map.h src/wrap/map.cpp
	$(CPP) -o .objs/src/wrap/map.o -c src/wrap/map.cpp $(FLAGS)

# miscelleneous

clean:
	rm -f *~ test .objs/src/*.o .objs/src/gui/*.o .objs/src/phys/*.o .objs/src/wrap/*.o
	rm -fR data/api/*

api:
	doxygen Doxyfile
	@echo "----------------------------------------"
	@echo "You will find API in data/api/ directory."
	@echo "----------------------------------------"

