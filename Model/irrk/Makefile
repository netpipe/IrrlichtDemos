#===============================================================================
#
#     Filename: Makefile
#  Description:
#
#        Usage: make              (generate executable                      )
#               make clean        (remove objects, executable, prerequisits )
#               make tarball      (generate compressed archive              )
#               make zip          (generate compressed archive              )
#
#      Version: 1.0
#      Created:
#     Revision: ---
#
#       Author:
#      Company:
#        Email:
#
#        Notes: This is a GNU make (gmake) makefile.
#               C   extension   :  c
#               C++ extensions  :  cc cpp C
#               C and C++ sources can be mixed.
#               Prerequisites are generated automatically; makedepend is not
#               needed (see documentation for GNU make Version 3.80, July 2002,
#               section 4.13). The utility sed is used.
#========================================== makefile template version 1.8 ======

# DEBUG can be set to YES to include debugging info, or NO otherwise
DEBUG          := YES

# PROFILE can be set to YES to include profiling info, or NO otherwise
PROFILE        := NO

# ------------  name of the executable  ----------------------------------------
EXECUTABLE      := irrik

# ------------  list of all source files  --------------------------------------
SOURCES         := irrik.cpp Target.cpp Input.cpp

# ------------  compiler  ------------------------------------------------------
CC              := gcc
CXX             := g++

# ------------  compiler flags  ------------------------------------------------
DEBUG_CFLAGS    := -Wall  -pedantic -O0 -ggdb3 
RELEASE_CFLAGS  := -Wall  -pedantic -O3

# ------------  linker flags  --------------------------------------------------
DEBUG_LDFLAGS    := -g -Wall
RELEASE_LDFLAGS  :=

ifeq (YES, ${DEBUG})
  CFLAGS       := ${DEBUG_CFLAGS}
  CXXFLAGS     := ${DEBUG_CXXFLAGS}
  LDFLAGS      := ${DEBUG_LDFLAGS}
else
  CFLAGS       := ${RELEASE_CFLAGS}
  CXXFLAGS     := ${RELEASE_CXXFLAGS}
  LDFLAGS      := ${RELEASE_LDFLAGS}
endif

ifeq (YES, ${PROFILE})
  CFLAGS       := ${CFLAGS}   -pg -O3
  CXXFLAGS     := ${CXXFLAGS} -pg -O3
  LDFLAGS      := ${LDFLAGS}  -pg
endif

# ------------  additional system include directories  -------------------------
GLOBAL_INC_DIR  =

# ------------  private include directories  -----------------------------------
LOCAL_INC_DIR   = $(HOME)/include ../Irrlicht/include 
# ------------  system libraries  (e.g. -lm )  ---------------------------------
SYS_LIBS        = -lm -lIrrlicht -lGL -lXxf86vm -lXext -lX11

# ------------  additional system library directories  -------------------------
GLOBAL_LIB_DIR  = 

# ------------  additional system libraries  -----------------------------------
GLOBAL_LIBS     = 
# ------------  private library directories  -----------------------------------
LOCAL_LIB_DIR   = $(HOME)/lib ../Irrlicht/lib/Linux

# ------------  private libraries  (e.g. libxyz.a )  ---------------------------
LOCAL_LIBS      =

# ------------  archive generation ---------------------------------------------
TARBALL_EXCLUDE = *.{o,gz,zip}
ZIP_EXCLUDE     = *.{o,gz,zip}

# ------------  run executable out of this Makefile  (yes/no)  -----------------
# ------------  cmd line parameters for this executable  -----------------------
EXE_START       = no
EXE_CMDLINE     =

#===============================================================================
# The following statements usually need not to be changed
#===============================================================================

