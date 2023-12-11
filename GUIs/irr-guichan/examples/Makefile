CFLAGS = -Werror -Wall -Wno-unused

all: irrlicht

irrlicht:
	g++ irrlichthelloworld.cpp ../src/irrlicht/*.cpp -o irrlichthelloworld $(CFLAGS) \
        -lguichan -lIrrlicht -lGL -lXxf86vm -lGLU \
        -I../include -I/usr/src/irrlicht/include -L/usr/src/irrlicht/lib/Linux
	g++ irrlichtwidgets.cpp ../src/irrlicht/*.cpp -o irrlichtwidgets $(CFLAGS) \
        -lguichan -lIrrlicht -lGL -lXxf86vm -lGLU \
        -I../include -I/usr/src/irrlicht/include -L/usr/src/irrlicht/lib/Linux
	g++ irrlichtaction.cpp ../src/irrlicht/*.cpp -o irrlichtaction $(CFLAGS) \
        -lguichan -lIrrlicht -lGL -lXxf86vm -lGLU \
        -I../include -I/usr/src/irrlicht/include -L/usr/src/irrlicht/lib/Linux

clean:
	rm -f irrlichtaction irrlichthelloworld irrlichtwidgets