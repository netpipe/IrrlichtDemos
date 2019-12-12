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
LIB_NAME = libdJointLibrary.a

INCLUDED_PATH = .
SRC_PATH = .
SDK_MATH = ../dMath
SDK_CONTAINERS = ../dContainers


LIB_SRCS = \
	$(SRC_PATH)/Custom6DOF.cpp \
	$(SRC_PATH)/CustomBallAndSocket.cpp \
	$(SRC_PATH)/CustomCorkScrew.cpp \
	$(SRC_PATH)/CustomDryRollingFriction.cpp \
	$(SRC_PATH)/CustomGear.cpp \
	$(SRC_PATH)/CustomHinge.cpp \
	$(SRC_PATH)/CustomMultiBodyVehicle.cpp \
	$(SRC_PATH)/CustomPlayerController.cpp \
	$(SRC_PATH)/CustomPulley.cpp \
	$(SRC_PATH)/CustomRagDoll.cpp \
	$(SRC_PATH)/CustomKinematicController.cpp \
	$(SRC_PATH)/CustomDGRayCastCar.cpp \
	$(SRC_PATH)/CustomSlider.cpp \
	$(SRC_PATH)/CustomSlidingContact.cpp \
	$(SRC_PATH)/CustomUniversal.cpp \
	$(SRC_PATH)/CustomUpVector.cpp \
	$(SRC_PATH)/CustomWormGear.cpp \
	$(SRC_PATH)/CustomUserBlank.cpp \
	$(SRC_PATH)/JointLibrary.cpp \
	$(SRC_PATH)/NewtonCustomJoint.cpp
	

# ******************************************************
#
# Allsource files
#
# ******************************************************
ALL_SRC_FILES = $(LIB_SRCS) 
DG_OBJ_FILES = $(ALL_SRC_FILES:.cpp=.o)

COMPILER = gcc

# Linux options	 gcc 4.2.2
CPU_FLAGS = -fPIC -O2 -msse  -msse2 -ffloat-store -ffast-math -freciprocal-math -funsafe-math-optimizations -fsingle-precision-constant 
FLAGS = -c -Wall -Wno-strict-aliasing -D_LINUX_VER  $(CPU_FLAGS) -I ../ -I$(INCLUDED_PATH) -I$(SDK_MATH) -I$(SDK_CONTAINERS) 


.SUFFIXES : .o .cpp
.cpp.o :
	$(COMPILER) $(FLAGS) -o $@ $<	


# libraries targets
libTemp.a : $(DG_OBJ_FILES)
	ar r $@ $?
	strip -g -S -d -v $@  -o$@ 
	cp $@ ../$(LIB_NAME) 	
	
	#gcc -shared -Wl,-soname,libNewton.so $? -o libNewton.so 
	#cp libNewton.so ../newtonSDK/sdk/libNewton.so
	#sudo cp libNewton.so /usr/lib


# clean all objects target
clean :
	rm $(DG_OBJ_FILES)
	touch $(ALL_SRC_FILES)
#	rm -*.a

 

	
