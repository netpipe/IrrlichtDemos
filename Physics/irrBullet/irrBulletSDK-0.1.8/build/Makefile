################################################################
# Linux Makefile for irrBullet (modified version of PIM)       #
#                                                              #
# Author: Johannes Lorenz                                      #
# You can contact me on http://odf.sourceforge.net !           #
#                                                              #
# This code is licensed under GPL 3.                           #
################################################################

################################################################
# Fill in your options here:

# alternative system makefile (e.g. 'Makefile.win' for windows, or leave empty for none)
SYS_MAKEFILE = Makefile.lin
# name of the installation Makefile
# (leave empty or comment out if there is none, standard is "Makefile.install")
INSTALL_MAKEFILE = Makefile.install

# compiler (you may insert the full path here)
CC= g++
# tar archive program (you may insert the full path here)
TAR = tar

# compiler flags for compiling and linking
FLAGS = -Wall -ansi -O3
# flags for the headers location
CFLAGS= -I/usr/local/include/irrlicht/ -I/usr/include/Bullet
# flags for the libs location
LIBS= -shared -fPIC -L/usr/lib/Bullet -lIrrlicht -lGL -lXxf86vm -lXext -lX11 -lbulletdynamics -lbulletcollision

# file endings that are recognized as source code (REGEXP, MUST BEGIN WITH .\)
ENDING = '\.cpp$$'

# in which directories shall we search for source code?
SEARCH_IN_DIRS = '../source'
# shall we also search in the subdirectories of those paths?
SEARCH_RECURSIVE = 'N'

# name of the file, where all the dependencies are stored in...
DEPENDFILE = .dependfile
# name of the outfile
OF = ../lib/linux/Release/libirrBullet.so
# softlink to the outfile
# (leave empty or comment out if there shall be none )
OF_SLINK = 
# files that shall be removed (e.g. files that will be created by the finished program)
#REMOVABLE_FILES = 

################################################################
# Do not change any code under this line!
# The whole Makefile...

# check for system specific additions
ifdef SYS_MAKEFILE
 include $(SYS_MAKEFILE)
 ifdef ALT_CC
  CC = $(ALT_CC)
 endif
 ifdef ALT_OF
  OF = $(ALT_OF)
 endif
FLAGS+=$(ADD_FLAGS)
CFLAGS+=$(ADD_CFLAGS)
LIBS+=$(ADD_LIBS)
DIRS+=$(ADD_DIRS)
endif

# get full source-file names with path
ifeq ($(SEARCH_RECURSIVE),'Y') # recursive search: all files in all dirs and subdirs of $(SEARCH_IN_DIRS)
SOURCES = $(shell find $(SEARCH_IN_DIRS) | grep $(ENDING) )
else # non-recursive-search: no subdirs
#SOURCES = $(shell TMP_DIRS=`echo $(SEARCH_IN_DIRS)`; for i in $$TMP_DIRS; do FNAME=`ls $$i`; for j in $$FNAME; do find . -name $$j| grep $(ENDING)$$;done ; done )
SOURCES = $(shell find . `echo $(SEARCH_IN_DIRS)` -maxdepth 1 | grep $(ENDING))
endif

#  get full object file names with path
OBJ = $(shell echo $(SOURCES) | sed 's/$(ENDING)/\.o/g')

TARGETS = create $(OF)

# phony targets shall be all targets that are not files
.PHONY: all release depend create dry compile cleano clean cleand cleanf tidy final relink remake rebuild tgz tbz2 kmail check_installing install uninstall show_install show_uninstall help info

# define SILENT tags, i.e those targets will not show what they're doing
.SILENT : depend check_installing

#all : create
#	$(MAKE) $(OF)
all : create $(OF)

#release :
#FLAGS+=$(RELEASEF)

# proceed/create the depend file
$(DEPENDFILE) :
	@echo "Checking dependencies..."
	if test -f $(DEPENDFILE); then rm -f $(DEPENDFILE); fi
	for i in $(SOURCES);do \
	$(CC) $(CFLAGS) $(FLAGS) -MM -MG $$i -MT `echo $$i | sed 's/$(ENDING)$$/.o/g'` >> $(DEPENDFILE); RES=$$?; \
	if [ $$RES = 0 ]; then printf '	$(CC) $(FLAGS) $(CFLAGS) -c %b -o %b\n' $$i `echo $$i|sed 's/$(ENDING)$$/\.o/g'`>> $(DEPENDFILE); else rm $(DEPENDFILE); false; fi; \
	done

create: $(DEPENDFILE)

-include $(DEPENDFILE)

$(OF): $(OBJ)
#	$(CC) $(FLAGS) $(LIBS) $(OBJ) -o $(OF)
	$(CC) $(FLAGS) $(OBJ) -o $(OF) $(LIBS)
ifdef OF_SLINK
	if ! test -f $(OF_SLINK); then ln -s $(OF) $(OF_SLINK); fi
endif
	@echo "...type 'make help' for more information"

