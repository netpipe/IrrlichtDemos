SHELL	= sh

MODS	= miniOGL mini ministub minitile

SRCS	= $(MODS:=.cpp)
OBJS	= $(MODS:=.o)

AR	= ar -crs
RM	= rm -rf

.SUFFIXES: .cpp

.cpp.o:
	$(COMPILER) $(OPTS) -c $*.cpp

trap:

lib:	$(OBJS)
	@-$(AR) libMini.a $(OBJS)

example: lib
	$(COMPILER) $(OPTS) -o example -I. example.cpp -L. -lMini $(LINK) -lGL -lGLU -lm

depend:
	@-$(MAKEDEPEND) $(SRCS) >Makefile.d 2>/dev/null

clean:
	@-$(RM) $(OBJS) ii_files

tidy:	clean
	@-$(RM) Debug Release *.ncb *.opt *.plg
	@-$(RM) libMini.a libMini example

sinclude Makefile.d
