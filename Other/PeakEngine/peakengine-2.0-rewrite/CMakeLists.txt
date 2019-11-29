
cmake_minimum_required(VERSION 2.6)

project(peakengine C CXX)

set(SRC
engine/src/core/Address.cpp
engine/src/core/Buffer.cpp
engine/src/core/Color.cpp
engine/src/core/Entity.cpp
engine/src/core/File.cpp
engine/src/core/FileSystem.cpp
engine/src/core/Game.cpp
engine/src/core/GameEngine.cpp
engine/src/core/IniFile.cpp
engine/src/core/Keycode.cpp
engine/src/core/Logger.cpp
engine/src/core/Model.cpp
engine/src/core/Object3D.cpp
engine/src/core/Quaternion.cpp
engine/src/core/SettingsManager.cpp
engine/src/core/Util.cpp
engine/src/core/Vector2D.cpp
engine/src/core/Vector2DI.cpp
engine/src/core/Vector3D.cpp
engine/src/graphics/GraphicsEngine.cpp
engine/src/graphics/SceneNode.cpp
engine/src/network/BroadcastClient.cpp
engine/src/network/BroadcastHost.cpp
engine/src/network/Connection.cpp
engine/src/network/NetworkClient.cpp
engine/src/network/NetworkHost.cpp
engine/src/physics/Body.cpp
engine/src/physics/Shape.cpp
engine/src/physics/World.cpp
test/src/main.cpp
)

set(CMAKE_CXX_FLAGS "-Wall -Wextra -W -Wno-long-long -Wno-unused-parameter -I/usr/local/include/horde3d -I/usr/local/include/bullet -g")
include_directories(test/include engine/include)
add_executable(peaktest ${SRC})
target_link_libraries(peaktest m Horde3D Horde3DUtils enet glfw freeimage BulletSoftBody BulletDynamics BulletCollision LinearMath guichan guichan_opengl lua tolua++ -lalut -lopenal -lvorbisfile -lvorbisenc -lvorbis)

