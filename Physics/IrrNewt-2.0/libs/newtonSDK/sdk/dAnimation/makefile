#*******************************************************
# Newton game dynamics 
# copy right by Julio Jerez 2002 - 2005 
#
#*******************************************************
#
# Generic makefile 
# this make file generate the libraries: 
# dg, physics, and newton
#
#*******************************************************  


# ******************************************************
#
# low level library
#
# ******************************************************
LIB_NAME = libdAnimation.a

INCLUDED_PATH = .
SRC_PATH = .
SDK_MATH = ../dMath
SDK_CONTAINERS = ../dContainers
SDK_XML = ../thirdParty/tinyxml
SDK_COLLADA = ../thirdParty/collada-dom
SDK_COLLADA_DAE = ../thirdParty/collada-dom/dom/include
SDK_COLLADA_DOM = ../thirdParty/collada-dom/dom/include/1.4



LIB_SRCS = \
	$(SRC_PATH)/dClassInfo.cpp \
	$(SRC_PATH)/dAnimationClip.cpp \
	$(SRC_PATH)/dBone.cpp \
	$(SRC_PATH)/dBonesToPoseBinding.cpp \
	$(SRC_PATH)/dGlueToCollada.cpp \
	$(SRC_PATH)/dMesh.cpp \
	$(SRC_PATH)/dModel.cpp \
	$(SRC_PATH)/dPoseGenerator.cpp \
	$(SRC_PATH)/dSkinModifier.cpp 
	

# ******************************************************
#
# Allsource files
#
# ******************************************************
ALL_SRC_FILES = $(LIB_SRCS) 
DG_OBJ_FILES = $(ALL_SRC_FILES:.cpp=.o)

COMPILER = gcc

# Linux options	 gcc 4.2.2
CPU_FLAGS = -fPIC -O2 -msse -msse2 -ffloat-store -ffast-math -mfpmath=sse -freciprocal-math -funsafe-math-optimizations -fsingle-precision-constant 
FLAGS = -c -Wall -Wno-strict-aliasing -D_LINUX_VER  $(CPU_FLAGS) -I ../ -I$(INCLUDED_PATH) -I$(SDK_MATH) -I$(SDK_CONTAINERS) \
	-I$(SDK_XML) -I$(SDK_COLLADA) -I$(SDK_COLLADA_DAE) -I$(SDK_COLLADA_DOM)
 


.SUFFIXES : .o .cpp
.cpp.o :
	$(COMPILER) $(FLAGS) -o $@ $<	


# libraries targets
libTemp.a : $(DG_OBJ_FILES)
	ar r $@ $?
	strip -g -S -d -v $@  -o$@ 
	cp $@ ../$(LIB_NAME) 	
	


# clean all objects target
clean :
	rm $(DG_OBJ_FILES)
	touch $(ALL_SRC_FILES)
#	rm -*.a

 

	