C_SOURCES       = $(filter     %.c, $(SOURCES))
CPP_SOURCES     = $(filter-out %.c, $(SOURCES))
ALL_INC_DIR     = $(addprefix -I, $(LOCAL_INC_DIR) $(GLOBAL_INC_DIR))
ALL_LIB_DIR     = $(addprefix -L, $(LOCAL_LIB_DIR) $(GLOBAL_LIB_DIR))
GLOBAL_LIBSS    = $(addprefix $(GLOBAL_LIB_DIR)/, $(GLOBAL_LIBS))
LOCAL_LIBSS     = $(addprefix $(LOCAL_LIB_DIR)/, $(LOCAL_LIBS))
ALL_CFLAGS      = $(CFLAGS) $(ALL_INC_DIR)
ALL_LFLAGS      = $(LDFLAGS) $(ALL_LIB_DIR)
BASENAMES       = $(basename $(SOURCES))

# ------------  generate the names of the object files  ------------------------
OBJECTS         = $(addsuffix .o,$(BASENAMES))

# ------------  generate the names of the hidden prerequisite files  -----------
PREREQUISITES   = $(addprefix .,$(addsuffix .d,$(BASENAMES)))

# ------------  make the executable (the default goal)  ------------------------
all: $(EXECUTABLE)
$(EXECUTABLE):	$(OBJECTS)
ifeq ($(strip $(CPP_SOURCES)),)
								$(CC)  $(ALL_LFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LOCAL_LIBSS) $(GLOBAL_LIBSS) $(SYS_LIBS)
else
								$(CXX) $(ALL_LFLAGS) -o $(EXECUTABLE) $(OBJECTS) $(LOCAL_LIBSS) $(GLOBAL_LIBSS) $(SYS_LIBS)
endif
ifeq ($(EXE_START),yes)
								./$(EXECUTABLE) $(EXE_CMDLINE)
endif

# ------------  include the automatically generated prerequisites  -------------
# ------------  if target is not clean, tarball or zip             -------------
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(MAKECMDGOALS),tarball)
ifneq ($(MAKECMDGOALS),zip)
include         $(PREREQUISITES)
endif
endif
endif

# ------------  make the objects  ----------------------------------------------
%.o:		%.c
				$(CC)  -c $(ALL_CFLAGS) $<

%.o:		%.cc
				$(CXX) -c $(ALL_CFLAGS) $<

%.o:		%.cpp
				$(CXX) -c $(ALL_CFLAGS) $<

%.o:		%.C
				$(CXX) -c $(ALL_CFLAGS) $<

# ------------  make the prerequisites  ----------------------------------------
#
.%.d:   %.c
				@$(make-prerequisite-c)

.%.d:		%.cc
				@$(make-prerequisite-cplusplus)

.%.d:		%.cpp
				@$(make-prerequisite-cplusplus)

.%.d:		%.C
				@$(make-prerequisite-cplusplus)

#  canned command sequences
#  echoing of the sed command is suppressed by the leading @

define	make-prerequisite-c
				@$(CC)   -MM $(ALL_CFLAGS) $< > $@.$$$$;            \
				sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' < $@.$$$$ > $@; \
				rm -f $@.$$$$;
endef

define	make-prerequisite-cplusplus
				@$(CXX)  -MM $(ALL_CFLAGS) $< > $@.$$$$;            \
				sed 's/\($*\)\.o[ :]*/\1.o $@ : /g' < $@.$$$$ > $@; \
				rm -f $@.$$$$;
endef

# ------------  remove generated files  ----------------------------------------
# ------------  remove hidden backup files  ------------------------------------
clean:
								-rm  --force  $(EXECUTABLE) $(OBJECTS) $(PREREQUISITES) *~

# ------------ tarball generation ----------------------------------------------
tarball:
					@lokaldir=`pwd`; lokaldir=$${lokaldir##*/}; \
					rm --force $$lokaldir.tar.gz;               \
					tar --exclude=$(TARBALL_EXCLUDE)            \
					    --create                                \
					    --gzip                                  \
					    --verbose                               \
					    --file  $$lokaldir.tar.gz *

# ------------ zip -------------------------------------------------------------
zip:
					@lokaldir=`pwd`; lokaldir=$${lokaldir##*/}; \
					zip -r  $$lokaldir.zip * -x $(ZIP_EXCLUDE)

.PHONY: clean tarball zip

# ==============================================================================
# vim: set tabstop=2: set shiftwidth=2:
