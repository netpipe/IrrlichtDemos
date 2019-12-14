#*******************************************************
#
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
# dg low level library
#
# ******************************************************
DG_INCLUDED_PATH = ../../source/core
DG_PATH = $(DG_INCLUDED_PATH)/
DG_SRCS = \
	$(DG_PATH)dg.cpp \
	$(DG_PATH)dgRef.cpp \
	$(DG_PATH)dgCRC.cpp \
	$(DG_PATH)dgTree.cpp \
	$(DG_PATH)dgNode.cpp \
	$(DG_PATH)dgTypes.cpp \
	$(DG_PATH)dgMatrix.cpp \
	$(DG_PATH)dgGoogol.cpp \
	$(DG_PATH)dgMemory.cpp \
	$(DG_PATH)dgSphere.cpp \
	$(DG_PATH)dgRandom.cpp \
	$(DG_PATH)dgThreads.cpp \
	$(DG_PATH)dgPolyhedra.cpp \
	$(DG_PATH)dgQuaternion.cpp \
	$(DG_PATH)dgConvexHull3d.cpp \
	$(DG_PATH)dgConvexHull4d.cpp \
	$(DG_PATH)dgIntersections.cpp \
	$(DG_PATH)dgAABBPolygonSoup.cpp \
	$(DG_PATH)dgSmallDeterminant.cpp \
	$(DG_PATH)dgPolygonSoupBuilder.cpp \
	$(DG_PATH)dgPolyhedraMassProperties.cpp \
	$(DG_PATH)dgDelaunayTetrahedralization.cpp
	

	
# ******************************************************
#
# Physics engine files
#
# ******************************************************
DG_INCLUDED_PHYSICS_PATH = ../../source/physics
DG_PHYSICS_PATH = $(DG_INCLUDED_PHYSICS_PATH)/
DG_PHYSICS_SRCS = \
	$(DG_PHYSICS_PATH)dgWorld.cpp \
	$(DG_PHYSICS_PATH)dgBody.cpp \
	$(DG_PHYSICS_PATH)dgMeshEffect.cpp \
	$(DG_PHYSICS_PATH)dgMeshEffect2.cpp \
	$(DG_PHYSICS_PATH)dgCollisionConvex.cpp \
	$(DG_PHYSICS_PATH)dgBallConstraint.cpp \
	$(DG_PHYSICS_PATH)dgBilateralConstraint.cpp \
	$(DG_PHYSICS_PATH)dgCorkscrewConstraint.cpp \
	$(DG_PHYSICS_PATH)dgHingeConstraint.cpp \
	$(DG_PHYSICS_PATH)dgNarrowPhaseCollision.cpp \
	$(DG_PHYSICS_PATH)dgBroadPhaseCollision.cpp \
	$(DG_PHYSICS_PATH)dgMinkowskiConv.cpp \
	$(DG_PHYSICS_PATH)dgCollisionBox.cpp \
	$(DG_PHYSICS_PATH)dgCollisionCapsule.cpp \
	$(DG_PHYSICS_PATH)dgCollisionBVH.cpp \
	$(DG_PHYSICS_PATH)dgCollisionMesh.cpp \
	$(DG_PHYSICS_PATH)dgCollisionUserMesh.cpp \
	$(DG_PHYSICS_PATH)dgCollisionChamferCylinder.cpp \
	$(DG_PHYSICS_PATH)dgPointToCurveConstraint.cpp \
	$(DG_PHYSICS_PATH)dgCollisionCone.cpp \
	$(DG_PHYSICS_PATH)dgCollisionConvexHull.cpp \
	$(DG_PHYSICS_PATH)dgSlidingConstraint.cpp \
	$(DG_PHYSICS_PATH)dgCollisionConvexModifier.cpp \
	$(DG_PHYSICS_PATH)dgUniversalConstraint.cpp \
	$(DG_PHYSICS_PATH)dgCollision.cpp \
	$(DG_PHYSICS_PATH)dgUpVectorConstraint.cpp \
	$(DG_PHYSICS_PATH)dgCollisionCylinder.cpp \
	$(DG_PHYSICS_PATH)dgUserConstraint.cpp \
	$(DG_PHYSICS_PATH)dgCollisionEllipse.cpp \
	$(DG_PHYSICS_PATH)dgCollisionNull.cpp \
	$(DG_PHYSICS_PATH)dgCollisionScene.cpp \
	$(DG_PHYSICS_PATH)dgCollisionCompoundBreakable.cpp \
	$(DG_PHYSICS_PATH)dgCollisionHeightField.cpp \
	$(DG_PHYSICS_PATH)dgWorldDynamicUpdate.cpp \
	$(DG_PHYSICS_PATH)dgCollisionSphere.cpp \
	$(DG_PHYSICS_PATH)dgCollisionCompound.cpp \
	$(DG_PHYSICS_PATH)dgConnectorConstraint.cpp \
	$(DG_PHYSICS_PATH)dgConstraint.cpp \
	$(DG_PHYSICS_PATH)dgBodyMasterList.cpp \
	$(DG_PHYSICS_PATH)dgContact.cpp


# ******************************************************
#
# Newton engine files
#g++ -shared -o libNewton.so libNewton.a
# ******************************************************
DG_INCLUDED_NEWTON_PATH = ../../source/newton
DG_NEWTON_PATH = $(DG_INCLUDED_NEWTON_PATH)/
DG_NEWTON_SRCS = \
	$(DG_NEWTON_PATH)Newton.cpp \
	$(DG_NEWTON_PATH)NewtonClass.cpp 

# ******************************************************
#
# Allsource files
#
# ******************************************************
ALL_SRC_FILES = $(DG_SRCS) $(DG_PHYSICS_SRCS) $(DG_NEWTON_SRCS)
DG_OBJ_FILES = $(ALL_SRC_FILES:.cpp=.o)

COMPILER = gcc

# Linux options	 gcc 4.2.2
#CPU_FLAGS = -fPIC  -O0 -g 
#CPU_FLAGS = -O2 -fsingle-precision-constant
CPU_FLAGS = -O2 -fpic -msse -msse2 -ffloat-store -ffast-math -freciprocal-math -funsafe-math-optimizations -fsingle-precision-constant 
	
#FLAGS = -c -Wall -Wno-strict-aliasing -D_LINUX_VER -ffloat-store -ffast-math -fsingle-precision-constant $(CPU_FLAGS) -I$(DG_INCLUDED_PATH) -I$(DG_INCLUDED_PHYSICS_PATH)
FLAGS = -c -Wall -Wno-strict-aliasing -D_LINUX_VER  $(CPU_FLAGS) -I$(DG_INCLUDED_PATH) -I$(DG_INCLUDED_PHYSICS_PATH)


.SUFFIXES : .o .cpp
.cpp.o :
	$(COMPILER) $(FLAGS) -o $@ $<	

# main target
engine : libNewton.a


# clean all objects target
clean :
	rm $(DG_OBJ_FILES)
	touch $(ALL_SRC_FILES)
#	rm -*.a

# libraries targets
libNewton.a : $(DG_OBJ_FILES)
	ar r $@ $?
	strip -g -S -d -v libNewton.a -olibNewton.a
	cp libNewton.a ../../../packages/Linux32/libNewton.a
	cp ../../source/newton/Newton.h ../../../packages/Linux32/Newton.h
	gcc -shared -Wl,-soname,libNewton.so $? -o libNewton.so 
	cp libNewton.so ../../../packages/Linux32/libNewton.so
	sudo cp libNewton.so /usr/lib

 

	
