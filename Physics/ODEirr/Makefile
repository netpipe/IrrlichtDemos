# The code is distributed under the terms of the GNU General Public License
# Part of the OSRPG project (http://osrpgcreation.sf.net)
# Do you want to create the best RPG? Join us!
# Copyright (C) 2006 Michael Kapelko (aka kornerr)

# Compiler flags

CPP = g++
FLAGS = -I"/usr/local/include/irrlicht" -I"/home/Dev/libs/ode/include/" -I"/home/Dev/libs/Irrlicht/include" -I"/usr/X11R6/include" $(CXXFLAGS)
LIBS = -L"/usr/X11R6/lib" -L"/usr/local/lib" -L"/home/Dev/libs/ode/lib" -L"/home/Dev/libs/Irrlicht/lib/Linux" -lode -lIrrlicht -lGL -lGLU -lXxf86vm -lXext -lX11 

# current directory

test: .objs/globals.o .objs/settings.o .objs/help.o .objs/physics/basic.o .objs/physics/box.o .objs/physics/player.o .objs/physics/rag_doll.o .objs/node/mesh_node.o .objs/base/rag_doll.o .objs/base/map.o .objs/base/box.o .objs/base/player.o .objs/base/cam_attachment.o .objs/misc/player_direction.o .objs/menu.o .objs/camera.o .objs/core.o 
	$(CPP) -o test .objs/globals.o .objs/settings.o .objs/help.o .objs/physics/basic.o .objs/physics/box.o .objs/physics/player.o .objs/physics/rag_doll.o .objs/node/mesh_node.o .objs/base/rag_doll.o .objs/base/map.o .objs/base/box.o .objs/base/player.o .objs/base/cam_attachment.o .objs/misc/player_direction.o .objs/menu.o .objs/camera.o .objs/core.o $(FLAGS) $(LIBS)
	@sh ./_data/success

.objs/globals.o: globals.h globals.cpp
	$(CPP) -o .objs/globals.o -c globals.cpp $(FLAGS)

.objs/settings.o: settings.h settings.cpp
	$(CPP) -o .objs/settings.o -c settings.cpp $(FLAGS)

.objs/help.o: help.h help.cpp
	$(CPP) -o .objs/help.o -c help.cpp $(FLAGS)

.objs/menu.o: menu.h menu.cpp
	$(CPP) -o .objs/menu.o -c menu.cpp $(FLAGS)

.objs/camera.o: camera.h camera.cpp
	$(CPP) -o .objs/camera.o -c camera.cpp $(FLAGS)

.objs/core.o: core.h core.cpp
	$(CPP) -o .objs/core.o -c core.cpp $(FLAGS)

# physics directory

.objs/physics/basic.o: physics/basic.h physics/basic.cpp
	$(CPP) -o .objs/physics/basic.o -c physics/basic.cpp $(FLAGS)

.objs/physics/rag_doll.o: physics/rag_doll.h physics/rag_doll.cpp
	$(CPP) -o .objs/physics/rag_doll.o -c physics/rag_doll.cpp $(FLAGS)

.objs/physics/box.o: physics/box.h physics/box.cpp
	$(CPP) -o .objs/physics/box.o -c physics/box.cpp $(FLAGS)

.objs/physics/player.o: physics/player.h physics/player.cpp
	$(CPP) -o .objs/physics/player.o -c physics/player.cpp $(FLAGS)

# node directory

.objs/node/mesh_node.o: node/mesh_node.h node/mesh_node.cpp
	$(CPP) -o .objs/node/mesh_node.o -c node/mesh_node.cpp $(FLAGS)

# base directory

.objs/base/rag_doll.o: base/rag_doll.h base/rag_doll.cpp
	$(CPP) -o .objs/base/rag_doll.o -c base/rag_doll.cpp $(FLAGS)

.objs/base/map.o: base/map.h base/map.cpp
	$(CPP) -o .objs/base/map.o -c base/map.cpp $(FLAGS)

.objs/base/box.o: base/box.h base/box.cpp
	$(CPP) -o .objs/base/box.o -c base/box.cpp $(FLAGS)

.objs/base/player.o: base/player.h base/player.cpp
	$(CPP) -o .objs/base/player.o -c base/player.cpp $(FLAGS)

.objs/base/cam_attachment.o: base/cam_attachment.h base/cam_attachment.cpp
	$(CPP) -o .objs/base/cam_attachment.o -c base/cam_attachment.cpp $(FLAGS)

# misc directory

.objs/misc/player_direction.o: misc/player_direction.h misc/player_direction.cpp
	$(CPP) -o .objs/misc/player_direction.o -c misc/player_direction.cpp $(FLAGS)

clean:
	rm -f *~ test .objs/*.o .objs/node/*.o .objs/physics/*.o .objs/base/*.o .objs/misc/*.o

doc:
	@doxygen
	@echo "----------------------------------------"
	@echo "You will find API docs in _data/docs"
	@echo "----------------------------------------"