dry:
	$(MAKE) -n -B $(OF)

compile: $(OF)

cleano:
	rm -f $(OF) # remove outfile

clean:	cleano
	rm -f $(OBJ) # remove all objective files
ifdef OF_SLINK
	if test -f $(OF_SLINK); then rm -f $(OF_SLINK); fi
endif

cleand:
	rm -f $(DEPENDFILE) # remove the dependfile

cleanf:
ifdef REMOVABLE_FILES	
	rm -f $(REMOVABLE_FILES) # remove removable files
endif
	
tidy: clean cleand cleanf # remove everything... (except the sources)

final:
	rm -f $(OBJ) # remove all objective files, they're temporary

relink: cleano $(OF)
remake: clean $(OF)
rebuild: tidy all

CUR_PATH = $(shell echo $$PWD | sed 's/.*\///g' )

tgz:
	$(TAR) -C .. -cz --owner=0 --group=0 -f ../$(CUR_PATH).tar.gz $(CUR_PATH)
tbz2:
	$(TAR) -C .. -cj --owner=0 --group=0 -f ../$(CUR_PATH).tar.bz2 $(CUR_PATH)

################################################################
# The installation code...

ifdef INSTALL_MAKEFILE
-include $(INSTALL_MAKEFILE) # include the installation Makefile
NO_INSTALL = 'INSTALL_MAKEFILE_SPECIFIED'
else
NO_INSTALL = 'NO_INSTALL_MAKEFILE_SPECIFIED'
endif

check_installing:
	if [ $(NO_INSTALL) = 'NO_INSTALL_MAKEFILE_SPECIFIED' ]; then echo "NO INSTALL MAKEFILE SPECIFIED!"; false; fi
	if ! test -f $(INSTALL_MAKEFILE); then echo "SPECIFIED INSTALL MAKEFILE DOES NOT EXIST!"; false; fi # Does this Makefile exist?
	@echo "NO ERRORS, READY FOR (DE)INSTALLATION! :)"

install: check_installing install_extern
uninstall: check_installing uninstall_extern
show_install: check_installing
	$(MAKE) -n -B -f $(INSTALL_MAKEFILE) install_extern # -n lets make print out the results but does not execute them
show_uninstall: check_installing
	$(MAKE) -n -B -f $(INSTALL_MAKEFILE) uninstall_extern # -n lets make print out the results but does not execute them

################################################################
# The help code...

help:
	@echo ""
	@echo "Linux Makefile for '"$(OF)"'"
	@echo ""
	@echo "Help for this Makefile:"
	@echo ""
	@echo "Type                   Action"
	@echo "==============================================================================="
	@echo ""
	@echo "make                   compile '"$(OF)"'"
	@echo "make depend            proceed or refresh depend-file '"$(DEPENDFILE)"'"
	@echo "make compile           the same as just make"
	@echo "make dry               dry run of compile - compile nothing, just print results"
	@echo "make all               the same as make depend and make compile together"
	@echo ""
	@echo "make clean             remove all objective files and the outfile '"$(OF)"'"
	@echo "make cleano            remove only the outfile '"$(OF)"'"
	@echo "make cleand            remove the dependfile '"$(DEPENDFILE)"'"
	@echo "make cleanf            remove temporary files in this directory"
	@echo "make tidy              the same as make clean and make cleand together"
	@echo "make final             remove all objective files (used to make a binary packet smaller)"
	@echo ""
	@echo "make relink            removes outfile and links objectives again"
	@echo "make remake            removes outfile and objective and compiles again "
	@echo "make rebuild           the same as remake, but removes dependencies and tmp files, too"
	@echo ""
ifdef TAR
	@echo "make tgz               creates" $(CUR_PATH).tar.gz "in .. with contents of ."
	@echo "make tbz2              creates" $(CUR_PATH).tar.bz2 "in .. with contents of ."
endif
	@echo ""
ifdef INSTALL_MAKEFILE
	@echo "The next 2 Options can be used by checkinstall..."
	@echo "make install           install everything (source in '"$(INSTALL_MAKEFILE)"')"
	@echo "make uninstall         uninstall everything (source in '"$(INSTALL_MAKEFILE)"')"
	@echo "make show_install      show how install would work, do not install"
	@echo "make show_uninstall    show how uninstall would work, do not uninstall"
	@echo ""
endif
	@echo "make help              show this help page"
	@echo ""
	@echo "==============================================================================="
	@echo ""
	@echo "The following variables might be important for you (find more in the Makefile):"
	@echo "CC=<(path to) your compiler>, default: "$(CC)
	@echo "TAR=<(path to) your tar archive program>, default: "$(TAR)
	@echo ""
	@echo "If you want to specify options before compiling, try './configure' before make!"
	@echo ""
	@echo "This Makefile is a modification of PIM and licensed under GPL 3."
	@echo "PIM can be downloaded from http://odf.sourceforge.net"
	@echo ""

info: help



