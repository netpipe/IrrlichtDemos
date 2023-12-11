all: xurfaced

xurfaced:
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/limb/llist.c -o src/limb/llist.o
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/config.c -o src/config.o
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/client.c -o src/client.o
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/display.c -o src/display.o
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/event.c -o src/event.o
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/xurfaced.c -o src/xurfaced.o
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/menu.c -o src/menu.o
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/render.c -o src/render.o
	gcc -Wall -I./include `pkg-config --cflags x11 xrender xcomposite cairo` -c src/window.c -o src/window.o
	gcc -Wall -lm -lX11 -lXxf86vm -lpthread `pkg-config --libs x11 xrender xcomposite cairo` src/limb/llist.o src/config.o src/client.o src/display.o src/event.o src/xurfaced.o src/menu.o src/render.o src/window.o -o xurfaced

install:
	cp ./xurfaced /usr/bin/
	rm -rf /usr/share/xurfaced
	mkdir -p /usr/share/xurfaced
	cp -r ./share/* /usr/share/xurfaced

clean:
	rm -f xurfaced
	rm -f src/*.o
	rm -f src/limb/*.o

