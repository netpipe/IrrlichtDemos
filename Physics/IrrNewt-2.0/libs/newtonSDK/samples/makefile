#*******************************************************
#
# Newton game dynamics 
# copy right by Julio Jerez 2003 - 2005 
#
#*******************************************************
#
# SDK Sample tutorial: 
#
#*******************************************************  


# ******************************************************
#
# project sorce files
#
# ******************************************************


# generic sdk flags
SDK_FLAGS = -O0 -c -Wall -D_LINUX_VER -D__WXGTK__ -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -g -pg
#SDK_FLAGS = -m32 -O3 -c -Wall -Werror -D_LINUX_VER -g 
#SDK_FLAGS = -m32 -O3 -c -Wall -D_LINUX_VER -g 


COLLADA_LIB_DIR = ../sdk/thirdParty/COLLADA_DOM/lib/


SDK_MAIN = sdkDemos
SDK_DEMOS = sdkDemos/demos
SDK_TOOLBOX = sdkDemos/toolBox
SDK_NEWTON = ../sdk
SDK_MATH = ../sdk/dMath
SDK_JOINTS = ../sdk/dCustomJoints
SDK_ANIMATION = ../sdk/dAnimation
SDK_CONTAINERS = ../sdk/dContainers
SDK_XML = ../sdk/thirdParty/tinyxml

WX_WIDGETS = thirdParties/wxGTK-2.8.10/buildgtk
WX_WIDGETS_LIB = thirdParties/wxGTK-2.8.10/buildgtk/lib
WX_WIDGETS_INCLUDE = thirdParties/wxGTK-2.8.10/include
WX_WIDGETS_INCLUDE_SETUP = thirdParties/wxGTK-2.8.10/buildgtk/lib/wx/include/gtk2-ansi-release-static-2.8

SDK_COLLADA = ../sdk/thirdParty/collada-dom
SDK_COLLADA_DAE = ../sdk/thirdParty/collada-dom/dom/include
SDK_COLLADA_DOM = ../sdk/thirdParty/collada-dom/dom/include/1.4
SDK_COLLADA_LIB = ../sdk/thirdParty/collada-dom/dom/build/linux-1.4



#LIBS = -L/usr/X11R6/lib -lGL -lGLU -lGLEW -lpthread -lrt -lglib-2.0 -lpcrecpp -lpcre -lboost_filesystem-gcc42-1_35.a lboost_system-gcc42-mt-1_35.a -static
LIBS  = -L/usr/lib -lgtk-x11-2.0 -lGL -lGLU -lGLEW -lpthread -lrt -lglib-2.0 -lgthread-2.0 -lpcrecpp -lpcre -lboost_system-mt
			

# this use newton as shared or static library
#NEWTON_LIB = -lNewton
NEWTON_LIB = $(SDK_NEWTON)/libNewton.a $(SDK_NEWTON)/libdJointLibrary.a $(SDK_NEWTON)/libdMath.a $(SDK_NEWTON)/libdContainers.a $(SDK_NEWTON)/libdAnimation.a


THIRD_PARTY_LIBRARIES = $(SDK_COLLADA_LIB)/libminizip.a $(SDK_COLLADA_LIB)/libcollada14dom.a \
			$(SDK_NEWTON)/libTinyXML.a \
			$(WX_WIDGETS_LIB)/libwx_gtk2_core-2.8.a $(WX_WIDGETS_LIB)/libwx_base-2.8.a $(WX_WIDGETS_LIB)/libwx_gtk2_gl-2.8.a \
			$(WX_WIDGETS_LIB)/libwx_gtk2_adv-2.8.a $(WX_WIDGETS_LIB)/libwx_gtk2_aui-2.8.a $(WX_WIDGETS_LIB)/libwx_gtk2_richtext-2.8.a \
			$(WX_WIDGETS_LIB)/libwx_base_net-2.8.a $(WX_WIDGETS_LIB)/libwxtiff-2.8.a $(WX_WIDGETS_LIB)/libwx_gtk2_xrc-2.8.a \
			$(WX_WIDGETS_LIB)/libwx_gtk2_qa-2.8.a $(WX_WIDGETS_LIB)/libwx_gtk2_html-2.8.a $(WX_WIDGETS_LIB)/libwx_base_xml-2.8.a



SDK_INCLUDE_PATH = -I $(SDK_MAIN) -I $(SDK_DEMOS) -I $(SDK_TOOLBOX) -I $(SDK_MATH) -I $(SDK_JOINTS) -I $(SDK_NEWTON) -I $(SDK_XML) -I $(SDK_ANIMATION) -I $(SDK_CONTAINERS) \
		   -I $(WX_WIDGETS_INCLUDE) -I $(WX_WIDGETS_INCLUDE_SETUP) -I$(SDK_COLLADA_DAE) -I$(SDK_COLLADA_DOM)


