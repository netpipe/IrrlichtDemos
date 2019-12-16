set (ENGINE_LUA_PKG ${CMAKE_CURRENT_SOURCE_DIR}/peak.pkg)
set (ENGINE_LUA_SRC ${CMAKE_CURRENT_SOURCE_DIR}/lua_peak.cpp)

set (TOLUA "tolua++")
set (TOLUA_ARGS -n peak -o ${CMAKE_CURRENT_SOURCE_DIR}/lua_peak.cpp ${ENGINE_LUA_PKG})
add_custom_command (OUTPUT ${ENGINE_LUA_SRC}
	COMMAND ${TOLUA} ARGS ${TOLUA_ARGS})

set (ENGINE_SRC
core/Address.cpp
core/Util.cpp 
core/GameEngine.cpp 
core/Script.cpp 
core/SettingsManager.cpp 
core/Buffer.cpp 
core/Vector3D.cpp 
core/Vector2DI.cpp 
core/Vector2D.cpp 
core/File.cpp 
core/FileSystem.cpp
core/IniFile.cpp 
core/Quaternion.cpp 
core/Game.cpp 
core/Color.cpp 
core/Logger.cpp 
core/InputManager.cpp 
core/Keycode.cpp 
core/Timer.cpp 
core/Level.cpp 
core/Entity.cpp 
core/GameObject.cpp 
core/Property.cpp 
core/Link.cpp 
core/DirectLink.cpp 
core/EventScriptLink.cpp 
core/EventRemoteLink.cpp 
core/Event.cpp 
core/PropertyScriptLink.cpp 
network/NetworkHost.cpp 
network/BroadcastClient.cpp 
network/BroadcastHost.cpp 
network/Connection.cpp 
network/NetworkClient.cpp 
physics/Body.cpp 
physics/Shape.cpp 
physics/Box.cpp 
physics/Plane.cpp 
physics/World.cpp 
physics/Trimesh.cpp
physics/Sphere.cpp
physics/Callback.cpp
physics/RayCallback.cpp
physics/Heightfield.cpp 
graphics/GraphicsEngine.cpp 
graphics/SceneNode.cpp 
graphics/CameraSceneNode.cpp 
graphics/MeshSceneNode.cpp 
graphics/Image.cpp 
graphics/Light.cpp 
graphics/Material.cpp 
graphics/OverlayManager.cpp 
graphics/Overlay.cpp  
graphics/TextureOverlay.cpp 
graphics/LabelOverlay.cpp 
graphics/SceneNodeController.cpp 
graphics/FollowingController.cpp 
graphics/RotationController.cpp
graphics/ParticleSystem.cpp 
graphics/ModelSceneNode.cpp 
graphics/Terrain.cpp 
sound/SoundEngine.cpp 
sound/Sound.cpp 
sound/Listener.cpp 
sound/DataSoundSource.cpp 
sound/OggVorbisSource.cpp 
sound/SoundFactory.cpp 
menu/MenuManager.cpp 
menu/Menu.cpp 
menu/MenuElement.cpp 
menu/Label.cpp 
menu/Button.cpp 
menu/CheckBox.cpp 
menu/ListBox.cpp 
menu/ComboBox.cpp
gui/ImageLoader.cpp
gui/GUIInput.cpp
support/cd_wavefront.cpp
support/float_math.cpp
support/tinyxml.cpp
support/tinystr.cpp
support/tinyxmlerror.cpp
support/tinyxmlparser.cpp
${ENGINE_LUA_SRC}
)

include_directories (. ../include ../include/support /usr/include /usr/local/include /usr/include/lua5.1 /usr/local/include/horde3d /usr/local/include/bullet)

#set (LUA_LIBS_1 "`pkg-config lua5.1 --libs`")
#set (LUA_LIBS_2 "`pkg-config lua --libs`")


set (CMAKE_SHARED_LINKER_FLAGS "-llua -lm -L/usr/local/lib -Wl,-rpath,/usr/local/lib -lalut -lopenal -lvorbisfile -lvorbisenc -lvorbis -logg -g -lHorde3D -lHorde3DUtils -lfreeimageplus -lguichan -lguichan_opengl")
set (CMAKE_CXX_FLAGS "-Wall")

set (LIBRARY_OUTPUT_PATH ../../lib)
add_library(peakengine SHARED ${ENGINE_SRC})
set(CMAKE_BUILD_TYPE debug)

