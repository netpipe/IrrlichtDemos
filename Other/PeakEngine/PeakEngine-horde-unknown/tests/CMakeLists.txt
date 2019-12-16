
add_subdirectory (test1)

project(peakbin)
set (MAIN_SRC main.cpp)

set (EXECUTABLE_OUTPUT_PATH ../bin)
set (CMAKE_CXX_FLAGS "-g")

include_directories(../engine/include /usr/local/include/horde3d)

add_executable (peakbin ${MAIN_SRC})
target_link_libraries (peakbin peakengine enet lua tolua++ -lHorde3D -lHorde3DUtils -lGL -lglfw BulletSoftBody BulletDynamics BulletCollision LinearMath)

