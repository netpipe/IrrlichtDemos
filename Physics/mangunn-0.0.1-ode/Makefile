# The code is distributed under the terms of the GNU GPL v3
# Part of the OSRPG project: http://osrpgcreation.sf.net
# Do you want to create the best RPG? Join us!
# Copyright (C) 2006 Michael Kapelko (aka kornerr)

# Compiler flags

CPP = g++
FLAGS = `tools/osrpg-config --cxxflags` $(CXXFLAGS)
LIBS = `tools/osrpg-config --libs`

# root directory

game: .objs/src/misc/help.o .objs/src/settings.o .objs/src/registry.o .objs/src/map/entity.o .objs/src/map/player.o .objs/src/map/sky.o .objs/src/map/mesh.o .objs/src/map/light.o .objs/src/map/place_tag.o .objs/src/map/joint.o .objs/src/map/joint_bs.o .objs/src/map/joint_hinge.o .objs/src/map/scene.o .objs/src/gui/options_menu.o .objs/src/gui/main_menu.o .objs/src/core.o
	$(CPP) -o game .objs/src/misc/help.o .objs/src/settings.o .objs/src/registry.o .objs/src/map/entity.o .objs/src/map/player.o .objs/src/map/sky.o .objs/src/map/mesh.o .objs/src/map/light.o .objs/src/map/place_tag.o .objs/src/map/joint.o .objs/src/map/joint_bs.o .objs/src/map/joint_hinge.o .objs/src/map/scene.o .objs/src/gui/options_menu.o .objs/src/gui/main_menu.o .objs/src/core.o $(CXXFLAGS) $(LIBS)
	@./tools/success

# src/ directory

.objs/src/settings.o: src/settings.* src/misc/help.h
	$(CPP) -o .objs/src/settings.o -c src/settings.cpp $(FLAGS)

.objs/src/registry.o: src/registry.* src/settings.h
	$(CPP) -o .objs/src/registry.o -c src/registry.cpp $(FLAGS)

.objs/src/core.o: src/core.* src/misc/events.h src/gui/main_menu.h src/map/scene.h
	$(CPP) -o .objs/src/core.o -c src/core.cpp $(FLAGS)

# src/gui/ directory

.objs/src/gui/options_menu.o: src/gui/options_menu.* src/misc/events.h src/misc/help.h src/registry.h
	$(CPP) -o .objs/src/gui/options_menu.o -c src/gui/options_menu.cpp $(FLAGS)

.objs/src/gui/main_menu.o: src/gui/main_menu.* src/misc/events.h src/misc/help.h
	$(CPP) -o .objs/src/gui/main_menu.o -c src/gui/main_menu.cpp $(FLAGS)

# src/map/ directory

.objs/src/map/entity.o: src/map/entity.* src/misc/help.h src/registry.h
	$(CPP) -o .objs/src/map/entity.o -c src/map/entity.cpp $(FLAGS)

.objs/src/map/player.o: src/map/player.* src/misc/help.h src/map/entity.h
	$(CPP) -o .objs/src/map/player.o -c src/map/player.cpp $(FLAGS)

.objs/src/map/sky.o: src/map/sky.* src/map/format.h src/registry.h
	$(CPP) -o .objs/src/map/sky.o -c src/map/sky.cpp $(FLAGS)

.objs/src/map/mesh.o: src/map/mesh.* src/misc/help.h src/map/entity.h src/map/format.h
	$(CPP) -o .objs/src/map/mesh.o -c src/map/mesh.cpp $(FLAGS)

.objs/src/map/light.o: src/map/light.* src/registry.h src/map/format.h
	$(CPP) -o .objs/src/map/light.o -c src/map/light.cpp $(FLAGS)

.objs/src/map/place_tag.o: src/map/place_tag.* src/map/format.h
	$(CPP) -o .objs/src/map/place_tag.o -c src/map/place_tag.cpp $(FLAGS)

.objs/src/map/joint.o: src/map/joint.* src/map/mesh.h
	$(CPP) -o .objs/src/map/joint.o -c src/map/joint.cpp $(FLAGS)

.objs/src/map/joint_bs.o: src/map/joint_bs.* src/map/joint.h
	$(CPP) -o .objs/src/map/joint_bs.o -c src/map/joint_bs.cpp $(FLAGS)

.objs/src/map/joint_hinge.o: src/map/joint_hinge.* src/misc/help.h src/map/joint.h
	$(CPP) -o .objs/src/map/joint_hinge.o -c src/map/joint_hinge.cpp $(FLAGS)

.objs/src/map/scene.o: src/map/scene.* src/map/mesh.h src/map/light.h src/map/place_tag.h src/map/joint_bs.h src/map/joint_hinge.h src/map/player.h src/misc/help.h
	$(CPP) -o .objs/src/map/scene.o -c src/map/scene.cpp $(FLAGS)

# src/misc/ directory

.objs/src/misc/help.o: src/misc/help.*
	$(CPP) -o .objs/src/misc/help.o -c src/misc/help.cpp $(FLAGS)

# miscelleneous

clean:
	rm -f *~ game .objs/src/*.o .objs/src/gui/*.o .objs/src/misc/*.o .objs/src/mm/*.o .objs/src/sm/*.o .objs/src/wrap/*.o .objs/src/map/*.o
	rm -fR doc/api/*

