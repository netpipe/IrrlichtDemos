#------------------------------------------------------------------------------
#	Generic C++ Makefile
#------------------------------------------------------------------------------

IRRLICHT_DIR = /home/smso/downloads/irrlicht/irrlicht-svn
IRRLICHT_LIB = ${IRRLICHT_DIR}/lib/Linux
IRRLICHT_INCLUDE = ${IRRLICHT_DIR}/include


TARGET = r

#------------------------------------------------------------------------------

CC = g++
CFLAGS = -ansi -Wall -g -O3 -ffast-math ${INCLUDES} -DIRRLICHT_SVN
LDFLAGS = -L. -L${IRRLICHT_LIB} ${LIBS}

#------------------------------------------------------------------------------

SRC = main.cpp \
turn_control.cpp

OBJ = $(filter %.o, $(SRC:.cpp=.o))

#------------------------------------------------------------------------------


INCLUDES = -I. -I${IRRLICHT_INCLUDE}
LIBS = -lm -lIrrlicht -lGL -lXxf86vm -lXext -lX11

#------------------------------------------------------------------------------

.SUFFIXES:	.cc .cpp .o

.cc.o:
	${CC} ${CFLAGS} -c -o $@ $<

.cpp.o:
	${CC} ${CFLAGS} -c -o $@ $<


#------------------------------------------------------------------------------

all: ${TARGET}

${TARGET}: ${OBJ}
	${CC} ${OBJ} ${LDFLAGS} -o $@

phony: clean

clean:
	rm -f ${OBJ} ${TARGET}

#------------------------------------------------------------------------------