SDK_MAIN_SRC = \
	$(SDK_MAIN)/SceneManager.cpp \
	$(SDK_MAIN)/OGLMesh.cpp \
	$(SDK_MAIN)/OGLModel.cpp \
	$(SDK_MAIN)/PhysicsUtils.cpp \
	$(SDK_MAIN)/RenderCanvas.cpp \
	$(SDK_MAIN)/MainFrame.cpp 

	

SDK_DEMOS_SRC = \
	$(SDK_DEMOS)/BasicSphereSpin.cpp \
	$(SDK_DEMOS)/BasicRestitution.cpp \
	$(SDK_DEMOS)/BasicFriction.cpp \
	$(SDK_DEMOS)/ArchimedesBuoyancy.cpp \
	$(SDK_DEMOS)/BasicCustomJoints.cpp \
	$(SDK_DEMOS)/BasicRobots.cpp \
	$(SDK_DEMOS)/BasicRagDoll.cpp \
	$(SDK_DEMOS)/BasicStacking.cpp \
	$(SDK_DEMOS)/CompoundCollision.cpp \
	$(SDK_DEMOS)/ClosestDistance.cpp \
	$(SDK_DEMOS)/ConvexCast.cpp \
	$(SDK_DEMOS)/Jengas.cpp \
	$(SDK_DEMOS)/LegacyJoints.cpp \
	$(SDK_DEMOS)/MeshCollision.cpp \
	$(SDK_DEMOS)/PrecessingTops.cpp \
	$(SDK_DEMOS)/Pyramids.cpp \
	$(SDK_DEMOS)/RayCastCar.cpp \
	$(SDK_DEMOS)/ScaledCollision.cpp \
	$(SDK_DEMOS)/SpecialTrackJoints.cpp \
	$(SDK_DEMOS)/Towers.cpp \
	$(SDK_DEMOS)/TriggersAndForceFields.cpp \
	$(SDK_DEMOS)/UnstableStacks.cpp \
	$(SDK_DEMOS)/CharacterController.cpp \
	$(SDK_DEMOS)/Walls.cpp 
	
#	$(SDK_DEMOS)/BuildingDestruction.cpp \
#	$(SDK_DEMOS)/RealTimeSimpleDestruction.cpp \
#	$(SDK_DEMOS)/PrefabSimpleDestruction.cpp \


SDK_TOOLBOX_SRC = \
	$(SDK_TOOLBOX)/DebugDisplay.cpp \
	$(SDK_TOOLBOX)/DGCamera.cpp \
	$(SDK_TOOLBOX)/dRuntimeProfiler.cpp \
	$(SDK_TOOLBOX)/HeightFieldPrimitive.cpp \
	$(SDK_TOOLBOX)/HiResTimer.cpp  \
	$(SDK_TOOLBOX)/LevelPrimitive.cpp \
	$(SDK_TOOLBOX)/MousePick.cpp \
	$(SDK_TOOLBOX)/OpenGlUtil.cpp \
	$(SDK_TOOLBOX)/PlaneCollision.cpp \
	$(SDK_TOOLBOX)/RenderPrimitive.cpp \
	$(SDK_TOOLBOX)/ScenePrimitive.cpp \
	$(SDK_TOOLBOX)/SkyBox.cpp \
	$(SDK_TOOLBOX)/TargaToOpenGl.cpp \
	$(SDK_TOOLBOX)/toolbox_stdafx.cpp \
	$(SDK_TOOLBOX)/UserHeightFieldCollision.cpp 


		
SDK_SRCS = $(SDK_MAIN_SRC) $(SDK_DEMOS_SRC) $(SDK_TOOLBOX_SRC) 

SDK_OBJS = $(SDK_SRCS:.cpp=.o)

COMPILER = g++
FLAGS = $(SDK_FLAGS) $(SDK_INCLUDE_PATH)


.SUFFIXES : .o .cpp
.cpp.o :
	$(COMPILER) $(FLAGS) -o $@ $<	

# main target
NewtonDemos : $(SDK_OBJS) 
	make -C $(SDK_MATH)
	make -C $(SDK_CONTAINERS)
	make -C $(SDK_ANIMATION)	
	make -C $(SDK_JOINTS)
	make -C $(SDK_XML)
	make -C $(WX_WIDGETS)
	make os=linux parser=tinyxml project=minizip -C $(SDK_COLLADA)/dom
	make os=linux parser=tinyxml project=dom -C $(SDK_COLLADA)/dom
	gcc $? $(NEWTON_LIB) $(LIBS) $(THIRD_PARTY_LIBRARIES)  -o $@.elf	


# clean all objects target
clean :
	make clean -C $(SDK_MATH)
	make clean -C $(SDK_CONTAINERS)
	make clean -C $(SDK_ANIMATION)
	make clean -C $(SDK_JOINTS)
	make clean -C $(SDK_XML)	
	make clean -C $(WX_WIDGETS)
	make os=linux parser=tinyxml project=minizip clean -C $(SDK_COLLADA)/dom
	make os=linux parser=tinyxml project=dom clean -C $(SDK_COLLADA)/dom
	rm $(SDK_OBJS)
	touch $(SDK_SRCS)
	

	
	
	
	
