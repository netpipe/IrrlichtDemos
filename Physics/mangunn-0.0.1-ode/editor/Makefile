# The code is distributed under the terms of the GNU GPL v3
# Part of the OSRPG project: http://osrpgcreation.sf.net
# Do you want to create the best RPG? Join us!
# Copyright (C) 2006 Michael Kapelko (aka kornerr)

# Compiler flags

CPP = g++
FLAGS = `../../trunk/tools/osrpg-config --cxxflags` $(CXXFLAGS)
LIBS = `../../trunk/tools/osrpg-config --libs`

# root directory

editor: .objs/src/misc/help.o .objs/src/settings.o .objs/src/registry.o .objs/src/map/item.o .objs/src/map/mesh.o .objs/src/map/light.o .objs/src/map/zone.o .objs/src/map/place_tag.o .objs/src/map/sky.o .objs/src/map/joint_ball_socket.o .objs/src/map/joint_hinge.o .objs/src/map/scene.o .objs/src/gui/add_mesh.o .objs/src/gui/edit_mesh.o .objs/src/gui/edit_light.o .objs/src/gui/edit_zone.o .objs/src/gui/edit_place_tag.o .objs/src/gui/edit_joint_bs.o .objs/src/gui/edit_joint_hinge.o .objs/src/gui/save_as.o .objs/src/gui/open.o .objs/src/gui/properties.o .objs/src/gui/main_menu.o .objs/src/core.o
	$(CPP) -o editor .objs/src/misc/help.o .objs/src/settings.o .objs/src/registry.o .objs/src/map/item.o .objs/src/map/mesh.o .objs/src/map/light.o .objs/src/map/zone.o .objs/src/map/place_tag.o .objs/src/map/sky.o .objs/src/map/joint_ball_socket.o .objs/src/map/joint_hinge.o .objs/src/map/scene.o .objs/src/gui/add_mesh.o .objs/src/gui/edit_mesh.o .objs/src/gui/edit_light.o .objs/src/gui/edit_zone.o .objs/src/gui/edit_place_tag.o .objs/src/gui/edit_joint_bs.o .objs/src/gui/edit_joint_hinge.o .objs/src/gui/save_as.o .objs/src/gui/open.o .objs/src/gui/properties.o .objs/src/gui/main_menu.o .objs/src/core.o $(FLAGS) $(LIBS)
	@../../trunk/tools/success

# src/

.objs/src/core.o: src/core.* src/gui/main_menu.h
	$(CPP) -o .objs/src/core.o -c src/core.cpp $(FLAGS)

.objs/src/registry.o: src/registry.* src/settings.h
	$(CPP) -o .objs/src/registry.o -c src/registry.cpp $(FLAGS)

.objs/src/settings.o: src/settings.* 
	$(CPP) -o .objs/src/settings.o -c src/settings.cpp $(FLAGS)

# src/gui/

.objs/src/gui/main_menu.o: src/gui/* src/misc/events.h src/misc/help.h
	$(CPP) -o .objs/src/gui/main_menu.o -c src/gui/main_menu.cpp $(FLAGS)

.objs/src/gui/add_mesh.o: src/gui/add_mesh.* src/map/scene.h src/misc/events.h
	$(CPP) -o .objs/src/gui/add_mesh.o -c src/gui/add_mesh.cpp $(FLAGS)

.objs/src/gui/edit_joint_bs.o: src/gui/edit_joint_bs.* src/map/scene.h src/misc/events.h
	$(CPP) -o .objs/src/gui/edit_joint_bs.o -c src/gui/edit_joint_bs.cpp $(FLAGS)

.objs/src/gui/edit_joint_hinge.o: src/gui/edit_joint_hinge.* src/map/scene.h src/misc/events.h
	$(CPP) -o .objs/src/gui/edit_joint_hinge.o -c src/gui/edit_joint_hinge.cpp $(FLAGS)

.objs/src/gui/edit_light.o: src/gui/edit_light.* src/map/scene.h src/misc/events.h
	$(CPP) -o .objs/src/gui/edit_light.o -c src/gui/edit_light.cpp $(FLAGS)

.objs/src/gui/edit_mesh.o: src/gui/edit_mesh.* src/map/scene.h src/misc/events.h
	$(CPP) -o .objs/src/gui/edit_mesh.o -c src/gui/edit_mesh.cpp $(FLAGS)

.objs/src/gui/edit_place_tag.o: src/gui/edit_place_tag.* src/map/scene.h src/misc/events.h
	$(CPP) -o .objs/src/gui/edit_place_tag.o -c src/gui/edit_place_tag.cpp $(FLAGS)

.objs/src/gui/edit_zone.o: src/gui/edit_zone.* src/map/scene.h src/misc/events.h
	$(CPP) -o .objs/src/gui/edit_zone.o -c src/gui/edit_zone.cpp $(FLAGS)

.objs/src/gui/open.o: src/gui/open.* src/map/scene.h src/misc/events.h
	$(CPP) -o .objs/src/gui/open.o -c src/gui/open.cpp $(FLAGS)

.objs/src/gui/properties.o: src/gui/properties.* src/map/scene.h src/misc/events.h
	$(CPP) -o .objs/src/gui/properties.o -c src/gui/properties.cpp $(FLAGS)

.objs/src/gui/save_as.o: src/gui/save_as.* src/map/scene.h src/misc/events.h
	$(CPP) -o .objs/src/gui/save_as.o -c src/gui/save_as.cpp $(FLAGS)

# src/map/

.objs/src/map/scene.o: src/map/* src/misc/help.h
	$(CPP) -o .objs/src/map/scene.o -c src/map/scene.cpp $(FLAGS)

.objs/src/map/item.o: src/map/item.*  src/registry.h
	$(CPP) -o .objs/src/map/item.o -c src/map/item.cpp $(FLAGS)

.objs/src/map/light.o: src/map/light.* src/map/item.h src/map/format.h
	$(CPP) -o .objs/src/map/light.o -c src/map/light.cpp $(FLAGS)

.objs/src/map/mesh.o: src/map/mesh.* src/map/item.h src/map/format.h
	$(CPP) -o .objs/src/map/mesh.o -c src/map/mesh.cpp $(FLAGS)

.objs/src/map/place_tag.o: src/map/place_tag.* src/map/item.h src/map/format.h
	$(CPP) -o .objs/src/map/place_tag.o -c src/map/place_tag.cpp $(FLAGS)

.objs/src/map/zone.o: src/map/zone.* src/map/item.h src/map/format.h src/misc/help.h
	$(CPP) -o .objs/src/map/zone.o -c src/map/zone.cpp $(FLAGS)

.objs/src/map/sky.o: src/map/sky.* src/map/item.h src/map/format.h src/misc/help.h
	$(CPP) -o .objs/src/map/sky.o -c src/map/sky.cpp $(FLAGS)

.objs/src/map/joint_ball_socket.o: src/map/joint_ball_socket.* src/map/mesh.h
	$(CPP) -o .objs/src/map/joint_ball_socket.o -c src/map/joint_ball_socket.cpp $(FLAGS)

.objs/src/map/joint_hinge.o: src/map/joint_hinge.* src/map/mesh.h
	$(CPP) -o .objs/src/map/joint_hinge.o -c src/map/joint_hinge.cpp $(FLAGS)

# src/misc/

.objs/src/misc/help.o: src/misc/help.*
	$(CPP) -o .objs/src/misc/help.o -c src/misc/help.cpp $(FLAGS)

# miscelleneous

clean:
	rm -f editor .objs/src/*.o .objs/src/map/*.o .objs/src/misc/*.o .objs/src/gui/*.o
