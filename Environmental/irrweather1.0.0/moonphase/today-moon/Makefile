##
#	Makefile for very verbose date command (today).
#
#Str1	= \"", Eastern Daylight Savings Time.  "\"
#Str2	= \"", Eastern Standard Time.  "\"
#HOME	= /users/sun3/jad
Str1	= \"", EDT."\"
Str2	= \"", EST."\"
BINDIR	= ${HOME}/bin
CFLAGS	= -O -DTZ_Str1=${Str1} -DTZ_Str2=${Str2} -DNJC
TODAY	= datetx.o moontx.o nbrtxt.o timetx.o today.o sun.o
POTM	= moontx.o potm.o
SUN	= moontx.o sun.o
BIN	= potm today sunrise sunset
LIB	= -lm

all:	today potm sunset sunrise

potm:	${POTM}
	cc ${CFLAGS} -o $@ ${POTM} ${LIB}

today:	${TODAY}
	cc ${CFLAGS} -o $@ ${TODAY} ${LIB}

sunrise: ${SUN}
	cc ${CFLAGS} $@.c -o $@ ${SUN} ${LIB}

sunset:	${SUN}
	cc ${CFLAGS} $@.c -o $@ ${SUN} ${LIB}

moontx.o:	moontx.h

install:	all
	mv -f ${BIN} ${BINDIR}
	rm -f ,* *.o ERRS core a.out

clean:
	rm -f ${BIN} a.out core ERRS ,* *.o *~ core foo
