PROJECT = CASTLE_BATTLE
WORKDIR = `pwd`

#Paths
SOURCE_PATH = ./src/
BIN_PATH = ./bin/
IRRLICHT_INCLUDE_PATH = ../Engine/include
BULLET_INCLUDE_PATH = /usr/local/include/bullet

#Compiler options
CXX = g++
CXXFLAGS = -std=c++11 -Wall
#NDEBUG: disable assertion
#DEBUG_MODE: enable assertion(only on debug mode)
RELEASE_OPTIONS = -O -Wswitch -DNDEBUG
DEBUG_OPTIONS = -g -Wswitch -DDEBUG_MODE
CXXFLAGS_DEBUG = $(CXXFLAGS) $(DEBUG_OPTIONS)
CXXFLAGS_RELEASE = $(CXXFLAGS) $(RELEASE_OPTIONS)

#Library path
BASELIBDIR = -L/usr/local/lib

#Irrlicht library path
IRRLICHT_LIB_DIR = -L../Engine/lib/Linux

#Linker flags
LDFLAGS = -lIrrlicht -lGL -lX11 -lXext -lXxf86vm -lBulletSoftBody -lBulletDynamics -lBulletCollision -lLinearMath

#Directories of the project
SUBDIRS := $(shell ls -d $(SOURCE_PATH)*/)
DIRS = ./ $(SUBDIRS) #add the current dir

#Get all the cpp files
SOURCES := $(foreach d, $(DIRS), $(wildcard $(d)*.cpp))

#Generate object files names
OBJECTS = $(patsubst %.cpp, %.o, $(SOURCES))

#Generate dependencies
DEPENDENCIES = $(patsubst %.cpp, %.depend, $(SOURCES))


#Prepend -I to INCLUDE directories for the compiler
INCLUDE_SUBDIRS := $(foreach d, $(DIRS), -I$d)

#Include all up together
INCLUDE := $(INCLUDE_SUBDIRS) -I$(IRRLICHT_INCLUDE_PATH) -I$(BULLET_INCLUDE_PATH)

#Compose linker options
LIBDIR =  $(BASELIBDIR) $(IRRLICHT_LIB_DIR) $(LDFLAGS)

#Output variables
OUT_DEBUG = bin/Debug/
OUT_RELEASE = bin/Release/

#0NLY ON DEBUG MODE: check if debug verbosity is passed from command line. If not, set the default verbosity to 1
#Example: make VERBOSITY=1 debug or simply make debug
ifeq ($(VERBOSITY),)
VERBOSITY = 1
endif


#DEFAULT TARGET is set to release mode
#Put here $(DEPENDENCIES) to refresh dependecies of the each file every make if something header has changed. Otherwise simply run make depend
all: release

#Release version
release: before_release out_release after_release run-release

before_release:
	@echo "Checking directories"
	test -d $(OUT_RELEASE) || mkdir -p bin/Release
	$(eval CXXFLAGS += $(RELEASE_OPTIONS))

after_release:
	@echo "Release version building finished!"

out_release: before_release $(OBJECTS) after_release
	$(CXX) $(CXXFLAGS) -o $(OUT_RELEASE)$(PROJECT) $(OBJECTS) $(LIBDIR) $(INCLUDE)
ifneq "$(strip $(DEPENDENCIES))" ""
 -include $(DEPENDENCIES)
endif


.PHONY: cleanall clean debug debug1 debug2 run-debug run-release doc doc-show depend-unic depend

debug1: CXXFLAGS += $(DEBUG_OPTIONS) -DDEBUG_OUTPUT_MASK=1
debug1: out_debug run-debug

debug2: CXXFLAGS += $(DEBUG_OPTIONS) -DDEBUG_OUTPUT_MASK=2
debug2: out_debug run-debug

#Debug version. Verbosity is automatically set to 1. Check VERBOSITY variable
debug:  CXXFLAGS += $(DEBUG_OPTIONS) -DDEBUG_OUTPUT_MASK=$(VERBOSITY)
debug: out_debug run-debug

debug-noflag: CXXFLAGS += $(DEBUG_OPTIONS)
debug-noflag: out_debug run-debug

before_debug:
	test -d $(OUT_DEBUG) || mkdir -p bin/Debug

after_debug:
	@echo "Debug version building finished!"

out_debug: before_debug $(OBJECTS) after_debug
	$(CXX) $(CXXFLAGS_DEBUG) -o ./$(OUT_DEBUG)$(PROJECT) $(OBJECTS) $(LIBDIR) $(INCLUDE)
ifneq "$(strip $(DEPENDENCIES))" ""
-include $(DEPENDENCIES)
endif

run-debug:
	./$(OUT_DEBUG)$(PROJECT)

run-release:
	./$(OUT_RELEASE)/$(PROJECT)
#Clean all objects, executables, docs and dependecies files
cleanall: clean
	rm -rf $(OUT_RELEASE)
	rm -rf $(OUT_DEBUG)
	rm -rf ./docs

clean:
	find . -name '*.o' -delete
	find . -name '*.depend*' -delete
	find . -name 'dependecies' -delete

#Generate documentation
doc:
	doxygen Doxyfile
doc-show:
	sensible-browser ./docs/html/index.html

#Generate object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@ $(INCLUDE)
#Automatically generate dependecy files in each respective sub-dir. This approach is much more modular and better than an unique dependecies file.
#You can also put -MMD when compiling each file to generate automatically a .d file, but i use this.
#Only this type of file generation will be used by g++ in the compiling process.
%.depend: %.cpp
	$(CXX) $(INCLUDE_SUBDIRS)  -MM $< -MT "$*.o" -MF $*.depend

#Generate dependencies
depend: $(DEPENDECIES)

#if you want to generate an unique file with all dependecies
depend-unique:
	$(CXX) $(INCLUDE_SUBDIRS) -MM $(SOURCES) > dependecies
gotorepo:
	sensible-browser https://github.com/iwasingh/CastleBattle
